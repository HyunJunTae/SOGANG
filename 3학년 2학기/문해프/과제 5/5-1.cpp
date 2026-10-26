#include <bits/stdc++.h>
using namespace std;

struct Light {
    long long x, y, s, e;

    bool operator<(Light other) {
        // 둘 다 가능
        // if (s != other.s) return s < other.s;
        // return e < other.e;
        if (e != other.e) return e < other.e;
        return s < other.s;
    }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1. 불빛들을 struct 형태로 저장. 시작점까지 포함하여 N+1개 저장.
    int N;
    cin >> N;

    vector<Light> lights(N+1);
    lights[0] = {0, 0, 0, 0};

    for (int i = 1; i <= N; ++i) {
        long long X, Y, S, E;
        cin >> X >> Y >> S >> E;
        lights[i] = {X, Y, S, E};
    }


    // 2. 불빛들을 시작 시간과 도착 시간을 기반으로 정렬
    sort(lights.begin()+1, lights.end());


    // 3. dp 방식으로 순서대로 최대값 찾기.
    // dp[i] : i번째 불빛이 마지막 도착지일 때, dp[0]에서 dp[i-1]까지에서 i번째 빛 충전량 더하기.
    vector<long long> dp(N+1);
    dp[0] = 0;

    for (int i=1; i<=N; ++i) {

        // dp[i]를 계산해야함. 일단 0으로 초기화.
        dp[i] = -1;

        for (int j=0; j<i; ++j) {

            // 빛 j에서 여기로 출발하는거 자체가 말이 안된다 (예를 들어 빛 j 좌표에 도달할 만큼 아직 시간이 안지난 경우.)
            if (dp[j] == -1) {
                continue;
            }

            long long charge_amount;

            // 빛 j에서 빛 i까지의 거리와, 빛 i에 도착하는 시간 계산.
            long long distance = abs(lights[i].x - lights[j].x) + abs(lights[i].y - lights[j].y);
            long long arrival_time = lights[j].e + distance;

            // 만약 빛 i에 도착하는 시간이 빛 i의 시작 시간보다 빠르거나 같다 -> 풀충전 가능
            if (arrival_time <= lights[i].s) {
                charge_amount = lights[i].e - lights[i].s;
            }

            // 만약 빛 i에 도착하는 시간이 빛 i의 끝나는 시간보다 늦거나 같다 -> 해당 경로 고려 X
            else if (arrival_time >= lights[i].e) {
                continue;
            }

            // 만약 빛 i에 도착하는 시간이 빛 i 시작시간 이후, 끝나는 시간 전이다 -> 빛 i 끝시간 - 도착시간 충전 가능
            else {
                charge_amount = lights[i].e - arrival_time;
            }

            
            // 만약 dp[j]의 최대량과 여기서 i로 와서의 충전량이 dp[i]보다 크다면, dp[i]를 업데이트.
            if (dp[i] < dp[j] + charge_amount) dp[i] = dp[j] + charge_amount;

        }
    }

    // 최대 충전량 구하기 및 출력
    long long max_charge = 0;
    for (int i=1; i<=N; ++i) {
        if (max_charge < dp[i]) max_charge = dp[i];
    }

    cout << max_charge;

    return 0;
}