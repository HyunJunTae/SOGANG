#include <stdio.h>
#include <stdlib.h>

void Merge(int* arr1, int* arr2, int* newArr) {

	int num1, num2;
	int i = 0;
	int j = 0;
	int k = 0;
	while(arr1[i] != -1 || arr2[j] != -1) {

		if(arr1[i] == -1) {
			newArr[k] = arr2[j];
			j = j + 1;
		}
		else if(arr2[j] == -1) {
			newArr[k] = arr1[i];
			i = i + 1;
		}		
		else if(arr1[i] > arr2[j]) {
			newArr[k] = arr2[j];
			j = j + 1;
		}
		else if(arr1[i] <= arr2[j]) {
			newArr[k] = arr1[i];
			i = i + 1;
		}
		k = k + 1;
	}
}



int main(void) {

	int* arr1;
	int* arr2;

	arr1 = (int*)malloc(8*sizeof(int));
	arr2 = (int*)malloc(8*sizeof(int));


	int i;
	int num;
	for(i=0;i<8;i++) {
		scanf("%d", &num);
		arr1[i] = num;
		if(num == -1) {
			break;
		}
	}

	for(i=0;i<8;i++) {
                scanf("%d", &num);
                arr2[i] = num;
                if(num == -1) {
                        break;
                }
        }
	int* newArr;
	newArr = (int*)malloc(16*sizeof(int));

	Merge(arr1, arr2, newArr);
	
	for(i=0;i<16;i++) {
		if( newArr[i] == 0) {
                        break;
                }
		printf("%d ", newArr[i]);
	}

	printf("\n");

	free(arr1);
	free(arr2);
	free(newArr);
	return 0;
}
