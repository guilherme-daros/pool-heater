#include <string>
class Node {

     std::string val;
     Node* next;

public:

     Node(std::string dat, Node* nxt);
     std::string getVal();
     Node* getNext();
     void setVal(std::string dat);
     void setNext(Node* nxt);
};

Node::Node(std::string dat, Node* nxt) {
     val = dat;
     next = nxt;
}

std::string Node::getVal() {
     return val;
}

Node* Node::getNext() {
     return next;
}

void Node::setVal(std::string dat) {
     val = dat;
}

void Node::setNext(Node* nxt) {
     next = nxt;
}
