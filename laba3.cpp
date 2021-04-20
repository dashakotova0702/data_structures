#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

struct Node {
        int vertex, weight;
        Node* next;
        Node (int _vertex, int _weight) : vertex(_vertex), weight(_weight), next(NULL) {
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

        void push_back (int vertex, int weight) {
                Node* node = new Node(vertex, weight);
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
                while (first) {
                        cout << first->vertex << " " << first->weight << " ";
                        first = first->next;
                        cout << endl;
                }
        }
};

class Strategy {
public:
virtual void init (ifstream &f) = 0;
virtual ~Strategy () {
}
};

class Matrix : public Strategy {
private:
int** matrix;

public:
void init (ifstream &f) {
        int size;
        string str;
        smatch m;
        getline(f, str);
        size = stoi(str);
        matrix = new int* [size];
        for (int i = 0; i < size; i++) {
                matrix[i] = new int [size];
        }
        for (int j = 0; j < size; j++) {
                for (int i = 0; i < size; i++) {
                        matrix[j][i] = 0;
                }
        }
        while (getline(f, str)) {
                int start, end;
                regex r1("\\:"), r2("\\("), r3("\\,"), r4("\\)");
                regex_search(str, m, r1);
                start = stoi(m.prefix());
                while (m.suffix().length() != 0) {
                        regex_search(str, m, r2);
                        str = m.suffix();
                        regex_search(str, m, r3);
                        end = stoi(m.prefix());
                        str = m.suffix();
                        regex_search(str, m, r4);
                        matrix[start][end] = stoi(m.prefix());
                        str = m.suffix();
                }
                str.clear();
        }
        for (int j = 0; j < size; j++) {
                for (int i = 0; i < size; i++) {
                        cout << matrix[j][i];
                }
                cout << endl;
        }
}
};

class ArrayLists : public Strategy {
private:
List* arr_lsts;

public:
void init (ifstream &f) {
        int size;
        string str;
        smatch m;
        getline(f, str);
        size = stoi(str);
        arr_lsts = new List[5];
        while (getline(f, str)) {
                int index, vertex, weight;
                regex r1("\\:"), r2("\\("), r3("\\,"), r4("\\)");
                regex_search(str, m, r1);
                index = stoi(m.prefix());
                while (m.suffix().length() != 0) {
                        regex_search(str, m, r2);
                        str = m.suffix();
                        regex_search(str, m, r3);
                        vertex = stoi(m.prefix());
                        str = m.suffix();
                        regex_search(str, m, r4);
                        weight = stoi(m.prefix());
                        arr_lsts[index].push_back(vertex, weight);
                        str = m.suffix();
                }
                str.clear();
        }
        for (int i = 0; i < size; i++)
                arr_lsts[i].print();
}
};

class Graph {
private:
Strategy *strtg_;

public:
Graph (Strategy *strtg = nullptr) : strtg_(strtg) {
}

void init (ifstream &f) {
        this->strtg_->init(f);
}

~Graph () {
        delete this->strtg_;
}
};

void dkstr (Graph* g, ifstream &f, int start, int end) {
        int size;
        string str;
        getline(f, str);
        getline(f, str);
        size = stoi(str);
        int marks[size];
        for (int i = 0; i < size; i++) {
                if (i == start)
                        marks[i] = 0;
                else
                        marks[i] = 100000;
        }

}

int main (int argc, char const *argv[]) {
        if (argc == 3) {
                int start = atoi(argv[1]);
                int end = atoi(argv[2]);
                string type;
                ifstream fout("graph.txt");
                getline(fout, type);
                Graph* g;
                if (type == "matrix")
                        g = new Graph(new Matrix);
                else
                        g = new Graph(new ArrayLists);
                g->init(fout);
                dkstr(g, fout, start, end);
                return 0;
        }
        else {
                return -1;
        }
}
