#include <bits/stdc++.h>
#include <cmath>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const long double PI = 3.14159265358979323846;
    const long double eps = 1e-12L;

    // 1. 점 개수 입력받기.
    int N;
    cin >> N;

    // 1.1 만약 점이 1개면 무조건 됨.
    if (N==1) {
        cout << "Yes";
        return 0;
    }

    // 2. N개의 점 X, Y 좌표 입력받아서 X, Y 벡터에 저장.
    vector<long double> X(N), Y(N);
    for (int i = 0; i < N; ++i) {
        cin >> X[i] >> Y[i];
    }

    // 3. 각 좌표를 각도로 변환해서 벡터에 담기
    vector<long double> degree(N);
    for (int i = 0; i < N; ++i) {
        degree[i] = atan2(Y[i], X[i]);

        // 만약 라디안 값이 음수라면 2PI를 더해주기 -> 모든 각을 0 ~ 2파이 사이로 만들기
        // if (degree[i] < 0) degree[i] += 2*PI;

    }

    // 4. 각도 벡터를 정렬
    sort(degree.begin(), degree.end());

    // 5. 인접한 두 각이 각도가 180도가 넘는가 검토. 하나라도 그런 각이 있다면 half-circle 속성 만족.
    bool half_circle = false;
    for (int i = 0; i <= N-2; ++i) {
        if (degree[i+1] - degree[i] >= PI - eps) {
            half_circle = true;
            break;
        }
    }
    if (2*PI - (degree[N-1] - degree[0]) >= PI - eps) half_circle = true;

    // 6. 출력
    if (half_circle) cout << "Yes";
    else cout << "No";

    return 0;
}
