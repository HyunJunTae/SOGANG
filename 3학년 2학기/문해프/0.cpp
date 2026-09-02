#include <iostream>
using namespace std;


int main(void) {

    // 1. sequence 길이 입력받기.
    int n;
    cin >> n;

    // 2. sequence 입력받기.
    int max = 0;
    int temp_max = 0;
    int number;
    for (int i=0; i<n; i++) {
        cin >> number;

        // 만약 입력받은 숫자가 양수라면, temp_max에 더하고 max = temp_max
        if (number >= 0) {
            temp_max += number;

            if (max < temp_max) max = temp_max;
        }

        // 만약 입력받은 숫자가 음수라면, temp_max에 더해보고, 만약 0보다 작으면 temp_max를 0으로 초기화
        else {
            temp_max += number;

            if (temp_max < 0) temp_max = 0;
        }

    }


    cout << max << endl;


    return max;
}