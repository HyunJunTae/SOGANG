#include <bits/stdc++.h>
using namespace std;


bool question(int i, long long x) {

    cout << "? " << " " << i << " " << x << endl;

    string ans;
    cin >> ans;

    if (ans == "-1") {
        exit(0);
    }

    return (ans == "Yes");
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    // N명을 랜덤 순서로 볼 수 있도록 셔플
    vector<int> q_order(N);
    for (int i=1; i<=N; ++i) {
        q_order[i-1] = i;
    }
    shuffle(q_order.begin(), q_order.end(), mt19937(time(NULL)));

    // 변수 선언
    long long bottom;
    long long top;
    long long middle;
    int richest = 1;
    long long current_rich_money = 0;
    

    for (int idx=0; idx<N; ++idx) {
        int i = q_order[idx];

        // 만약 현재 사람이 이전 사람보다 돈이 많다면, 이 사람도 검사해봐야함
        if (question(i, current_rich_money+1)) {
            bottom = current_rich_money;
            top = 1000000000;
            
            while(bottom < top) {
                middle = bottom + ((top - bottom + 1) / 2);

                bool ans = question(i, middle);

                // i가 가진 재산이 middle 이상일 경우
                if (ans) {
                    bottom = middle;
                }

                else {
                    top = middle - 1;
                }
            }

            richest = i;
            current_rich_money = bottom;

            if (current_rich_money == 1000000000) break;

        }
    }

    cout << "!" << " " << richest << " " << current_rich_money << endl;

    return 0;
}
