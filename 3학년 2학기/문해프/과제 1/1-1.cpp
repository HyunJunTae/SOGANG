#include <iostream>
#include <vector>
using namespace std;

int min_steps(int n) {

    // 소인수분해 하면서, 각 소수를 더하기.
    int count = 0;
    int i=2;
    int temp_n = n;

    while(temp_n != 1) {

        // 만약 i가 n을 구성하는 소수라면, 해당 숫자를 count에 더하고 같은 숫자로 한 번 더 계산해보기
        if (temp_n % i == 0) {
            count += i;
            temp_n /= i;
        }

        // 만약 i가 n을 구성하는 소수가 아니라면, 다음 숫자로 넘어가기
        else {
            ++i;
        }
    }

    return count;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) {
        cout << 0 << "\n";
        return 0;
    }
    cout << min_steps(n) << "\n";
    return 0;
}