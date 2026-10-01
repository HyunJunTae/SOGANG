#include <bits/stdc++.h>
#include <iostream>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long M;
    cin >> N >> M;


    // 0. 만약 거스름돈 요쳥한 사람이 1명이면 그냥 그에 해당하는 거스름돈 동전 하나 있으면 됨
    if (N==1) {
        cout << ceil(log2(M+1));
        return 0;
    }

    // 1. 동전을 담을 배열 정의
    int count = 0;

    // 2. 1원짜리 동전 세기
    count += N;


    // 3. 2원짜리 동전 세기
    if (M >= 2) {
        int two_coins = (N+1) / 2;
        count += two_coins;
    }

    // 4. 3원짜리 동전 세기
    if (M >= 3) {
        int half_coins = N/2;
        int three_coins = ceil( (2.0/3.0) * half_coins);
        count += three_coins;
    }


    cout << count;


    return 0;
}
