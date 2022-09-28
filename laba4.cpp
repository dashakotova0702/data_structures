#include <iostream>
#include <string>
#include <fstream>
#include <locale.h>
using namespace std;

class TreeNode {
public:
    char data;
    int weight;
    string binarycode = "";
    TreeNode* left;
    TreeNode* right;
    TreeNode() : left(NULL), right(NULL) {
    }
};

struct Node {
    char symbol;
    int stat;
    string binarycode;
    TreeNode* node;
    Node* next;
    Node(char _symbol, int _stat) : symbol(_symbol), stat(_stat), node(NULL), next(NULL) {
    }
};

struct List {
    Node* first;
    Node* last;
    List() : first(NULL), last(NULL) {
    }

    bool is_empty() {
        return (first == NULL);
    }
    void push_back(char symbol, int stat) {
        Node* node = new Node(symbol, stat);
        if (is_empty()) {
            first = node;
            last = node;
            return;
        }
        last->next = node;
        last = node;
    }
    void print() {
        if (is_empty()) return;
        Node* node = first;
        while (node) {
            if (node->node == NULL) {
                cout << "Symbol: " << node->symbol << " Statistic: " << node->stat << " Binary code: " << node->binarycode;
                node = node->next;
                cout << endl;
            }
            else {
                cout << "tree " << node->stat;
                node = node->next;
                cout << endl;
            }
        }
    }
    void stat_to_file(fstream& file) {
        Node* node = first;
        while (node) {
            file << node->symbol << " " << node->stat;
            node = node->next;
            file << endl;
        }
    }
    void sort() {
        Node* a, * b, * p, * h = NULL;
        for (Node* i = first; i != NULL; ) {
            a = i;
            i = i->next;
            b = h;
            for (p = NULL; (b != NULL) && (a->stat > b->stat); ) {
                p = b;
                b = b->next;
            }

            if (p == NULL) {
                a->next = h;
                h = a;
            }
            else {
                a->next = b;
                p->next = a;
            }
        }
        if (h != NULL)
            first = h;
    }
    void remove_first() {
        if (is_empty()) return;
        Node* p = first;
        first = p->next;
        delete p;
    }
};

class Data {
public:
    TreeNode* tree;
    List stat;

    void addNode(char data, int weight, TreeNode* node, TreeNode* node_next) {
        node->data = data;
        node->weight = weight;
        if (node_next != 0) {
            node->left = node_next->left;
            node->right = node_next->right;
        }
    }
    void connectNode(TreeNode* node1, TreeNode* node2, TreeNode* root) {
        root->data = 0;
        root->weight = node1->weight + node2->weight;
        root->left = node1;
        root->right = node2;
    }
    void print(TreeNode* root) {
        if (root != NULL) {
            cout << root->data << " = " << root->binarycode << endl;
            print(root->left);
            print(root->right);
        }
    }
    void binary(TreeNode* root, string bin) {
        if (root != NULL) {
            root->binarycode = bin;
            string bnr = root->binarycode;
            binary(root->left, bnr + "0");
            binary(root->right, bnr + "1");
        }
    }
    void search(TreeNode* root, Node* node) {
        if (root != NULL) {
            if (root->data == node->symbol) {
                for (int i = 1; i < root->binarycode.length(); i++) {
                    node->binarycode += root->binarycode[i];
                }
            }
            search(root->left, node);
            search(root->right, node);
        }
    }
};

