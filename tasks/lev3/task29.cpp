#include <iostream>
#include <stdexcept>

class ArrayStack {
private:
  int *arr;
  int capacity;
  int topIndex;

public:
  ArrayStack(int size = 100) : capacity(size), topIndex(-1) {
    arr = new int[capacity];
  }

  ~ArrayStack() { delete[] arr; }

  void push(int val) {
    if (topIndex + 1 >= capacity) {
      throw std::overflow_error("Стек переполнен");
    }
    arr[++topIndex] = val;
  }

  int pop() {
    if (isEmpty()) {
      throw std::underflow_error("Стек пуст");
    }
    return arr[topIndex--];
  }

  int top() const {
    if (isEmpty()) {
      throw std::underflow_error("Стек пуст");
    }
    return arr[topIndex];
  }

  bool isEmpty() const { return topIndex == -1; }
};

struct Node {
  int data;
  Node *next;
  Node(int val) : data(val), next(nullptr) {}
};

class ListStack {
private:
  Node *topNode;

public:
  ListStack() : topNode(nullptr) {}

  ~ListStack() {
    while (topNode) {
      Node *temp = topNode;
      topNode = topNode->next;
      delete temp;
    }
  }

  void push(int val) {
    Node *newNode = new Node(val);
    newNode->next = topNode;
    topNode = newNode;
  }

  int pop() {
    if (isEmpty()) {
      throw std::underflow_error("Стек пуст");
    }
    int val = topNode->data;
    Node *temp = topNode;
    topNode = topNode->next;
    delete temp;
    return val;
  }

  int top() const {
    if (isEmpty()) {
      throw std::underflow_error("Стек пуст");
    }
    return topNode->data;
  }

  bool isEmpty() const { return topNode == nullptr; }
};

void testStack(const std::string &name, auto &stack) {
  std::cout << "Тестирование " << name << ":\n";
  stack.push(10);
  stack.push(20);
  stack.push(30);
  std::cout << "Верхний элемент: " << stack.top() << std::endl;
  std::cout << "Извлекаем: " << stack.pop() << std::endl;
  std::cout << "Извлекаем: " << stack.pop() << std::endl;
  std::cout << "Пуст? " << (stack.isEmpty() ? "да" : "нет") << std::endl;
  stack.push(40);
  std::cout << "Верхний элемент: " << stack.top() << std::endl;
  std::cout << "Извлекаем: " << stack.pop() << std::endl;
  std::cout << "Извлекаем: " << stack.pop() << std::endl;
  std::cout << "Пуст? " << (stack.isEmpty() ? "да" : "нет") << std::endl;
  std::cout << std::endl;
}

int main() {
  ArrayStack arrayStack(10);
  testStack("стек на массиве", arrayStack);

  ListStack listStack;
  testStack("стек на списке", listStack);

  std::cout << "Сравнение:\n"
            << "Стек на массиве: фиксированный размер, быстрый доступ, но "
               "возможен overflow.\n"
            << "Стек на списке: динамический размер, требует дополнительной "
               "памяти на указатели.\n";
  return 0;
}