#include <stdio.h>

int main(void){
    int start=100;
    int *pointer = &start;
    int num;
    int no2;
    int no1;

    printf("no1 : ");
    scanf("%d", &no1);
    scanf("%d", &no2);

    printf("\n-------------print start---------------\n");
    printf("start_value, memory_address : %d %p\n", start, &start);
    printf("no1_value, memory_address : %d %p\n", no1, &no1);
    printf("no2_value, memory_address : %d %p\n", no2, &no2);
    printf("-------------print end---------------\n\n");

    printf("change start's address with pointer : ");
    scanf("%d", &num);
    
    if (num % 2 == 1) {
    	//start = no1; 없어도됨
    	pointer = &no1;
    }
    else if (num % 2 == 0) {
	    //start = no2; 없어도됨
	    pointer = &no2;
    }


    printf("\n-------------print start---------------\n");
    printf("start_value, memory_address : %d %p\n", start, &start);
    printf("no1_value, memory_address : %d %p\n", no1, &no1);
    printf("no2_value, memory_address : %d %p\n", no2, &no2);
    printf("start_value, memory_address : %d %p\n", *pointer, pointer);
    printf("-------------print end---------------\n\n");

    printf("change start value with pointer : ");


    scanf("%d", pointer);
    // pointer 자체가 주소값이니까 따로 &를 붙일 필요가 없다. 바로 scanf로 입력 가능.


    // scanf("%d", &num);
    // start = num;
    // 이렇게 하면 왜안됨?


    printf("\n-------------print start---------------\n");
    printf("start_value, memory_address : %d %p\n", start, &start);
    printf("no1_value, memory_address : %d %p\n", no1, &no1);
    printf("no2_value, memory_address : %d %p\n", no2, &no2);
    printf("start_value, memory_address : %d %p\n", *pointer, pointer);
    printf("-------------print end---------------\n\n");
    return 0;
}

// 위 출력문을 비교해보면, 그냥 정수인 no1, no2를 출력할 때는
// no1, &no1 이런 식으로 출력함. no1 출력하면 말 그대로 no1의 값, &no1은 no1의 주소값.
// pointer 출력할 때는 *pointer, pointer 하는데, pointer 변수 자체가 포인터 변수니까 
// 그냥 pointer 출력하면 주소값 출력, *pointer 출력하면 pointer 주소에 있는 숫자를 출력.
// 함수 add(int *a, int *b) 가 있고, 이걸 부를 때 add(&a, &b) 이렇게 쓰는 이유가,
// 저렇게 하면 함수에서 값이 바뀌면 바로바로 저장되서 바뀌지, 그냥 숫자만 가져가는게 아니라
// 주소값 자체를 가져가니까. &a, &b로 주소값을 가져간 다음, *로 되돌리는건가?