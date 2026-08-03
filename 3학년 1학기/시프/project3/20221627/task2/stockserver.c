/* 
 * stockserver.c - A concurrent stock server using threads and a shared buffer
 */ 
#include "csapp.h"

#define NTHREADS 20
#define SBUFSIZE 1024

/* sbuf_t: Shared buffer for connection descriptors */
typedef struct {
    int *buf;          /* Buffer array */         
    int n;             /* Maximum number of slots */
    int front;         /* buf[(front+1)%n] is first item */
    int rear;          /* buf[rear%n] is last item */
    sem_t mutex;       /* Protects accesses to buf */
    sem_t slots;       /* Counts available slots */
    sem_t items;       /* Counts available items */
} sbuf_t;

void sbuf_init(sbuf_t *sp, int n) {
    sp->buf = Calloc(n, sizeof(int)); 
    sp->n = n;                       /* Buffer holds n items */
    sp->front = sp->rear = 0;        /* Empty buffer iff front == rear */
    Sem_init(&sp->mutex, 0, 1);      /* Binary semaphore for locking */
    Sem_init(&sp->slots, 0, n);      /* Initially, buf has n empty slots */
    Sem_init(&sp->items, 0, 0);      /* Initially, buf has zero items */
}

void sbuf_insert(sbuf_t *sp, int item) {
    P(&sp->slots);                          /* Wait for available slot */
    P(&sp->mutex);                          /* Lock the buffer */
    sp->buf[(++sp->rear)%(sp->n)] = item;   /* Insert the item */
    V(&sp->mutex);                          /* Unlock the buffer */
    V(&sp->items);                          /* Announce available item */
}

int sbuf_remove(sbuf_t *sp) {
    int item;
    P(&sp->items);                          /* Wait for available item */
    P(&sp->mutex);                          /* Lock the buffer */
    item = sp->buf[(++sp->front)%(sp->n)];  /* Remove the item */
    V(&sp->mutex);                          /* Unlock the buffer */
    V(&sp->slots);                          /* Announce available slot */
    return item;
}

/* Stock data structures */
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
sbuf_t sbuf; /* Shared buffer of connected descriptors */

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

void process_client(int connfd) {
    int n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
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
            break;
        }
        
        Rio_writen(connfd, response, MAXLINE);
    }
}

void *thread(void *vargp) {
    Pthread_detach(pthread_self());
    while (1) {
        int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */
        process_client(connfd);          /* Service client */
        Close(connfd);
    }
}

int main(int argc, char **argv) {
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    Signal(SIGINT, sigint_handler);
    load_stocks("stock.txt");

    listenfd = Open_listenfd(argv[1]);
    sbuf_init(&sbuf, SBUFSIZE); 
    
    /* Create worker threads */
    for (int i = 0; i < NTHREADS; i++) {
        Pthread_create(&tid, NULL, thread, NULL);
    }

    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        sbuf_insert(&sbuf, connfd); /* Insert connfd into buffer */
    }
}
