#include "csapp.h"
#include <time.h>
#include <sys/time.h>

#define MAX_CLIENT 100
#define ORDER_PER_CLIENT 1000  /* Increased for better performance analysis */
#define STOCK_NUM 10
#define BUY_SELL_MAX 10

int main(int argc, char **argv) 
{
	pid_t pids[MAX_CLIENT];
	int runprocess = 0, status, i;

	int clientfd, num_client;
	char *host, *port, buf[MAXLINE];
	rio_t rio;

    struct timeval start_time, end_time;
    double elapsed_time;

	if (argc != 4) {
		fprintf(stderr, "usage: %s <host> <port> <client#>\n", argv[0]);
		exit(0);
	}

	host = argv[1];
	port = argv[2];
	num_client = atoi(argv[3]);

    if (num_client > MAX_CLIENT) {
        fprintf(stderr, "client# should be less than %d\n", MAX_CLIENT);
        exit(0);
    }

    gettimeofday(&start_time, NULL);

	while(runprocess < num_client){
		pids[runprocess] = fork();

		if(pids[runprocess] < 0)
			return -1;
		else if(pids[runprocess] == 0){
			clientfd = Open_clientfd(host, port);
			Rio_readinitb(&rio, clientfd);
			srand((unsigned int) getpid());

			for(i=0;i<ORDER_PER_CLIENT;i++){
				int option = rand() % 3;
				
				if(option == 0){
					sprintf(buf, "show\n");
				}
				else if(option == 1){
					int list_num = rand() % STOCK_NUM + 1;
					int num_to_buy = rand() % BUY_SELL_MAX + 1;
					sprintf(buf, "buy %d %d\n", list_num, num_to_buy);
				}
				else if(option == 2){
					int list_num = rand() % STOCK_NUM + 1; 
					int num_to_sell = rand() % BUY_SELL_MAX + 1;
					sprintf(buf, "sell %d %d\n", list_num, num_to_sell);
				}
			
				Rio_writen(clientfd, buf, strlen(buf));
				Rio_readnb(&rio, buf, MAXLINE);
			}

			Close(clientfd);
			exit(0);
		}
		runprocess++;
	}

	for(i=0;i<num_client;i++){
		waitpid(pids[i], &status, 0);
	}

    gettimeofday(&end_time, NULL);
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 
                   (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    printf("\n============================================\n");
    printf("Test Results for %d clients, %d orders/client\n", num_client, ORDER_PER_CLIENT);
    printf("Total Elapsed Time: %.4f seconds\n", elapsed_time);
    printf("Throughput: %.2f orders/sec\n", (num_client * ORDER_PER_CLIENT) / elapsed_time);
    printf("============================================\n");

	return 0;
}
