/* 
 * stockserver.c - A concurrent stock server using select()
 */ 
#include "csapp.h"

typedef struct {
    int maxfd;        /* Largest descriptor in read_set */
    fd_set read_set;  /* Set of all active descriptors */
    fd_set ready_set; /* Subset of descriptors ready for reading */
    int nready;       /* Number of ready descriptors from select */
    int maxi;         /* High water index into client array */
    int clientfd[FD_SETSIZE];    /* Set of active descriptors */
    rio_t clientrio[FD_SETSIZE]; /* Set of active read buffers */
} pool;

struct item {
    int ID;
    int left_stock;
    int price;
    int readcnt;
    sem_t mutex; // Protects readcnt
    sem_t w;     // Protects writing to the node
    struct item *left;
    struct item *right;
};

struct item *stock_root = NULL;

void init_pool(int listenfd, pool *p) {
    int i;
    p->maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++)
        p->clientfd[i] = -1;
    p->maxfd = listenfd;
    FD_ZERO(&p->read_set);
    FD_SET(listenfd, &p->read_set);
}

void add_client(int connfd, pool *p) {
    int i;
    p->nready--;
    for (i = 0; i < FD_SETSIZE; i++) {
        if (p->clientfd[i] < 0) {
            p->clientfd[i] = connfd;
            Rio_readinitb(&p->clientrio[i], connfd);
            FD_SET(connfd, &p->read_set);
            if (connfd > p->maxfd) p->maxfd = connfd;
            if (i > p->maxi) p->maxi = i;
            break;
        }
    }
    if (i == FD_SETSIZE) app_error("add_client error: Too many clients");
}

struct item *insert_stock(struct item *node, int ID, int left_stock, int price) {
    if (node == NULL) {
        node = (struct item *)Malloc(sizeof(struct item));
        node->ID = ID;
        node->left_stock = left_stock;
        node->price = price;
        node->readcnt = 0;
        Sem_init(&node->mutex, 0, 1);
        Sem_init(&node->w, 0, 1);
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    if (ID < node->ID) {
        node->left = insert_stock(node->left, ID, left_stock, price);
    } else if (ID > node->ID) {
        node->right = insert_stock(node->right, ID, left_stock, price);
    }
    return node;
}

void load_stocks(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;
    int id, left, price;
    while (fscanf(fp, "%d %d %d", &id, &left, &price) != EOF) {
        stock_root = insert_stock(stock_root, id, left, price);
    }
    fclose(fp);
}

void save_stocks_helper(struct item *node, FILE *fp) {
    if (node == NULL) return;
    save_stocks_helper(node->left, fp);
    fprintf(fp, "%d %d %d\n", node->ID, node->left_stock, node->price);
    save_stocks_helper(node->right, fp);
}

void save_stocks(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    save_stocks_helper(stock_root, fp);
    fclose(fp);
}

void sigint_handler(int sig) {
    save_stocks("stock.txt");
    exit(0);
}

struct item *find_stock(struct item *node, int ID) {
    if (node == NULL) return NULL;
    if (ID == node->ID) return node;
    else if (ID < node->ID) return find_stock(node->left, ID);
    else return find_stock(node->right, ID);
}

void show_stocks_helper(struct item *node, char *buf) {
    if (node == NULL) return;
    show_stocks_helper(node->left, buf);
    
    // Readers-Writers Lock (Reader)
    P(&node->mutex);
    node->readcnt++;
    if (node->readcnt == 1) P(&node->w);
    V(&node->mutex);
    
    char line[128];
    sprintf(line, "%d %d %d\n", node->ID, node->left_stock, node->price);
    strcat(buf, line);
    
    P(&node->mutex);
    node->readcnt--;
    if (node->readcnt == 0) V(&node->w);
    V(&node->mutex);
    
    show_stocks_helper(node->right, buf);
}

void check_clients(pool *p) {
    int i, connfd, n;
    char buf[MAXLINE];
    rio_t rio;

    for (i = 0; (i <= p->maxi) && (p->nready > 0); i++) {
        connfd = p->clientfd[i];
        rio = p->clientrio[i];

        if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))) {
            p->nready--;
            if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
                printf("server received %d bytes\n", n);
                
                char cmd[MAXLINE];
                int id = -1, amount = -1;
                char response[MAXLINE];
                memset(response, 0, MAXLINE);
                
                sscanf(buf, "%s %d %d", cmd, &id, &amount);
                
                if (!strcmp(cmd, "show")) {
                    show_stocks_helper(stock_root, response);
                } else if (!strcmp(cmd, "buy")) {
                    struct item *node = find_stock(stock_root, id);
                    if (node) {
                        // Readers-Writers Lock (Writer)
                        P(&node->w);
                        if (node->left_stock >= amount) {
                            node->left_stock -= amount;
                            strcpy(response, "[buy] success\n");
                        } else {
                            strcpy(response, "Not enough left stocks\n");
                        }
                        V(&node->w);
                    }
                } else if (!strcmp(cmd, "sell")) {
                    struct item *node = find_stock(stock_root, id);
                    if (node) {
                        // Readers-Writers Lock (Writer)
                        P(&node->w);
                        node->left_stock += amount;
                        strcpy(response, "[sell] success\n");
                        V(&node->w);
                    }
                } else if (!strcmp(cmd, "exit")) {
                    Close(connfd);
                    FD_CLR(connfd, &p->read_set);
                    p->clientfd[i] = -1;
                    continue;
                }
                
                Rio_writen(connfd, response, MAXLINE);
            } else {
                Close(connfd);
                FD_CLR(connfd, &p->read_set);
                p->clientfd[i] = -1;
            }
        }
    }
}

int main(int argc, char **argv) {
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    static pool pool;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    Signal(SIGINT, sigint_handler);
    load_stocks("stock.txt");

    listenfd = Open_listenfd(argv[1]);
    init_pool(listenfd, &pool);

    while (1) {
        pool.ready_set = pool.read_set;
        pool.nready = Select(pool.maxfd + 1, &pool.ready_set, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &pool.ready_set)) {
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            add_client(connfd, &pool);
        }

        check_clients(&pool);
    }
}
