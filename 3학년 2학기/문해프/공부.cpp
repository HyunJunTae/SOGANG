


// 1-2. Minimum Calculation

bool dfs(vector<int>& powers, int n, int limit, int count) {

    int last_element = powers.back();
    
    // 1. 만약 이미 limit회만큼 연산을 했다면, 현재 배열의 마지막 원소를 검사.
    if (count == limit) {
        return (last_element == n);
    }

    // 2. 만약 현재 남은 횟수만큼 제일 큰거 계속 곱해도 안된다면, 이 케이스는 노답
    int mult = 1;
    for (int i=0; i<limit-count; i++) mult *= 2;
    long long max_possible = (long long)last_element * mult;
    if (max_possible < n) {
        return false;
    }

    // 3. powers 를 돌면서, 원소 새로 추가하고 다음 dfs로 넘기기
    for (int i=0; i<powers.size(); ++i) {
        for (int j=i; j<powers.size(); ++j) {

            // 3-1. powers에 넣을 값 k 게산해서, k가 현재 powers의 마지막 원소보다 크고, n보다 작거나 같은 경우에만 dfs 실행.
            int k = powers[i] + powers[j];


            if (k > last_element && k <= n) {
                powers.push_back(k);

                if (k == n) return true;

                // 3-2. dfs 결과가 true였다면, 찾은거니까 return true
                if (dfs(powers, n, limit, count+1)) {
                    return true;
                }

                // 3-3. dfs 결과가 false였다면, 못찾은거임
                else {
                    powers.pop_back();
                }
            }

        }
    }

    return false;
}


vector<int> min_multiplications(int n) {

    // 1. 만약 n이 1이면 1을 return.
    if (n==1) return {1};

    vector<int> powers = {1};

    // 2. 최대 깊이 상한선을 점진적으로 늘려가며 DFS로 풀기
    for (int limit=1; ; ++limit) {

        // 2-1. 만약 이번 limit 회차에서 true인 리턴값이 반환되면, powers에 다 담겨있을거임. break하면 됨
        if ( dfs(powers, n, limit, 0) ) {
            break;
        }
    }

    return powers;
}