#include <stdio.h>
#include <stdlib.h>

char a[20];                        
char b[20];                           
char d[10]="IVXLCDM";                
int di[10]={1,5,10,50,100,500,1000}; 

int get_index(char x) //어떤 문자의 값 위치를 찾기
{
    int i;
    for(i=0; i<=6; i++)
        if(d[i]==x) break;
    return i;
}

int RomeToArabia(char x[])
{
    int i, s=0;
    for(i=0; x[i]!='\0';i++);
    while(i){
        i--;
        if(i==0)     
            return s+di[get_index(x[i])];
        else if(get_index(x[i-1]) >= get_index(x[i]))
            s+=di[get_index(x[i])];
        else{          
            s+=di[get_index(x[i])];
            s-=di[get_index(x[i-1])];
            i--;
        }
    }
    return s; 
}
void ArabiaToRome(int k) //어떤 값을 전달 받으면, 단어로 출력하는 작업을 함
{
    int c[7]={};
    for(int i=6; i>=0; i--){ // 큰 문자 값부터 빼면서 세어 두고
        while(k>=di[i]){
            c[i]++;
            k = k-di[i];
        }
    }
    for(int i=1; i<=c[6]; i++) printf("M");

    for(int i=5; i>=1; i-=2)
    {
        if(c[i-1]==4 && c[i]==1) printf("%c%c", d[i-1], d[i+1]);
        else if(c[i-1]==4 && c[i]==0) printf("%c%c", d[i-1],d[i]);
        else
        {
            if(c[i]==1) printf("%c", d[i]);
            for(int j=1; j<=c[i-1]; j++) printf("%c", d[i-1]);
        }
    }
    printf("\n");
}
int main()
{
    scanf("%s",a);   
    scanf("%s",b);   
    printf("%d\n", RomeToArabia(a)+RomeToArabia(b));
    ArabiaToRome(RomeToArabia(a)+RomeToArabia(b)); 
    return 0;
}
