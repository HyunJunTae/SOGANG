#include <iostream>
#include <cassert>
#include <cstdlib>

/**
 * @brief Queries the value stored at cell (r, c).
 * @param r The row index, where 1 <= r <= n.
 * @param c The column index, where 1 <= c <= n.
 * @return The integer stored at cell (r, c).
 */
int query_cell(int r, int c) {
    int result;
    std::cout << "? " << r << ' ' << c << std::endl;

    if (!(std::cin >> result)) {
        std::exit(0);
    }

    return result;
}

/**
 * @brief Submits cell (r, c) as the position of K and terminates the program.
 * @param r The row index, where 1 <= r <= n.
 * @param c The column index, where 1 <= c <= n.
 */
void answer_with_cell(int r, int c) {
    int result;
    std::cout << "! " << r << ' ' << c << std::endl;

    if (!(std::cin >> result)) {
        std::exit(0);
    }

    assert(result == 0 || result == 1);
    std::exit(0);
}

/**
 * @brief Submits that K does not occur in the matrix and terminates the program.
 */
void answer_without_cell() {
    int result;
    std::cout << "! -1 -1" << std::endl;

    if (!(std::cin >> result)) {
        std::exit(0);
    }

    assert(result == 0 || result == 1);
    std::exit(0);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;


    // (0. 0)에서 시작
    int i=1, j=1;   // i행 j열
    int check;
    while(true) {
        // 좌표 값 물어보기
        check = query_cell(i, j);

        // 만약 그 좌표 값이 k와 같다면 -> answer with cell
        if (check == k) {
            answer_with_cell(i, j);
        }

        // 만약 그 좌표 값이 k보다 크다면 -> 검사 좌표를 아래로 이동
        else if (check > k) {
            ++i;
        }

        // 만약 그 좌표 값이 k보다 작다면 -> 검사 좌표를 오른쪽으로 이동
        else if (check < k) {
            ++j;
        }

        // 만약 이동한 좌표값이 n보다 크다면 answer without cell
        if (i > n || j > n) {
            answer_without_cell();
        }
    }


    return 0;
}