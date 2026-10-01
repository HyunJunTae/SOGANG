#include <bits/stdc++.h>
#include <iostream>
#include <queue>
#include <limits>
using namespace std;

// 각 vertex가 가지는 edge의 정보를 담는 구조체
struct edge {
    int number;
    int layer;
    long long cost;
};

// 다익스트라 탐색 시 ()이때까지 지난 경로 + 다음 edge)  cost가 제일 작은 거를 보기 위해 노드, 층, 코스트 한 번에 담는 구조체
struct status {
    int number;
    int layer;
    long long total_cost;

    // 내가 지금 보고있는게 비용이 더 작으면 
    bool operator>(const status& other) const {
        return total_cost > other.total_cost;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1. N M K S T 입력받기
    int N, M, K, S, T;
    cin >> N >> M >> K >> S >> T;

    // 2. 노드들을 쪼개서, 3차원 벡터로 나타내기. (n개의 노드가 k개로 쪼개지고, 각각이 edge들을 여러 개 가질 수 있음)
    vector<vector<vector<edge>>> graph(N+1, vector<vector<edge>>(K+1));


    // 3. 같은 노드가 쪼개진 경우 color 1 증가하는 노드끼리 비용 0인 edge로 연결
    for (int i=1; i<=N; ++i) {
        for (int c=1; c<K; ++c) {
            graph[i][c].push_back({i, c+1, 0});
        }
    }

    // 4. M개의 실제 엣지들을 입력받아서 연결
    int U, V, C;
    long long W;
    for (int i=0; i<M; ++i) {
        cin >> U >> V >> W >> C;

        graph[U][C].push_back({V, C, W});
        graph[V][C].push_back({U, C, W});
    }

    // 5. 다익스트라 알고리즘 실행
    // 5-1. 최소값 담을 배열과 최소힙 선언
    vector<vector<long long>> min_costs(N+1, vector<long long> (K+1, LLONG_MAX)); // 각 노드로 가는 최소 비용 저장
    priority_queue<status, vector<status>, greater<status>> pq;

    // 5-1. 시작 노드 우선순위 큐에 담기
    min_costs[S][1] = 0;
    pq.push({S, 1, 0});

    long long answer = -1;

    // 5-2. 다익스트라 탐색 시작
    while(!pq.empty()) {

        status current_status = pq.top();
        pq.pop();

        int current_number = current_status.number;
        int current_layer = current_status.layer;
        long long current_cost = current_status.cost;

        // 5-2-1. 만약 노드 T에 도착했다면, cost 저장 후 종료
        if(current_number == T) {
            answer = current_cost;
            break;
        }

        // 5-2-2. 이미 더 적은 비용으로 해당 노드에 도착한 적이 있다면, 계산 생략
        if (current_cost > min_costs[current_number][current_layer]) {
            continue;
        }

        // 5-2-3. 해당 노드르 추가함으로써 갈 수 있는 경로들 업데이트
        for (const edge& next_edge : graph[current_number][current_layer]) {
            int next_number = next_edge.number;
            int next_layer = next_edge.layer;
            long long next_cost = current_cost + next_edge.cost;

            if (next_cost < min_costs[next_number][next_layer]) {
                min_costs[next_number][next_layer] = next_cost;
                pq.push({next_number, next_layer, next_cost});
            }
        }
    }
    

    // 6. 출력
    cout << answer;



    return 0;
}
