#include <bits/stdc++.h>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;


    // 1. 크기가 (N+1)x(N+1)인 벡터 A 선언 및 값 채워넣기
    vector<vector<int>> A(N+1, vector<int> (N+1, 0));
    for (int i=1; i<=N; ++i) {
        for (int j=1; j<=N; ++j) {
            cin >> A[i][j];
        }
    }

    // 2. 크기가 (N+2)x(N+2)인 벡터 C, 크기가 (N+1)x(N+1)인 벡터 B 선언
    vector<vector<int>> C(N+2, vector<int> (N+2, 0));
    vector<vector<int>> B(N+1, vector<int> (N+1, 0));


    // 3. Q를 몇 번 반복할건지 입력받기.
    int Q;
    cin >> Q;

    // 4. 아래를 Q번 반복
    int R1, C1, R2, C2, V;
    for (int i=0; i<Q; ++i) {
        // 3-1. 값 더하기 범위 입력받기
        cin >> R1 >> C1 >> R2 >> C2 >> V;

        // 3-2. 범위 값을 차분 배열 C에 저장
        C[R1][C1] += V;
        C[R2+1][C2+1] += V;
        C[R2+1][C1] += -1 * V;
        C[R1][C2+1] += -1 * V;

    }


    // 5. 누적합 배열 B에 계산. ( B[i][j] = C[i][j] + B[i-1][j] + B[i][j-1] - B[i-1][j-1])
    for (int i=1; i<=N; ++i) {
        for (int j=1; j<=N; ++j) {
            
            B[i][j] = C[i][j] + B[i-1][j] + B[i][j-1] - B[i-1][j-1];

        }
    }

    // 6. 기존 배열 A에 B 더하기
    for (int i=1; i<=N; ++i) {
        for (int j=1; j<=N; ++j) {
            
            A[i][j] += B[i][j];

        }
    }

    // 7. 전체 출력
    for (int i=1; i<=N; ++i) {
        for (int j=1; j<=N; ++j) {
            cout << A[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}
