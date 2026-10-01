#include <bits/stdc++.h>
#include <iostream>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N, M;
    cin >> N >> M;

    long long sum = 0;
    long long count = 0;

    long long coin;
    while(sum < N * M) {
        coin = sum / N + 1;
        sum += coin;
        ++count;
    }


    cout << count;


    return 0;
}