void stat(Data* d, fstream& input_file, fstream& output_file, string prefix) {
    string text;
    getline(input_file, text);
    int count = 1;
    cout << "String: " << text << endl;
    for (int i = 0; i < text.length(); i++) {
        if (d->stat.is_empty()) {
            d->stat.push_back(text[i], 1);
        }
        else {
            int a = 0;
            Node* node = d->stat.first;
            while (node) {
                if (text[i] == node->symbol) {
                    node->stat++;
                    a = 1;
                }
                node = node->next;
            }
            if (a != 1) {
                d->stat.push_back(text[i], 1);
                count++;
            }
            a = 0;
        }
    }
    output_file << prefix << endl;
    output_file << text.length() << endl;
    cout << "Size of original text: " << text.length() << " byte" << endl;
    output_file << count << endl;
    d->stat.sort();
    d->stat.sort();
    d->stat.stat_to_file(output_file);
}
void makeTree(Data* d, fstream& output_file, string prefix) {
    List list;
    string line;
    output_file.clear();
    output_file.seekg(0);
    getline(output_file, line);
    if (prefix != line)
        return;
    getline(output_file, line);
    getline(output_file, line);
    while (getline(output_file, line)) {
        if (line.length() < 10) {
            string a;
            for (int i = 2; i < line.length(); i++) {
                a += line[i];
            }
            list.push_back(line[0], stoi(a));
        }  
    }
    Node* node = list.first;
    if (!d->stat.first) {
        while (node) {
            d->stat.push_back(node->symbol, node->stat);
            node = node->next;
        }
    }
    node = list.first;
    int count = -1;
    while (node) {
        count++;
        node = node->next;
    }
    node = list.first;
    count = count * 3;
    TreeNode* trees = new TreeNode [count];
    for (int i = 0; i < count; i++) {
        d->addNode(node->symbol, node->stat, &trees[i], node->node);
        node = node->next;
        i++;
        d->addNode(node->symbol, node->stat, &trees[i], node->node);
        i++;
        d->connectNode(&trees[i - 2], &trees[i - 1], &trees[i]);
        node->symbol = 0;
        node->stat = trees[i].weight;
        node->node = &trees[i];
        list.remove_first();
        list.sort();
        list.sort();
        node = list.first;
    }
    d->tree = &trees[count - 1];
}
void makeBinaryCode(Data* d) {
    TreeNode* tr = new TreeNode;
    tr = d->tree;
    d->binary(tr, "0");
    tr = d->tree;
    Node* node = d->stat.first;
    while (node) {
        d->search(tr, node);
        node = node->next;
    }
    d->stat.print();
}
void compress(Data* d, fstream& input_file, fstream& output_file) {
    Node* node = d->stat.first;
    string text;
    input_file.clear();
    input_file.seekg(0);
    getline(input_file, text);
    output_file.clear();
    output_file.seekg(0, ios::end);
    cout << "Compressed data: ";
    int size = 0;
    for (int i = 0; i < text.length(); i++) {
        while (node) {
            if (node->symbol == text[i]) {
                    size += node->binarycode.length();
                    cout << node->binarycode;
                    output_file << node->binarycode;
            }
            node = node->next;
        }
        node = d->stat.first;
    }
    cout << endl << "Size of compressed text: " << size / 8 << " byte" << endl;
}
void decompress(Data* d, fstream& input_file, fstream& output_file) {
    Node* node = d->stat.first;
    string binary, b, str;
    output_file.clear();
    output_file.seekg(0);
    getline(output_file, binary);
    getline(output_file, binary);
    getline(output_file, binary);
    do {
        getline(output_file, binary);
    }
    while (binary[1] == ' ');
    for (int i = 0; i < binary.length(); i++) {
        b += binary[i];
        while (node) {
            if (node->binarycode == b) {
                input_file << node->symbol;
                for (int j = i + 1; j < binary.length(); j++) {
                    str += binary[j];
                }
                binary = str;
                str.clear();
                b.clear();
                i = -1;
            }
            node = node->next;
        }
        node = d->stat.first;
    }
}

int main(int argc, char const* argv[]) {
    setlocale(LC_ALL, "Russian");
    if (argc == 5) {
        Data d;
        if (string(argv[1]) == "compress") {
            fstream fout;
            fout.open(string(argv[2]));
            fstream fin;
            fin.open(string(argv[3]), ios_base::in | ios_base::out | ios_base::trunc);
            stat(&d, fout, fin, string(argv[4]));
            makeTree(&d, fin, string(argv[4]));
            makeBinaryCode(&d);
            compress(&d, fout, fin);
        }
        if (string(argv[1]) == "decompress") {
            fstream fout;
            fout.open(string(argv[2]), ios_base::in);
            fstream fin;
            fin.open(string(argv[3]), ios_base::out | ios_base::trunc);
            makeTree(&d, fout, string(argv[4]));
            makeBinaryCode(&d);
            decompress(&d, fin, fout);
        }
        return 0;
    }
    else {
        return -1;
    }
}
