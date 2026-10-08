#include <bits/stdc++.h>
using namespace std;

int N, K;
vector<vector<pair<int, long long>>> adj;

// start에서 모든 노드까지 거리와 부모를 구함
void bfs(int start, vector<long long>& dist, vector<int>& par) {
    dist.assign(N + 1, -1);
    par.assign(N + 1, 0);
    queue<int> q;
    dist[start] = 0;
    q.push(start);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto [v, w] : adj[u]) {
            if (dist[v] != -1) continue;
            dist[v] = dist[u] + w;
            par[v] = u;
            q.push(v);
        }
    }
}

int farthest(const vector<long long>& dist) {
    int best = 1;
    for (int i = 1; i <= N; ++i) if (dist[i] > dist[best]) best = i;
    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> K;
    adj.assign(N + 1, {});
    for (int i = 0; i < N - 1; ++i) {
        int u, v; long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // 1. 지름: 아무 점 → 가장 먼 점 a → a에서 가장 먼 점 b
    vector<long long> dist; vector<int> par;
    bfs(1, dist, par);
    int a = farthest(dist);
    bfs(a, dist, par);
    int b = farthest(dist);
    long long D = dist[b];

    if (K == 1) { cout << D << '\n'; return 0; }

    // 2. 지름 경로 p[0]=a ... p[L-1]=b, a로부터의 거리 pos
    vector<int> path;
    for (int v = b; v != a; v = par[v]) path.push_back(v);
    path.push_back(a);
    reverse(path.begin(), path.end());
    int L = path.size();
    vector<long long> pos(L);
    for (int i = 0; i < L; ++i) pos[i] = dist[path[i]];

    // 3. h[i]: 경로 노드 i에서 경로가 아닌 쪽 가지의 최대 깊이
    vector<int> onPath(N + 1, -1);
    for (int i = 0; i < L; ++i) onPath[path[i]] = i;
    vector<long long> h(L, 0), depth(N + 1, -1);
    for (int i = 0; i < L; ++i) {
        int r = path[i];
        vector<int> st = {r};
        depth[r] = 0;
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            h[i] = max(h[i], depth[u]);
            for (auto [v, w] : adj[u]) {
                if (onPath[v] != -1 || depth[v] != -1) continue;
                depth[v] = depth[u] + w;
                st.push_back(v);
            }
        }
    }

    // 4. pre[i]: a쪽 조각(p[0..i])의 지름 = a에서 가장 먼 거리
    //    suf[i]: b쪽 조각(p[i..])의 지름 = b에서 가장 먼 거리
    vector<long long> pre(L), suf(L);
    for (int i = 0; i < L; ++i)
        pre[i] = max(i ? pre[i - 1] : 0LL, pos[i] + h[i]); // 이전 반복까지의 최대 거리 or a부터 이번 노드에서 뻗는 가지 끝까지의 거리
    for (int i = L - 1; i >= 0; --i)
        suf[i] = max(i + 1 < L ? suf[i + 1] : 0LL, D - pos[i] + h[i]);

    // 5. 지름 경로 위 간선 (p[i], p[i+1])을 자르는 경우 중 최소
    long long ans = D;   // 두 소방서를 같은 곳에 두는 경우 (N=1 포함)
    for (int i = 0; i + 1 < L; ++i)
        ans = min(ans, max(pre[i], suf[i + 1]));

    cout << ans << '\n';
    return 0;
}