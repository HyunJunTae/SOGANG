#include <stdio.h>



int main(void) {

	char a[50];
	char b[50];
	FILE *itp, *otp, *output;
	int res1, res2;
	char data1, data2;
	char c[1000];
	char d[1000];


	itp = fopen("test1.txt", "r");
	if(itp==NULL) {
		printf("File 1 open error!\n");
		return 1;
	}
	otp = fopen("test1.txt", "r");
        if(otp==NULL) {
                printf("File 2 open error!\n");
                return 1;
        }

	output = fopen("output.txt", "w");
	if(output==NULL) {
                printf("output file open error!\n");
                return 1;
        }


	int count = 0;
	while(1) {
		res1 = fscanf(itp, "%c", &c[0]);
		if(res1==EOF) {
			break;
		}
		res2 = fscanf(otp, "%c", &d[0]);
                if(res2==EOF) {
                        break;
                }
		count = count + 1;
	}
	count = count - 1;
	
	int count2 = 0;
	for(int i=0;i<count;i++) {
		if( c[i] == d[i]) {
			count2 += 1;
		}
	}

	fprintf(output, "Total character : %d\n", count);
	fprintf(output, "Same character : %d\n", count2);
	fprintf(output, "%.3f\n", (float)count2 / (float)count);


	fclose(itp);
	fclose(otp);
	fclose(output);
	
	return 0;
}

