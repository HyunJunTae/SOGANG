#include "netsim2.h"
#include <vector>
#include <cstring>
#include <cstdint>
#include <algorithm>
using namespace std;

#define INF 999999

struct RouterState {
    int my_id;                                                          // 해당 노드 ID
    int my_seq_version;                                                 // 해당 노드의 LSA 시퀀스 번호
    int num_nodes;                                                      // 전체 노드 수
    vector<int> direct_links;                                           // 직접 연결된 이웃 링크들의 비용
    vector<vector<int>> total_link_state;                               // 전체 링크 상태
    vector<int> total_seq_version;                                      // 전체 노드가 보낸 정보의 시퀀스 번호
    vector<int> next_hop;                                               // 패킷을 특정 노드로 보내기 위해, 다음 hop을 어디로 해야할지를 저장
};


// 다익스트라 알고리즘을 통한 최단 경로 및 다음 홉 계산
static void run_dijkstra(RouterState* s) {
    vector<int> dist(s->num_nodes, INF);       // 최단 거리 배열
    vector<int> prev(s->num_nodes, -1);        // 특정 노드에 가기 위한 이전 노드 추적 배열
    vector<bool> visited(s->num_nodes, false); // 이미 최단 거리가 확정된 노드 담아둘 배열. 일단 다 false 로 설정.

    dist[s->my_id] = 0;

    for (int i = 0; i < s->num_nodes; i++) {
        int u = -1; // 아직 방문하지 않은 노드 중에 제일 dist 값이 작은 노드 ID
        int min_d = INF; // u까지의 거리

        // 1. 현재 집합 내에서 최단 거리 노드 찾아내기
        for (int j = 0; j < s->num_nodes; j++) {
            if (!visited[j] && dist[j] < min_d) {
                min_d = dist[j];
                u = j;
            }
        }
        
        // 만약 u와 min_d 가 기존 설정값이랑 그대로라면, 이미 최단거리 다 찾은거임 -> break
        if (u == -1 || min_d == INF) break;

        // 2. u 노드 방문 한걸로 설정
        visited[u] = true;

        // 3. 모든 노드에 v에 대해서, 기존 경로보다 u를 거쳐가는게 cost 더 낮으면 u를 거쳐가는 경로로 dist, prev 변경.
        for (int v = 0; v < s->num_nodes; v++) {
            int u_v_link_cost = s->total_link_state[u][v];
            int v_u_link_cost = s->total_link_state[v][u];
            
            // 양방향 모두 INF가 아닐 때만 유효한 링크로 간주하여 오류
            if (u_v_link_cost != INF && v_u_link_cost != INF) {
                int alt = dist[u] + u_v_link_cost;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }

    // 다익스트라 알고리즘은 위에서 다 끝남 -> dist, prev 배열 완성
    // 4. 지금 내 노드에서, 다른 노드로 가기 위해선 one hop을 어디로 가야하는가 를 저장 (next_hop 채우기)
    for (int i = 0; i < s->num_nodes; i++) {
        if (i == s->my_id) {
            s->next_hop[i] = i;
        } 
    
        else if (dist[i] == INF) {
            s->next_hop[i] = -1;
        } 
        
        else {
            int current_node = i;
            while (prev[current_node] != s->my_id && prev[current_node] != -1) {
                current_node = prev[current_node];
            }
            s->next_hop[i] = current_node;
        }
    }
}

// 자신의 최신 링크 상태를 이웃 노드들에게 브로드캐스트
static void broadcast_lsa(RouterState* s, int exclude_neighbor) {

    // active_neighbors 에, 직접 연결된 이웃들 추가
    vector<int> active_neighbors;
    for (int i = 0; i < s->num_nodes; i++) {
        if (s->direct_links[i] != INF) {
            active_neighbors.push_back(i);
        }
    }

    // 패킷 크기 계산, 메모리 할당
    int num_links = active_neighbors.size();
    int len = 12 + num_links * 8; // 12바이트 : 내 노드 id, 시퀀스 번호, 이웃 수 / num_links * 8 : 나와 직접 연결된 링크들의 정보는 이웃 노드 id, cost -> 개당 8바이트. 
    uint8_t* payload = new uint8_t[len];

    // 내 노드 id, 시퀀스 번호, 이웃 수
    int offset = 0;
    memcpy(payload + offset, &s->my_id, 4);
    offset += 4;

    memcpy(payload + offset, &s->my_seq_version, 4);
    offset += 4;

    memcpy(payload + offset, &num_links, 4);
    offset += 4;
    
    // 직접 연결된 이웃들 정보 담기
    for (int n : active_neighbors) {

        int cost = s->direct_links[n];

        memcpy(payload + offset, &n, 4); 
        offset += 4;

        memcpy(payload + offset, &cost, 4); 
        offset += 4;
    }

    // 포장된 LSA 패킷을 정보를 전달해준 노드(exclude_neighbor)만 제외하고 나머지 이웃들에게 전송
    for (int n : active_neighbors) {
        if (n != exclude_neighbor) {
            send_control(n, payload, len);
        }
    }
    delete[] payload; // 전송 완료 후 동적 할당했던 payload 메모리 해제
}

// 수신받은 LSA를 다른 이웃 노드에게 플러딩
static void forwarding_lsa(RouterState* s, int from, const uint8_t* payload, int len) {
    // 현재 살아있는 이웃 노드들을 전부 탐색
    for (int i = 0; i < s->num_nodes; i++) {
        // 나에게 이 메시지를 보내준 노드(from)가 아니고, 링크가 연결되어 있다면 해당 이웃에게 그대로 전달
        if (s->direct_links[i] != INF && i != from) {
            send_control(i, payload, len);
        }
    }
}


struct RouterState *router_init(int my_id, int num_nodes, const int *neighbor_ids, const int *link_costs, int num_neighbors) {
    
    // 기본값 할당
    RouterState* s = new RouterState();
    s->my_id = my_id;
    s->num_nodes = num_nodes;
    s->my_seq_version = 1;
    s->direct_links.assign(num_nodes, INF);
    s->total_link_state.assign(num_nodes, vector<int>(num_nodes, INF));
    s->total_seq_version.assign(num_nodes, 0);
    s->next_hop.assign(num_nodes, -1);

    // total_link_state 에서 노드 자기 자신으로 가는 링크들에게는 거리 0 부여
    for (int i = 0; i < num_nodes; i++) {
        s->total_link_state[i][i] = 0;
    }

    // 초기 이웃 노드 설정
    // 고립된 노드는 아닌지 검사
    if (num_neighbors > 0 && neighbor_ids != nullptr && link_costs != nullptr) {
        // neighbors_ids 배열과 link_costs 배열에서 순서대로 id 와 cost 가져오면서 값 할당
        for (int i = 0; i < num_neighbors; i++) {
            int n = neighbor_ids[i];
            s->direct_links[n] = link_costs[i];
            s->total_link_state[my_id][n] = link_costs[i];
            s->total_link_state[n][my_id] = link_costs[i]; 
        }
    }

    run_dijkstra(s);      // 초기 이웃 정보를 바탕으로 첫 번째 최단 경로 계산
    broadcast_lsa(s, -1); // 시뮬레이션 시작과 동시에 내 주변 링크 상태를 온 동네에 최초 광고 (제외 노드 없음: -1)

    return s;
}

void on_link_change(struct RouterState *s, int neighbor, int new_cost) {

    bool was_inf = (s->direct_links[neighbor] == INF); // 변경 전 링크가 없었는지 있었는지
    s->direct_links[neighbor] = (new_cost == NETSIM2_NO_LINK) ? INF : new_cost; // 링크 단절 신호면 INF로, 아니면 새 비용으로 갱신

    // total_link_state에 링크 변경 즉각 반영
    s->total_link_state[s->my_id][neighbor] = s->direct_links[neighbor];
    s->total_link_state[neighbor][s->my_id] = s->direct_links[neighbor];

    // 내 링크 상태 지도가 변경되었으므로 정보 버전 번호(시퀀스) 1 증가
    s->my_seq_version++;

    run_dijkstra(s);      // 지도가 바뀌었으므로 next_hop 재계산
    broadcast_lsa(s, -1); // 변경된 내 정보를 이웃들에게 새로 브로드캐스트

    // 새 링크가 연결되었거나 복구되었을 때, 연결된 이웃에게 total_link_state 동기화
    if (was_inf && new_cost != NETSIM2_NO_LINK) {
        // 전체 노드를 돌면서 내가 수집해둔 타 노드들의 최신 LSA 정보를 복원하여 전송
        for (int node = 0; node < s->num_nodes; node++) {

            // 내가 아니고, 한 번이라도 정보가 수집되었던(시퀀스 > 0) 유효한 노드 지도 데이터인 경우
            if (node != s->my_id && s->total_seq_version[node] > 0) {
                vector<int> n_links_list;
                // 해당 노드가 가지고 있는 활성 링크들을 전부 추출
                for (int v = 0; v < s->num_nodes; v++) {
                    if (node != v && s->total_link_state[node][v] != INF) {
                        n_links_list.push_back(v);
                    }
                }
                int n_links = n_links_list.size();
                int len = 12 + n_links * 8; // 복원할 LSA 패킷 크기 계산
                uint8_t* p = new uint8_t[len];
                int offset = 0;
                int seq = s->total_seq_version[node];
                
                // 해당 노드의 과거 최신 LSA 패킷 형태로 그대로 재포장 (복사)
                memcpy(p + offset, &node, 4); offset += 4;
                memcpy(p + offset, &seq, 4); offset += 4;
                memcpy(p + offset, &n_links, 4); offset += 4;
                
                for (int v : n_links_list) {
                    int c = s->total_link_state[node][v];
                    memcpy(p + offset, &v, 4); offset += 4;
                    memcpy(p + offset, &c, 4); offset += 4;
                }
                send_control(neighbor, p, len); // 새로 연결된 이웃(neighbor)에게만 이 과거 정보들을 밀어 넣어 동기화 시킴
                delete[] p;                     // 동적 할당 버퍼 해제
            }
        }
    }
}

void on_control(struct RouterState *s, int from, const uint8_t *payload, int len) {
    if (len < 12) return; // 패킷 헤더 크기(12바이트)보다 작으면 비정상 패킷이므로 무시
    
    int src, seq, num_links;
    int offset = 0;
    memcpy(&src, payload + offset, 4); offset += 4;       // 이 LSA를 처음 생성한 노드 ID 추출
    memcpy(&seq, payload + offset, 4); offset += 4;       // LSA의 버전 번호(시퀀스) 추출
    memcpy(&num_links, payload + offset, 4); offset += 4; // LSA에 포함된 링크 정보 개수 추출

    // 자신의 LSA가 돌아오면 무시
    if (src == s->my_id) return;

    // 최신 시퀀스 번호의 LSA만 처리하여 루프 방지
    if (seq > s->total_seq_version[src]) {
        s->total_seq_version[src] = seq; // 새로운 최신 버전 번호로 업데이트

        // src 노드의 이전 링크 상태 초기화 (새로운 정보로 갈아끼우기 전 전부 INF로 리셋)
        for (int i = 0; i < s->num_nodes; i++) {
            s->total_link_state[src][i] = INF;
        }
        s->total_link_state[src][src] = 0;

        // 패킷에 담겨온 src 노드의 최신 링크 정보들을 하나씩 꺼내서 전체 지도(total_link_state)에 반영
        for (int i = 0; i < num_links; i++) {
            int n, cost;
            memcpy(&n, payload + offset, 4); offset += 4;
            memcpy(&cost, payload + offset, 4); offset += 4;
            s->total_link_state[src][n] = cost;
        }

        run_dijkstra(s);                      // 전체 지도가 업데이트되었으므로 내 라우팅 테이블 재계산
        forwarding_lsa(s, from, payload, len); // 이 최신 소문을 내 다른 이웃들에게도 릴레이 플러딩 해줌
    }
}

int on_packet(struct RouterState *s, int dst) {
    int nexthop = s->next_hop[dst];
    // Next hop이 명확하고 현재 Up 상태의 링크 상태인지 확인
    if (nexthop != -1 && s->direct_links[nexthop] != INF) {
        return nexthop;
    }
    return -1; // 도달 불가능하면 명시적 Drop
}

void on_timer(struct RouterState *s) {
    // Link-State 기반으로 동작하므로 Timer가 불필요
}

void router_shutdown(struct RouterState *s) {
    delete s; // 동적 할당된 s 및 멤버 메모리 자동 반환
}