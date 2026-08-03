#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 50

typedef enum {head, entry} tagfield;

typedef struct entry_node {
    int row;
    int col;
    int value;
} entry_node;

typedef struct matrix_node *matrix_pointer;
typedef struct matrix_node {
    matrix_pointer down;
    matrix_pointer right;
    tagfield tag;
    union {
        matrix_pointer next;
        struct entry_node entry;
    }u;
} matrix_node;

matrix_pointer hdnode[MAX_SIZE];
matrix_pointer hdnode_t[MAX_SIZE];

matrix_pointer new_node() {
    matrix_pointer temp = (matrix_pointer)malloc(sizeof(struct matrix_node));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    return temp;
}

matrix_pointer mread() { 
    /* read in a matrix and set up its linked representation.
    An auxiliary global array hdnode is used */

    FILE *inputFile;

    // input.txt 파일 열기
    inputFile = fopen("input.txt", "r");

    int num_rows, num_cols, num_terms, num_heads, i;
    int row, col, value, current_row;
    matrix_pointer temp, last, node;

    // 파일에서 첫 줄의 row수, col수, 0이 아닌 원소 개수 읽어오기
    fscanf(inputFile, "%d %d %d", &num_rows, &num_cols, &num_terms);
    
    num_heads = (num_cols > num_rows) ? num_cols : num_rows;
    // 메인 노드 설정
    node = new_node();
    node->tag = entry;
    node->u.entry.row = num_rows;
    node->u.entry.col = num_cols;
    node->u.entry.value = num_terms;

    if (!num_heads)
        node->right = node;
    else
    { // 헤더 노드 설정
        for (i = 0; i < num_heads; i++)
        {
            temp = new_node();
            hdnode[i] = temp;
            hdnode[i]->tag = head;
            hdnode[i]->right = temp;
            hdnode[i]->u.next = temp;
        }


        current_row = 0;
        last = hdnode[0];
        for (i = 0; i < num_terms; i++)
        {   
            
            // row col value 값 받아오기
            fscanf(inputFile, "%d %d %d", &row, &col, &value);

            // 만약 받은 row가 지금 보고있는 행, 즉 current_row 보다 값이 큰 행이라면
            // 기존에 보던 행 right 연결시켜주고
            // 해당 행으로 이동
            if (row > current_row) { /* close current row */
                last->right = hdnode[current_row];
                current_row = row;
                last = hdnode[row];
            }

            // temp 노드 만들기
            temp = new_node();
            temp->tag = entry;
            temp->u.entry.row = row;
            temp->u.entry.col = col;
            temp->u.entry.value = value;

            // right 설정
            last->right = temp; /* link into row list */
            last = temp;

            // down 설정
            hdnode[col]->u.next->down = temp; /* link into column list */
            hdnode[col]->u.next = temp;
        }
        // 마지막 행 right 닫아주기
        last->right = hdnode[current_row];
        // 각 열 닫아주기
        for (i = 0; i < num_cols; i++)
            hdnode[i]->u.next->down = hdnode[i];
        // 각 헤더노드 연결
        for (i = 0; i < num_heads - 1; i++)
            hdnode[i]->u.next = hdnode[i + 1];
        hdnode[num_heads - 1]->u.next = node;
        node->right = hdnode[0];
    }

    fclose(inputFile);

    return node;
}

