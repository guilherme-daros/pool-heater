
#include "Node.cpp"

class Queue {
public:
  Node *head;
  Queue();
  ~Queue();
  void push(string);
  string pop();
};

Queue::Queue() { head = 0; }

Queue::~Queue() {
  Node *cursor = head;
  while (head) {
    cursor = cursor->getNext();
    delete head;
    head = cursor;
  }
  head = 0;
}

void Queue::push(string dat) {
  Node *p = head;
  Node *q = head;

  if (head == 0)
    head = new Node(dat, head);
  else {
    while (q != 0) {
      p = q;
      q = p->getNext();
    }
    p->setNext(new Node(dat, 0));
  }
}

string Queue::pop() {
  string retval = "0";
  if (head != 0) {
    retval = head->getVal();
    Node *oldHead = head;
    head = head->getNext();
    delete oldHead;
  }
  return retval;
}
