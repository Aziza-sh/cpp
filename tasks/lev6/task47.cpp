#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <vector>
#include <bitset>
#include <string>

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(int f, Node *l, Node *r) : ch(0), freq(f), left(l), right(r) {}
    ~Node() { delete left; delete right; }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void buildCodes(Node* root, const std::string &code, std::map<char, std::string> &codes) {
    if (!root) return;
    if (root->ch) codes[root->ch] = code;
    buildCodes(root->left, code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

void compress(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) { std::cerr << "Не удалось открыть файл\n"; return; }

    std::map<char, int> freq;
    char ch;
    while (in.get(ch)) freq[ch]++;
    in.clear(); in.seekg(0);

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto &p : freq) pq.push(new Node(p.first, p.second));
    while (pq.size() > 1) {
        Node *l = pq.top(); pq.pop();
        Node *r = pq.top(); pq.pop();
        pq.push(new Node(l->freq + r->freq, l, r));
    }
    Node* root = pq.top();

    std::map<char, std::string> codes;
    buildCodes(root, "", codes);

    std::ofstream out(outputFile, std::ios::binary);
    if (!out) { std::cerr << "Не удалось создать выходной файл\n"; return; }


    out.put(static_cast<char>(freq.size()));
    for (auto &p : freq) {
        out.put(p.first);
        out.write(reinterpret_cast<char*>(&p.second), sizeof(p.second));
    }

    std::string bits;
    while (in.get(ch)) bits += codes[ch];
    int padding = (8 - bits.size() % 8) % 8;
    bits += std::string(padding, '0');
    out.put(static_cast<char>(padding));

    for (size_t i = 0; i < bits.size(); i += 8) {
        std::bitset<8> byte(bits.substr(i, 8));
        out.put(static_cast<char>(byte.to_ulong()));
    }

    std::cout << "Сжатие завершено.\n";
    delete root;
}

void decompress(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) { std::cerr << "Не удалось открыть файл\n"; return; }

    int unique = in.get();
    std::map<char, int> freq;
    for (int i = 0; i < unique; ++i) {
        char c = in.get();
        int f;
        in.read(reinterpret_cast<char*>(&f), sizeof(f));
        freq[c] = f;
    }

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto &p : freq) pq.push(new Node(p.first, p.second));
    while (pq.size() > 1) {
        Node *l = pq.top(); pq.pop();
        Node *r = pq.top(); pq.pop();
        pq.push(new Node(l->freq + r->freq, l, r));
    }
    Node* root = pq.top();

    int padding = in.get();
    std::string bits;
    char byte;
    while (in.get(byte)) {
        bits += std::bitset<8>(byte).to_string();
    }
    bits = bits.substr(0, bits.size() - padding);

    std::ofstream out(outputFile, std::ios::binary);
    Node* cur = root;
    for (char b : bits) {
        if (b == '0') cur = cur->left;
        else cur = cur->right;
        if (cur->ch) {
            out.put(cur->ch);
            cur = root;
        }
    }

    std::cout << "Распаковка завершена.\n";
    delete root;
}

int main() {
    int choice;
    std::string input, output;
    std::cout << "1. Сжать\n2. Распаковать\nВыберите действие: ";
    std::cin >> choice;
    std::cin.ignore();
    std::cout << "Входной файл: ";
    std::getline(std::cin, input);
    std::cout << "Выходной файл: ";
    std::getline(std::cin, output);

    if (choice == 1) compress(input, output);
    else if (choice == 2) decompress(input, output);
    else std::cerr << "Неверный выбор\n";

    return 0;
}