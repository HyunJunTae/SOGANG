#include <iostream>

using namespace std;

struct ListNode {
    int data;
    ListNode *next;
};

void printList(ListNode *front) {
    // TODO
    for (ListNode* ptr = front; ptr != nullptr; ptr = ptr->next) {
        cout << ptr->data << ' ';
    }
    
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

    ListNode *trash = front;
    front = front->next;
    delete trash;
    
    printList(front);

    return 0;
}