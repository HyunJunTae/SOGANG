#include <bits/stdc++.h>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int N, K;
vector<vector<pair<int, long long>>> adjacency_list;

// bfs 함수. 시작점에서부터 모든 노드까지의 거리와, 각 노드의 부모 구하기
void bfs(int start, vector<long long>& dist, vector<int>& par) {
    dist.assign(N+1, -1);
    par.assign(N+1, 0);

    queue<int> q;
    q.push(start);
    dist[start] = 0;

    while(!q.empty()) {
        int u = q.front(); 
        q.pop();

        for(auto [v, w] : adjacency_list[u]) {
            if (dist[v] == -1) {
                q.push(v);
                dist[v] = dist[u] + w;
                par[v] = u;
            }

        }
    }

}


// dist 배열을 기준으로 제일 멀리있는 노드 찾기.
int farthest(const vector<long long>& dist) {
    int farthest_node = 1;
    for (int i=1; i<=N; ++i) {
        if (dist[i] > dist[farthest_node]) farthest_node = i;
    }

    return farthest_node;
}





int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1. 빌딩 개수 N, 소방서 개수 K 입력.
    cin >> N >> K;

    adjacency_list.assign(N+1, {});

    // 2. N-1개의 도로 정보 입력. (두 빌딩과 거리)
    for (int i = 0; i < N - 1; ++i) {
        int U, V;
        long long W;
        cin >> U >> V >> W;

        adjacency_list[U].push_back({V, W});
        adjacency_list[V].push_back({U, W});
    }




    // 3. 트리의 최대 거리 노드 a, b 구하기
    vector<long long> dist; // 노드까지의 거리
    vector<int> par;        // 각 노드의 부모 노드

    // 3-1. 1번 노드로 시작해서 제일 먼 노드 a 찾기.
    bfs(1, dist, par);
    int a = farthest(dist);

    // 3-2. a노드에서 제일 먼 노드 b 찾기.
    bfs(a, dist, par);
    int b = farthest(dist);

    // 3-3. 트리의 지름 구하기
    long long d = dist[b];

    // 3-4. 만약 K=1이라면, 지름의 정중앙에 두는게 정답.
    if (K==1) {
        cout << d;
        return 0;
    }



    // 4. 트리의 최대 거리 경로 구하기. (경로 상의 노드 번호 담기)
    vector<int> longest_path;
    for (int i=b; i!=a; i=par[i]) {
        longest_path.push_back(i);
    }
    longest_path.push_back(a);
    reverse(longest_path.begin(), longest_path.end());


    // 5. 트리 최대 거리 경로의 노드 수 구하기
    int longest_path_count = longest_path.size();

    // 6. a에서 지름 위의 각 노드까지의 거리 구하기
    vector<long long> a_to_node(longest_path_count);
    for (int i=0; i<longest_path_count; ++i) {
        a_to_node[i] = dist[longest_path[i]];
    }


    // 7. 지름 위의 각 노드들에 대해, 각 노드에서 지름방향 제외한 최대 거리 경로 찾기
    vector<bool> is_on_longest_path(N+1, false);
    for (int i=0; i<longest_path_count; ++i) is_on_longest_path[longest_path[i]] = true;

    vector<long long> longest_branch_path(longest_path_count, 0);
    vector<long long> depth(N+1, -1);

    for (int i=0; i<longest_path_count; ++i) {

        // 7-1. 현재 노드 설정
        int current_node = longest_path[i];

        // 7-2. 현재 노드에서 DFS 실행 -> 최대 branch 찾기
        vector<int> stack = {current_node};
        depth[current_node] = 0;

        while(!stack.empty()) {

            int u = stack.back();
            stack.pop_back();

            longest_branch_path[i] = max(longest_branch_path[i], depth[u]);

            for (auto [v, w] : adjacency_list[u]) {

                if (is_on_longest_path[v] == false && depth[v] == -1) {
                    depth[v] = depth[u] + w;
                    stack.push_back(v);
                }
            }

        }

    }


    // 8. (a에서 각 노드까지 + 각 노드에서 최대 거리 경로) 구하기.
    
    vector<long long> a_piece(longest_path_count);
    a_piece[0] = 0;
    for (int i=1; i<longest_path_count; ++i) {
        a_piece[i] = max(a_piece[i-1], a_to_node[i] + longest_branch_path[i]);
    }

    //    (b에서 각 노드까지 + 각 노드에서 최대 거리 경로) 구하기
    vector<long long> b_piece(longest_path_count);
    b_piece[longest_path_count-1] = 0;
    for (int i=longest_path_count-2; i>=0; --i) {
        b_piece[i] = max(b_piece[i+1], d - a_to_node[i] + longest_branch_path[i]);
    }


    // 9. 지름 위의 간선들을 하나씩 잘랐을 때, 가장 경로 값이 작은 경우 찾기
    long long answer = d;
    for (int i=0; i<=longest_path_count-2; ++i) {
        answer = min(answer, max(a_piece[i], b_piece[i+1]));
    }

    
    // 10. 출력
    cout << answer;

    return 0;
}







    // K=1 ->   최장 거리 경로 찾기
    //          그거 정확히 반 가르는 자리에 소방서 놓기. (비용만 알면 반 나누면 됨)


    // K=2 ->   최장 거리 경로 찾기 (어떤 노드들을 지나가는거 다 알아야함.)
    //          최장 거리 경로 이루는 간선들 하나씩 골라서, 그거 기준으로 트리 쪼개기
    //              쪼개진 두 서브트리에서, 각각 최장 거리 경로 찾기
    //              쪼개진 두 서브트리에서, 각각 최정 거리 경로 한 가운데에 소방서 놓고 최대 거리 재기
    //          위를 모든 가능한 간선 쪼개기로 다 구해보고, 제일 짧은 경우를 선택.
    /*          
    근데 이렇게 하면 너무 오래 걸림
    -> 최장 거리 경로 찾기 (노드 a에서 b 까지가 최장 경로라고 하자.)
    -> 어떻게 쪼개든, 한쪽 서브트리에는 노드 a, 다른 한 쪽에는 b가 들어감.
    -> 서브트리 1에서, 어떤 경로든 무조건 a에서 출발하는게 다른데서 출발하는 것보다 더 멀거나 같음
        왜? -> 만약 서브트리 1에서 a가 끝점이 아닌 다른 최장 경로가 있다고 하자. (u, v 라고 하자.)
        트리 구조니까, u->v 와 a->v는 최장 경로를 이루는 노드 중에 하나인 x에서 만난다.
        즉, u->?->x->Y-> v 와 u->?->x->Y->v 가 될거다.
        이 때 x->Y->v 는 같음. 그러니까 u->?->x 가 a->?->x보다 길다는거임.
        그런데 a->b 가 최장거리 이기 때문에, a->b 경로 상의 모든 분기점으로부터 뻗어나온 각 가지는,
        해당 분기점에서 a로 가는 길에 제일 김.
        -> u->?->x 가 a->?->x보다 길었으면 그게 최장경로였겠지. 근데 a->?->x가 더 길잖아?
        -> 그러니까 u는 존재할 수 없고, 반드시 서브트리 1의 최장경로에는 a가 포함됨.
    -> a->b 경로의 모든 노드에서, 최장 거리 경로 상의 길을 제외한, 각 노드에서의 가장 깊은 길을 기록해두기
    -> 서브트리 매 번 만들 때 마다, a에서 쪼개진 부분까지의 각 노드에서 제일 깊은 거리 중에 제일 깊은게 최대 거리
        동시에, b에서 쪼개진 부분까지의 각 노드에서 제일 깊은 거리 중에 제일 깊은게 최대 거리


    */