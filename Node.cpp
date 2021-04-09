#include <iostream>
#include <string>

using namespace std;

class Node {
    string val;
    Node* next;

public:
    Node(string dat, Node* nxt);
    string getVal();
    Node* getNext();
    void setVal(int dat);
    void setNext(Node* nxt);
};

Node::Node(string dat, Node* nxt) {
    val = dat;
    next = nxt;
}

string Node::getVal() {
    return val;
}

Node* Node::getNext() {
    return next;
}

void Node::setVal(int dat) {
    val = dat;
}

void Node::setNext(Node* nxt) {
    next = nxt;
}
