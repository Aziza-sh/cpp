#include <iostream>

struct Node {
  int data;
  Node *next;
  Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
private:
  Node *head;

public:
  LinkedList() : head(nullptr) {}

  ~LinkedList() {
    while (head) {
      Node *temp = head;
      head = head->next;
      delete temp;
    }
  }

  void addToBeginning(int val) {
    Node *newNode = new Node(val);
    newNode->next = head;
    head = newNode;
  }

  void addToEnd(int val) {
    Node *newNode = new Node(val);
    if (!head) {
      head = newNode;
      return;
    }
    Node *current = head;
    while (current->next) {
      current = current->next;
    }
    current->next = newNode;
  }

  bool deleteByValue(int val) {
    if (!head)
      return false;
    if (head->data == val) {
      Node *temp = head;
      head = head->next;
      delete temp;
      return true;
    }
    Node *current = head;
    while (current->next && current->next->data != val) {
      current = current->next;
    }
    if (current->next) {
      Node *temp = current->next;
      current->next = current->next->next;
      delete temp;
      return true;
    }
    return false;
  }

  Node *search(int val) {
    Node *current = head;
    while (current) {
      if (current->data == val)
        return current;
      current = current->next;
    }
    return nullptr;
  }

  void print() const {
    Node *current = head;
    while (current) {
      std::cout << current->data << " ";
      current = current->next;
    }
    std::cout << std::endl;
  }
};

int main() {
  LinkedList list;
  list.addToEnd(10);
  list.addToEnd(20);
  list.addToBeginning(5);
  list.addToEnd(30);
  std::cout << "Список: ";
  list.print();

  std::cout << "Удаляем 20: ";
  list.deleteByValue(20);
  list.print();

  int searchVal = 10;
  Node *found = list.search(searchVal);
  if (found)
    std::cout << "Найдено значение " << searchVal << std::endl;
  else
    std::cout << "Значение " << searchVal << " не найдено" << std::endl;

  return 0;
}