matrix_pointer mtranspose(matrix_pointer node) {

    matrix_pointer nodeT, temp;
    // 메인 노드 설정
    nodeT = new_node();
    nodeT->tag = entry;
    nodeT->u.entry.row = node->u.entry.col;
    nodeT->u.entry.col = node->u.entry.row;
    nodeT->u.entry.value = node->u.entry.value;

    
    // 헤더 노드 설정
    int num_heads = (nodeT->u.entry.col > nodeT->u.entry.row) ? nodeT->u.entry.col : nodeT->u.entry.row;

    if (!num_heads) {
        node->right = node;
    }
    
    else {
        for (int i = 0; i < num_heads; i++) {
                temp = new_node();
                hdnode_t[i] = temp;
                hdnode_t[i]->tag = head;
                hdnode_t[i]->right = temp;
                hdnode_t[i]->u.next = temp;
            }


        int current_col_for_hdnode = 0;
        matrix_pointer node_from_hdnode = hdnode[0];
        int current_row = 0;
        matrix_pointer last = hdnode_t[0];
        int row, col, value;
        for (int i = 0; i < node->u.entry.value; i++) {

            // 원래 매트릭스에서의 다음 노드만 찾아내면 됨.
            int found = 0;
            while(!found) {
                node_from_hdnode = node_from_hdnode->down;
    
                if (node_from_hdnode->tag == entry) {
                    found = 1;
                }
                else if (node_from_hdnode->tag == head) {
                    current_col_for_hdnode++;
                    node_from_hdnode = hdnode[current_col_for_hdnode];
                }
            }

            // row col value 값 받아오기
            row = node_from_hdnode->u.entry.col;
            col= node_from_hdnode->u.entry.row;
            value = node_from_hdnode->u.entry.value;

            printf("%d %d %d\n", row, col, value);

            // 만약 받은 row가 지금 보고있는 행, 즉 current_row 보다 값이 큰 행이라면
            // 기존에 보던 행 right 연결시켜주고
            // 해당 행으로 이동
            if (row > current_row) { /* close current row */
                last->right = hdnode_t[current_row];
                current_row = row;
                last = hdnode_t[row];
            }

            // temp 노드 만들기
            temp = new_node();
            temp->tag = entry;
            temp->u.entry.row = row;
            temp->u.entry.col = col;
            temp->u.entry.value = value;

            // right 설정
            // last 는 해당 row의 맨 마지막 노드를 가리킴.
            // 해당 row의 맨 마지막 노드가 temp 를 가리키게 하고
            // temp 를 해당 row 의 맨 마지막 노드로 재설정
            last->right = temp;
            last = temp;

            // down 설정
            // 해당 column 의 next 노드(컬럼의 맨 마지막 노드)의 down 이 temp를 가리키게
            // 그리고 해당 column의 next 노드를 temp로 변경.
            hdnode_t[col]->u.next->down = temp;
            hdnode_t[col]->u.next = temp;
        }
        // 마지막 행 right 닫아주기
        last->right = hdnode_t[current_row];
        // 각 열 닫아주기
        for (int i = 0; i < node->u.entry.row; i++)
            hdnode_t[i]->u.next->down = hdnode_t[i];
        // 각 헤더노드 연결
        for (int i = 0; i < num_heads - 1; i++)
            hdnode_t[i]->u.next = hdnode_t[i + 1];
        hdnode_t[num_heads - 1]->u.next = nodeT;
        nodeT->right = hdnode_t[0];

    }
}

void mwrite(matrix_pointer node) {

    FILE *outputFile;

    outputFile = fopen("output.txt", "w");

    int i;
    matrix_pointer temp, head = node->right;
    /* matrix dimensions */
    fprintf(outputFile, "%d %d %d\n", node->u.entry.row, node->u.entry.col, node->u.entry.value);
    for (i = 0; i < node->u.entry.row; i++)
    {
        // 해당 행에 대해서 right 로 이동하면서 쭉 출력
        for (temp = head->right; temp != head; temp = temp->right) {
            fprintf(outputFile, "%d %d %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
        }
        
        // 헤드를 다음 헤드 노드로 이동
        head = head->u.next;
    }
}

void merase(matrix_pointer *node)
{ /* erase the matrix, return the nodes to the heap */
    matrix_pointer x, y, head = (*node)->right;
    int i;
    /* free the entry and header nodes by row */
    for (i = 0; i < (*node)->u.entry.row; i++)
    {
        y = head->right;
        while (y != head)
        {
            x = y;
            y = y->right;
            free(x);
        }
        x = head;
        head = head->u.next;
        free(x);
    }
    /* free remaining head nodes */
    y = head;
    while (y != *node)
    {
        x = y;
        y = y->u.next;
        free(x);
    }
    free(*node);
    *node = NULL;
}

int main(void)
{

    printf("a");
    matrix_pointer node;
    node = mread();

    printf("b");
    matrix_pointer nodeT = mtranspose(node);

    printf("c");
    mwrite(nodeT);

    // printf("%d\n", nodeT->right->u.next->right->u.entry.row);
    // printf("%d\n", nodeT->right->u.next->right->u.entry.col);
    // printf("%d\n", nodeT->right->u.next->right->u.entry.value);

    // printf("%d ", hdnode_t[1]->right->u.entry.row);
    // printf("%d ", hdnode_t[1]->right->u.entry.col);
    // printf("%d \n", hdnode_t[1]->right->u.entry.value);

    printf("d");
    return 0;
}