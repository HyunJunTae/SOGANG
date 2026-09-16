#include <iostream>
#include <vector>
#include <queue>
using namespace std;

vector<int> findNLargestElements(const vector<vector<int>>& matrix, int n) {
    vector<int> allElements;
    vector<int> largestElements;
    
    // allElements에 값 입력
    for (int i=0; i<n; ++i) {
        for (int j=0; j<n; ++j) {
            allElements.push_back(matrix[i][j]);
        }
    }

    // 최대힙 만들기
    priority_queue<int> pq(allElements.begin(), allElements.end());


    // pq에서 top에 있는거 꺼내고, 다시 최대힙 만들고 를 n번 반복
    for (int i=0; i<n; ++i) {
        largestElements.push_back(pq.top());
        pq.pop();
    }
    
    
    return largestElements;
}

int main() {
    // Do NOT delete these lines unless you know what you are doing:
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> M(n, vector<int>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> M[i][j];
        }
    }

    vector<int> largestElements = findNLargestElements(M, n);

    for (int element : largestElements) {
        cout << element << "\n";
    }

    return 0;
}
