#include <iostream>
using namespace std;

template <typename T>
class Node {
private :
    T data;
    Node *next = NULL;
public :
    Node(T d) : data(d) {}
    T GetData () {
        return data;
    }
};

int main(void) {
    Node <int> *n1 = new Node <int> (1);
    auto n2 = new Node <double> (2.3);
    auto d = n2->GetData();
    cout << d << endl;
    return 0;
}