#include <iostream>
#include <queue>

using namespace std;

struct Node {
  int data;
  Node *left;
  Node *right;
  Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BST {
private:
  Node *root;

  Node *insert(Node *node, int val) {
    if (!node)
      return new Node(val);
    if (val < node->data)
      node->left = insert(node->left, val);
    else if (val > node->data)
      node->right = insert(node->right, val);
    return node;
  }

  Node *findMin(Node *node) {
    while (node && node->left)
      node = node->left;
    return node;
  }

  Node *remove(Node *node, int val) {
    if (!node)
      return nullptr;
    if (val < node->data)
      node->left = remove(node->left, val);
    else if (val > node->data)
      node->right = remove(node->right, val);
    else {

      if (!node->left) {
        Node *rightChild = node->right;
        delete node;
        return rightChild;
      }
      if (!node->right) {
        Node *leftChild = node->left;
        delete node;
        return leftChild;
      }

      Node *minNode = findMin(node->right);
      node->data = minNode->data;
      node->right = remove(node->right, minNode->data);
    }
    return node;
  }

  void inorder(Node *node) {
    if (!node)
      return;
    inorder(node->left);
    cout << node->data << " ";
    inorder(node->right);
  }

  void preorder(Node *node) {
    if (!node)
      return;
    cout << node->data << " ";
    preorder(node->left);
    preorder(node->right);
  }

  void postorder(Node *node) {
    if (!node)
      return;
    postorder(node->left);
    postorder(node->right);
    cout << node->data << " ";
  }

public:
  BST() : root(nullptr) {}

  void insert(int val) { root = insert(root, val); }

  void remove(int val) { root = remove(root, val); }

  void inorderTraversal() {
    cout << "Inorder: ";
    inorder(root);
    cout << "\n";
  }

  void preorderTraversal() {
    cout << "Preorder: ";
    preorder(root);
    cout << "\n";
  }

  void postorderTraversal() {
    cout << "Postorder: ";
    postorder(root);
    cout << "\n";
  }
};

int main() {
  BST tree;
  tree.insert(5);
  tree.insert(3);
  tree.insert(7);
  tree.insert(2);
  tree.insert(4);
  tree.insert(6);
  tree.insert(8);

  tree.inorderTraversal();
  tree.preorderTraversal();
  tree.postorderTraversal();

  cout << "Удаляем 3\n";
  tree.remove(3);
  tree.inorderTraversal();

  cout << "Удаляем 5\n";
  tree.remove(5);
  tree.inorderTraversal();

  return 0;
}