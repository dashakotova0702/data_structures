#include <iostream>
#include <fstream>
#include <list>
#include <queue>
using namespace std;

class TreeNode {
public:
char data;
int weight;
TreeNode *left;
TreeNode *right;
TreeNode() : left(NULL), right(NULL) {
}
};

struct Node {
        char symbol;
        int stat;
        TreeNode* node;
        Node* next;
        Node (char _symbol, int _stat) : symbol(_symbol), stat(_stat), node(NULL), next(NULL) {
        }
};

struct List {
        Node* first;
        Node* last;
        List() : first(NULL), last(NULL) {
        }

        bool is_empty () {
                return (first == NULL);
        }
        void push_back (char symbol, int stat) {
                Node* node = new Node(symbol, stat);
                if (is_empty()) {
                        first = node;
                        last = node;
                        return;
                }
                last->next = node;
                last = node;
        }
        void print () {
                if (is_empty()) return;
                Node* node = first;
                while (node) {
                        if (node->node == NULL) {
                                cout << node->symbol << " " << node->stat;
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
                Node* a, *b, *p, *h = NULL;
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
                                h       = a;
                        } else {
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

void addNode (char data, int weight, TreeNode* node, TreeNode* node_next) {
        node->data = data;
        node->weight = weight;
        if (node_next != 0) {
                node->left = node_next->left;
                node->right = node_next->right;
        }
}
void connectNode (TreeNode* node1, TreeNode* node2, TreeNode* root) {
        root->data = 0;
        root->weight = node1->weight + node2->weight;
        root->left = node1;
        root->right = node2;
}
void print (TreeNode* root) {
        if (root != NULL) {
                cout << root->data << "&" << root->weight << endl;
                print(root->left);
                print(root->right);
        }
}
};

void stat (Data* d, fstream& input_file, fstream& output_file, string prefix) {
        string text;
        getline(input_file, text);
        int count = 1;
        cout << text << endl;
        for (int i = 0; i < text.length(); i++) {
                if (d->stat.is_empty()) {
                        d->stat.push_back(text[i], 1);
                }
                else {
                        int a;
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
        output_file << count << endl;
        d->stat.sort();
        d->stat.sort();
        d->stat.print();
        cout << endl;
        d->stat.stat_to_file(output_file);
}
void makeTree (Data* d) {
        Node* node = d->stat.first;
        int count = -1;
        while (node) {
                count++;
                node = node->next;
        }
        node = d->stat.first;
        TreeNode links[count];
        TreeNode tree1;
        TreeNode tree2;
        TreeNode root;
        for (int i = 0; i < count; i++) {
                if (node->node != NULL)
                        d->addNode(node->symbol, node->stat,  &tree1, node->node);
                else
                        d->addNode(node->symbol, node->stat, &tree1, 0);
                node = node->next;
                if (node->node != NULL)
                        d->addNode(node->symbol, node->stat, &tree2, node->node);
                else
                        d->addNode(node->symbol, node->stat, &tree2, 0);
                d->connectNode(&tree1, &tree2, &root);
                d->addNode(root.data, root.weight, &links[i], &root);
                node->symbol = 0;
                node->stat = links[i].weight;
                node->node = &links[i];
                d->print(&links[0]);
                d->print(&links[1]);
                d->stat.remove_first();
                d->stat.sort();
                d->stat.sort();
                d->stat.print();
                cout << endl;
                node = d->stat.first;
        }
}
int main (int argc, char const *argv[]) {
        if (argc == 5) {
                Data d;
                if (string(argv[1]) == "compress") {
                        fstream fout;
                        fout.open(string(argv[2]));
                        fstream fin;
                        fin.open(string(argv[3]), ios::app);
                        stat(&d, fout, fin, string(argv[4]));
                        makeTree(&d);
                }
                if (string(argv[1]) == "decompress") {

                }
                return 0;
        }
        else {
                return -1;
        }
}
