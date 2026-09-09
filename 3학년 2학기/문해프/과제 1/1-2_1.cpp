#include<iostream>
#include<vector>
#include <algorithm>
using namespace std;

vector<int> min_multiplications(int n) {
    vector<int> powers;


    // 1. 이진분류로 풀기
    // 시작은 1, n의 절반보다 작거나 같다면 계속 2를 곱하기.
    vector<int> temp_powers;
    temp_powers.push_back(1);
    powers.push_back(1);
    int current_num=1;
    while(current_num*2 <= n) {
        current_num *= 2;
        temp_powers.push_back(current_num);
        powers.push_back(current_num);
    }

    // 멈췄다면, 거꾸로 올라가면서 더할 수 있는 최대 크기를 더하기
    for (int i=temp_powers.size()-1; i>=0; --i) {
        if (current_num + temp_powers[i] <= n) {
            current_num += temp_powers[i];
            powers.push_back(current_num);
        }

        if (current_num == n) {
            break;
        }
    }




    // BFS로 풀기
    // 1) 현재 가지고있는 배열들을 가지고, 만들 수 있는 모든 배열을 만들기 (중복 확인 해야함)
    // 2) 만들어진 배열들 중에서, 제일 큰 수를 {(이진분류 알고리즘 최적 경우) - (현재 연산 수)} 만큼 곱해도, n보다 작은 경우에는 해당 케이스 삭제.
    
    // vector<int>들을 담는 배열
    vector<vector<int>> bfs_powers;
    bfs_powers.push_back({1});

    int limit = powers.size()-1;
    int count = 0;
    int array_size;
    int last_element;

    while(limit-count != 0) {

        vector<vector<int>> bfstemp_powers;
        
        // bfs_powers의 배열을 하나씩 꺼내서, 가능한 모든 경우의 수를 bfstemp_powers에 담기
        // 배열 하나 꺼내오기
        for (const auto& temp : bfs_powers) {
            array_size = temp.size();
            last_element = temp.back();
            
            // 해당 배열에서 숫자 2개씩 골라서 곱해서 배열에 추가
            for (int i=0; i < array_size; ++i) {
                int k = temp[i] + last_element;

                // k가 현재 배열의 최대값보다 큰 경우 + k가 n보다 작거나 같은 경우 에 대해서만 이 배열을 다음 단계로 넘기기
                if (k > last_element && k <= n) {
                    vector<int> next_temp = temp;
                    next_temp.push_back(k);

                    // 만약 이게 정확하게 맞는 경우라면 이 경우를 리턴
                    if (k==n) {
                        return next_temp;
                    }

                    bfstemp_powers.push_back(next_temp);
                    }

                
            }

        }

        // 다음 단게에서 검사할 벡터들을 담을 벡터.
        vector<vector<int>> next_bfs_powers;


        // bfstemp_powers 중에서 제일 큰 수를 {(이진분류 알고리즘 최적 경우) - (현재 연산 수)} 만큼 곱해서 가능한 경우에만 temp_powers로 전달
        int mult = 1;
        for (int i=0; i<limit-count-1; i++) mult *= 2;
        for (auto temp : bfstemp_powers) {
            int max_value = temp.back();

            long long max_possible = (long long)max_value * mult;

            if (max_possible >= n) {
                next_bfs_powers.push_back(temp);
            }
        }
        bfs_powers = next_bfs_powers;
        
        ++count;
    }

    return powers;
}

int main(){

    int n;
    cin >> n;

    
    vector<int> steps = min_multiplications(n);
    cout << steps.size() - 1 << " ";
    for (int step : steps) {
        cout << step << " ";
    }

    return 0;
}