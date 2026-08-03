#include <iostream>

using namespace std;

struct ListNode {
    int data;
    ListNode *next;
};

void printList(ListNode *front) {
    // TODO

    ListNode* ptr = front;

    while(1) {
        cout << ptr->data << ' ';
        ptr = ptr->next;
        
        if (ptr->next == nullptr) {
            cout << ptr->data << endl;
            break;
        }
    }
}

void deleteList(ListNode *front) {
    // TODO

    while(1) {
        ListNode *trash = front;
        front = front->next;
        delete trash;

        if (front->next == nullptr) {
            delete front;
            break;
        }
    }
}

void reversedList(ListNode *&front) {
    // TODO
    ListNode *prev = nullptr;
    ListNode *cur = front;
    ListNode *nx = front->next;

    while(nx != nullptr) {
        cur->next = prev;
        prev = cur;
        cur = nx;
        nx = cur->next;
    }
    cur->next = prev;
    front = cur;
}

int main() {
    ListNode* front = new ListNode();
    front->data = 42;
    front->next = new ListNode();
    front->next->data = -3;
    front->next->next = new ListNode();
    front->next->next->data = 17;
    front->next->next->next = nullptr;

    reversedList(front);
    cout << "reversed" << endl;
    printList(front);

    deleteList(front);
    
    return 0;
}