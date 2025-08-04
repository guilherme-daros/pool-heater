#include "Node.cpp"
#include <sstream>

class List {

  Node *head;

public:
  List();
  ~List();
  void insertBeforeFirst(std::string);
  void insertAfterLast(std::string dat);
  std::string listAll();
};

List::List() { head = 0; }

List::~List() {
  Node *cursor = head;
  while (head) {
    cursor = cursor->getNext();
    delete head;
    head = cursor;
  }
  head = 0;
}

void List::insertBeforeFirst(std::string dat) { head = new Node(dat, head); }

void List::insertAfterLast(std::string dat) {
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

std::string List::listAll() {
  Node *p = head;
  Node *q = head;
  std::ostringstream buffer;

  if (head == 0) {
    return 0;
  } else {
    while (q != 0) {
      p = q;
      buffer << p->getVal() << ";";
      q = p->getNext();
    }
  }
  return buffer.str();
}
