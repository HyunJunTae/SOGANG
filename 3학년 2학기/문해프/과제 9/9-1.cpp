#include<iostream>
#include<queue>
#include<string>
#include<algorithm>
using namespace std;

// 노드 번호 할당. 흰색 노드 0 ~ n-1, 회색 노드 n ~ 2n-1
int assign_number (int here, int digit, int n) {
    int there = here * 10 + digit;

    // 현재 총 숫자가 n 이상이면 회색 노드
    if (there >= n) {
        return n + there % n;
    }

    // 현재 총 숫자가 n 미만이면 흰색 노드
    return there;
}

string Gifts(const string& d, int n, int m) {
    string answer;

    // 숫자 정렬.
    string digits = d;
    sort(digits.begin(), digits.end());

    // parent : vertex v에 처음 도착하기 직전의 vertex
    // choice : 그 때 붙인 숫자.
    vector<int> parent(2*n, -1);
    vector<int> choice(2*n, -1);
    queue<int> q;
    parent[0] = 0;
    q.push(0);

    // 그래프 만들기
    while(!q.empty()) {
        int here = q.front();
        q.pop();

        for (char c : digits) {
            int there = assign_number(here, c - '0', n);

            if (parent[there] == -1) {
                parent[there] = here;
                choice[there] = c - '0';
                q.push(there);
            }
        }
    }

    // 회색 m 노드가 목표 번호. 여기로 가야함.
    int goal = n + m;

    // 만약 회색 m 노드가 부모가 없다면, 여기로 못감 -> IMPOSSIBLE
    if (parent[goal] == -1) {
        return "IMPOSSIBLE";
    }

    

    // 그래프의 끝점에서 시작점까지 거슬러 올라가며 붙인 숫자 모으고 reverse
    for (int here = goal; here != 0; here = parent[here]) {
        answer += char('0' + choice[here]);
    }
    reverse(answer.begin(), answer.end());


    return answer;
}

int main() {
    ios_base::sync_with_stdio(0);
    int test_num;
    cin >> test_num;

    while (test_num--) {
        string d;
        int n, m;
        cin >> d >> n >> m;

        cout << Gifts(d, n, m) << "\n";
    }

    return 0;
}
