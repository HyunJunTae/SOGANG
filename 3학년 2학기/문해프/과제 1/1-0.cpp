#include <iostream>
#include <cassert>

/**
 * @brief Asks person A if they know person B.
 * @param a The number of person A.
 * @param b The number of person B.
 * @return true if A knows B, otherwise returns false.
 */
bool ask_a_to_know_b(int a, int b) {
    int result;
    std::cout << "? " << a << ' ' << b << std::endl;
    std::cin >> result;
    assert(result == 0 || result == 1);
    return result;
}

/**
 * @brief Verifies if person x is a celebrity.
 * @param x The number of the person to verify, or -1 if there is no celebrity.
 * @return true if the answer is correct, otherwise returns false.
 */
bool answer(int x) {
    int result;
    std::cout << "! " << x << std::endl;
    std::cin >> result;
    assert(result == 0 || result == 1);
    return result;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // 총 몇 명이 있는가 입력받는다
    int n;
    std::cin >> n;

    // 맨 첫 번째 사람을 current_person으로 설정하고, 아래를 반복한다.
    // 만약 a가 b를 안다면, a는 celebrity가 아님.-> b를 current_person으로 설정.
    // 만약 모른다면, b는 celebrity가 아님. -> a를 그대로 current_person으로 설정.
    int current_person = 1;
    int ask_result;
    for (int i=2; i<=n; ++i) {
        ask_result = ask_a_to_know_b(current_person, i);

        // a가 b를 아는 경우 -> a가 celebrity가 아님. b를 새로운 current_person로 설정.
        if (ask_result == 1) {
            current_person = i;
        }

        // a가 b를 모르는 경우 -> b가 celebrity가 아님. current_person을 그대로 유지
        else {
            continue;
        }
    }

    // 한 명 남은 current_person 후보가, celebrity인가 아닌가 검사
    for (int i=1; i<=n; ++i) {

        if (current_person == i) continue;

        // 후보가 i를 아는지 물어보기. 만약 안다면 celebrity가 아님.
        if (ask_a_to_know_b(current_person, i) == 1) answer(-1);

        // i가 후보를 아는지 물어보기. 만약 모른다면 celebrity가 아님.
        if (ask_a_to_know_b(i, current_person) == 0) answer(-1);

    }

    answer(current_person);

    return 0;
}
