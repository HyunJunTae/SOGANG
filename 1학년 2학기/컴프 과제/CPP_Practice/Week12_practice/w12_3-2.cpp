#include <iostream>

using namespace std;

struct ListNode {
    int data;
    ListNode *next;
};

void printList(ListNode *front) {
    // TODO
    ListNode* ptr = front;

    while(ptr != nullptr) {
        cout << ptr->data << ' ';
        ptr = ptr->next;
    }
    cout << '\n';
}

void deleteList(ListNode *front) {
    // TODO

    while(front != nullptr) {
        ListNode *trash = front;
        front = front->next;
        delete trash;
    }
}

void reversedList(ListNode *&front) {
    // TODO
    ListNode *p = front;
    ListNode *q = nullptr;
    ListNode *r;
    // int count = 0;

    while(p != nullptr) {
        r = q;
        q = p;
        p = p->next;
        q->next = r;
        // count += 1;
    }

    // while문 첫 번째에서 기존 첫번째 next를 q->next = r 을 통해 nullptr로 만들어주니까 따로 마지막 노드 가서 nullptr로 만드는 작업 필요X.
    // r = q;
    // for(int i=0; i<count-1; i++) {
    //     r = r->next;
    // }
    // r->next = nullptr;
    
    front = q;
}

int main() {
    ListNode* front = new ListNode();
    front->data = 42;
    front->next = new ListNode();
    front->next->data = -3;
    front->next->next = new ListNode();
    front->next->next->data = 17;
    front->next->next->next = nullptr;

    printList(front);
    reversedList(front);
    cout << "reversed" << endl;
    printList(front);

    deleteList(front);
    
    return 0;
}