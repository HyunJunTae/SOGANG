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

    // 1. 입력 간선 쭉 입력받기
    vector<int> edge_u(M), edge_v(M), edge_color(M);
    vector<long long> edge_cost(M);


    // 2. 각 vertex 별로 가지고있는 edge의 색깔을 vector로 저장
    vector<vector<int>> vertex_colors(N+1, vector<int>{0});

    for (int i=0; i<M; ++i) {
        cin >> edge_u[i] >> edge_v[i] >> edge_cost[i] >> edge_color[i];
        vertex_colors[edge_u[i]].push_back(edge_color[i]);
        vertex_colors[edge_v[i]].push_back(edge_color[i]);
    }


    // 3. vertex_colors를 정렬 및 중복 제거
    for (int i=1; i<=N; ++i) {
        auto& colors = vertex_colors[i];
        sort(colors.begin(), colors.end());
        colors.erase(unique(colors.begin(), colors.end()), colors.end());
    }



    // 4. 그래프 생성. graph[vertex][color의 vertex_colors에서의 인덱스] = 나가는 간선 목록.
    // (같은 정점에서 위로 올라가는 비용 0인 edge 만들기)
    vector<vector<vector<edge>>> graph(N+1);
    for (int i=1; i<=N; ++i) {

        // 4-1. 원래 노드가 가지고있는 색깔 개수만큼으로 쪼개기
        int layer_count = vertex_colors[i].size();
        graph[i].resize(layer_count);

        // 4-2. 쪼개진 각 노드에서 위로 올라가는 비용 0짜리 edge 만들기
        for (int layer=0; layer < layer_count-1; ++layer) {
            graph[i][layer].push_back({i, layer+1, 0});
        }
    }

    // 5. 쪼개진 vertex가 graph에서 몇 번째 인덱스에 있는지 알아내기
    auto layer_to_index = [&](int v, int color) {
        const auto& colors = vertex_colors[v];
        return(lower_bound(colors.begin(), colors.end(), color) - colors.begin());
    };


    // 6. 실제 edge를 연결
    for (int i=0; i<M; ++i) {
        int u_layer_index = layer_to_index(edge_u[i], edge_color[i]);
        int v_layer_index = layer_to_index(edge_v[i], edge_color[i]);
        graph[edge_u[i]][u_layer_index].push_back({edge_v[i], v_layer_index, edge_cost[i]});
        graph[edge_v[i]][v_layer_index].push_back({edge_u[i], u_layer_index, edge_cost[i]});
    }

    // 7. 최소 비용 배열 만들기 (min_costs[vertex][layer_index])
    vector<vector<long long>> min_costs(N+1);

    for (int i=1; i<=N; ++i) {
        min_costs[i].assign(vertex_colors[i].size(), LLONG_MAX);
    }

    priority_queue<status, vector<status>, greater<status>> pq;
    min_costs[S][0] = 0;
    pq.push({S, 0, 0});

    long long answer = -1;

    // 8. 다익스트라 알고리즘 실행


    // 8-2. 다익스트라 탐색 시작
    while(!pq.empty()) {

        status current_status = pq.top();
        pq.pop();

        int current_number = current_status.number;
        int current_layer = current_status.layer;
        long long current_cost = current_status.total_cost;

        // 8-2-1. 만약 노드 T에 도착했다면, cost 저장 후 종료
        if(current_number == T) {
            answer = current_cost;
            break;
        }

        // 8-2-2. 이미 더 적은 비용으로 해당 노드에 도착한 적이 있다면, 계산 생략
        if (current_cost > min_costs[current_number][current_layer]) {
            continue;
        }

        // 8-2-3. 해당 노드르 추가함으로써 갈 수 있는 경로들 업데이트
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
    

    // 9. 출력
    cout << answer;



    return 0;
}
