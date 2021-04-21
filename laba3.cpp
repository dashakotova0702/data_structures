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
        int get_count_adj_vertex () {
                int count = 0;
                while (first) {
                        count++;
                        first = first->next;
                }
                return count;
        }
        int get_adjacent_vertex (Marks* m) {
                while (first) {
                        int a = 0, b = 0;
                        for (int i = 0; i < m->size; i++) {
                                if (first->vertex == m->processed_vertex[i])
                                        a++;
                        }
                        if (a == 0) {
                                for (int i = 0; i < m->size; i++) {
                                        if (first->vertex == m->processed_vertex_marks[i])
                                                b++;
                                }
                                if (b == 0) {
                                        m->processed_vertex_marks[m->id_processed_vertex_marks] = first->vertex;
                                        m->id_processed_vertex_marks++;
                                        return first->vertex;
                                }
                                else
                                        first = first->next;
                        }
                        else
                                first = first->next;
                }
        }
        int get_weight_vertex (int vertex, int adjacent_vertex) {
                while (first) {
                        if (first->vertex == adjacent_vertex)
                                return first->weight;
                }
        }
};

class Strategy {
public:
virtual void init (ifstream &f) = 0;
virtual int adjacent_vertex (Marks* m, int vertex);
virtual int get_count_adj_vertex(int vertex);
virtual int weight_vertex (int vertex, int adjacent_vertex);
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

int adjacent_vertex (Marks* m, int vertex) {
        return 0;
}
int get_count_adj_vertex(int vertex) {
        return 0;
}
int weight_vertex (int adjacent_vertex) {
        return 0;
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

int adjacent_vertex (Marks* m, int vertex) {
        return arr_lsts[vertex].get_adjacent_vertex(m);
}

int get_count_adj_vertex(int vertex) {
        return arr_lsts[vertex].get_count_adj_vertex();
}
int weight_vertex (int vertex, int adjacent_vertex) {
        return arr_lsts[vertex].get_weight_vertex(vertex, adjacent_vertex);
}
};

class Graph {
private:
Strategy* strtg_;

public:

Graph (Strategy *strtg = nullptr) : strtg_(strtg) {
}

void init (ifstream &f) {
        this->strtg_->init(f);
}

int get_count_adj_vertex(int vertex) {
        return this->strtg_->get_count_adj_vertex(vertex);
}

int adjacent_vertex (Marks* m, int vertex) {
        return this->strtg_->adjacent_vertex(m, vertex);
}

int weight_vertex (int vertex, int adjacent_vertex) {
        return this->strtg_->weight_vertex(vertex, adjacent_vertex);
}

~Graph () {
        delete this->strtg_;
}
};
class Marks {
public:
int* marks;
int* processed_vertex;
int* processed_vertex_marks;
int size;
int id_processed_vertex_marks = 0;
};

void dkstr (Graph* g, Marks* m, ifstream &f, int start, int end) {
        int vertex = start, id_processed_vertex = 0;
        string str;
        getline(f, str);
        getline(f, str);
        m->size = stoi(str);
        m->marks = new int [m->size];
        m->processed_vertex = new int [m->size];
        m->processed_vertex_marks = new int [m->size];
        for (int i = 0; i < m->size; i++) {
                if (i == start)
                        m->marks[i] = 0;
                else
                        m->marks[i] = 100000;
        }
        int count_adj_vertex = g->get_count_adj_vertex(vertex);
        for (int i = 0; i < count_adj_vertex; i++) {
                int adjacent_vertex = g->adjacent_vertex(m, vertex);
                int weight = g->weight_vertex(vertex, adjacent_vertex);
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
                Marks m;
                if (type == "matrix")
                        g = new Graph(new Matrix);
                else
                        g = new Graph(new ArrayLists);
                g->init(fout);
                dkstr(g, &m, fout, start, end);
                return 0;
        }
        else {
                return -1;
        }
}
