#include<iostream>
#include<algorithm>
#include<vector>
#include <stack>

using namespace std;


vector<vector<int>> adj;
vector<bool> visited;
int installed = 0;

int UNWATCHED = 0;
int CAMERA_SETTED = 1;
int WATCHED = 2;


int dfs(int here) {

    // 자식 상태 기록. 감시X -> 0 / 카메라설치됨->1 / 감시O->2
    vector<int> child_state_count = {0, 0, 0};

    // 만약 이 노드 처음 방문했다면, 방문했다고 표시
    if (visited[here] == false) {
        visited[here] = true;

        // 이 노드에서 방문할 수 있는, 아직 not visited인 다른 노드로 bfs 방문
        for (int i=0; i<adj[here].size(); ++i) {
            int there = adj[here][i];

            if (visited[there] == false) {
                ++child_state_count[dfs(there)];
            }
        }

    }

    // 만약 child_state_count의 모든 항이 0이라면 UNWATCHED를 반환
    // (아직 방문 안한 주변 노드가 없는 경우, 즉 리프 노드 or 혼자 있음)
    if (child_state_count[0] + child_state_count[1] + child_state_count[2] == 0) {
        return UNWATCHED;
    }

    // 만약 자식 노드 중에 감시 중이지 않은 곳이 있다면, 내가 설치
    // 즉 UNWATCHED인 자식 노드가 하나라도 있으면 CAMERA_SETTED 해야함.
    if(child_state_count[0] != 0) {
        ++installed;
        return CAMERA_SETTED;
    }


    // 만약 자식 노드 중에 감시 중인 곳이 있다면, 나는 이미 감시 받고 있음.
    // 즉 CAMERA_SETTED인 노드가 있다면, 나는 WATCHED
    if (child_state_count[1] != 0) {
        return WATCHED;
    }

    return UNWATCHED;
}

int solve(int G) {

    int answer = 0;
    installed = 0;

    for (int i=0; i<G; ++i) {
        // 혼자 있는 노드 (아직 visit 안함 + dfs에서 UNWATCHED가 리턴됨)
        if (visited[i] == false && dfs(i) == UNWATCHED) {
            ++installed;
        }
    }

    answer = installed;

	return answer;
}

int main(void) {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int C, H, G, p1, p2;
	cin >> C; // 테스트 케이스 수

	while (C--) {
		cin >> G >> H; // vertex 수, edge 수.
		adj = vector<vector<int>>(G); // 인접 리스트로 그래프 표현 
        visited = vector<bool> (G, false);

        // edge 추가.
		for (int i = 0; i < H; i++) {
			cin >> p1 >> p2;

			adj[p1].push_back(p2);
			adj[p2].push_back(p1);
		}

		cout << solve(G) << endl;
	}

	return 0;
}
