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
};

struct Node {
        char symbol;
        int stat;
        Node* next;
        Node (char _symbol, int _stat) : symbol(_symbol), stat(_stat), next(NULL) {
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
                        cout << node->symbol << " " << node->stat;
                        node = node->next;
                        cout << endl;
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
};

class Data {
public:
TreeNode* tree;
List stat;
};

void levelOrderPrint (TreeNode* root, List* list) {
        if (root == NULL) {
                return;
        }
        queue <TreeNode*> q;
        q.push(root);

        while (!q.empty() ) {
                TreeNode* temp = q.front();
                q.pop();
                cout << temp->data << " ";

                if ( temp->left != NULL )
                        q.push(temp->left);

                if ( temp->right != NULL )
                        q.push(temp->right);
        }
}
void preorderPrint (TreeNode* root) {
        if (root == NULL) {
                return;
        }
        cout << root->data << " ";
        preorderPrint(root->left);
        preorderPrint(root->right);
}
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
        d->stat.print();
        d->stat.stat_to_file(output_file);
}
void make_tree(Data* d, TreeNode* root) {
        levelOrderPrint (root, list);
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
                }
                if (string(argv[1]) == "decompress") {

                }
                return 0;
        }
        else {
                return -1;
        }
}
