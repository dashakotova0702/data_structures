#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

class Marks {
public:
int* marks;
int* processed_vertex;
int* processed_vertex_marks;
int size;
int id_processed_vertex_marks = 0;
};

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
                Node* node = first;
                while (node) {
                        cout << node->vertex << " " << node->weight << " ";
                        node = node->next;
                        cout << endl;
                }
        }
        int get_count_adj_vertex () {
                int count = 0;
                Node* node = first;
                while (node) {
                        count++;
                        node = node->next;
                }
                return count;
        }
        int get_adjacent_vertex (Marks* m) {
                Node* node = first;
                while (node) {
                        int a = 0, b = 0;
                        for (int i = 0; i < m->size; i++) {
                                if (node->vertex == m->processed_vertex[i])
                                        a++;
                        }
                        if (a == 0) {
                                for (int i = 0; i < m->size; i++) {
                                        if (node->vertex == m->processed_vertex_marks[i])
                                                b++;
                                }
                                if (b == 0) {
                                        m->processed_vertex_marks[m->id_processed_vertex_marks] = node->vertex;
                                        m->id_processed_vertex_marks++;
                                        return node->vertex;
                                }
                                else {
                                        a = 0, b = 0;
                                        node = node->next;
                                }
                        }
                        else {
                                a = 0, b = 0;
                                node = node->next;
                        }

                }
                return 0;
        }
        int get_weight_vertex (int adjacent_vertex) {
                Node* node = first;
                while (node) {
                        if (node->vertex == adjacent_vertex) {
                                return node->weight;
                        }
                        node = node->next;
                }
                return 0;
        }
};

class Strategy {
public:
virtual void init (ifstream &f) = 0;
virtual int adjacent_vertex (Marks* m, int vertex) = 0;
virtual int get_count_adj_vertex(Marks* m, int vertex) = 0;
virtual int weight_vertex (Marks* m, int vertex, int adjacent_vertex) = 0;
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
}
int adjacent_vertex (Marks* m, int vertex) {
        int a = 0, b = 0;
        for (int i = 0; i < m->size; i++) {
                if (matrix[vertex][i] != 0) {
                        for (int j = 0; j < m->size; j++) {
                                if (i == m->processed_vertex[j]) {
                                        a++;
                                }
                        }
                        if (a == 0) {
                                for (int k = 0; k < m->size; k++) {
                                        if (i == m->processed_vertex_marks[k]) {
                                                b++;
                                        }
                                }
                                if (b == 0) {
                                        m->processed_vertex_marks[m->id_processed_vertex_marks] = i;
                                        m->id_processed_vertex_marks++;
                                        return i;
                                }
                        }
                }
                a = 0, b = 0;
        }
        return 0;
}
int get_count_adj_vertex (Marks* m, int vertex) {
        int count = 0;
        for (int i = 0; i < m->size; i++) {
                if (matrix[vertex][i] != 0)
                        count++;
        }
        return count;
}
int weight_vertex (Marks* m, int vertex, int adjacent_vertex) {
        return matrix[vertex][adjacent_vertex];
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
        arr_lsts = new List[size];
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
}
int adjacent_vertex (Marks* m, int vertex) {
        return arr_lsts[vertex].get_adjacent_vertex(m);
}
int get_count_adj_vertex(Marks* m, int vertex) {
        return arr_lsts[vertex].get_count_adj_vertex();
}
int weight_vertex (Marks* m, int vertex, int adjacent_vertex) {
        return arr_lsts[vertex].get_weight_vertex(adjacent_vertex);
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
int get_count_adj_vertex(Marks* m, int vertex) {
        return this->strtg_->get_count_adj_vertex(m, vertex);
}
int adjacent_vertex (Marks* m, int vertex) {
        return this->strtg_->adjacent_vertex(m, vertex);
}
int weight_vertex (Marks* m, int vertex, int adjacent_vertex) {
        return this->strtg_->weight_vertex(m, vertex, adjacent_vertex);
}
~Graph () {
        delete this->strtg_;
}
};

void dkstr (Graph* g, Marks* m, ifstream &f, int start, int end) {
        f.clear();
        f.seekg(0);
        int vertex, id_processed_vertex = 0;
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

        while (id_processed_vertex != m->size) {
                int min = 100000, min_id = 0;
                for (int i = 0; i < m->size; i++) {
                        if (m->marks[i] < min) {
                                int a = 0;
                                for (int j = 0; j < m->size; j++) {
                                        if (i == m->processed_vertex[j])
                                                a++;
                                }
                                if (a == 0) {
                                        min = m->marks[i];
                                        min_id = i;
                                }
                        }
                }
                vertex = min_id;
                int count_adj_vertex = g->get_count_adj_vertex(m, vertex);
                for (int i = 0; i < count_adj_vertex; i++) {
                        int adjacent_vertex = g->adjacent_vertex(m, vertex);
                        int weight_adjacent_vertex = g->weight_vertex(m, vertex, adjacent_vertex);
                        int mark = m->marks[vertex] + weight_adjacent_vertex;
                        if (mark < m->marks[adjacent_vertex])
                                m->marks[adjacent_vertex] = mark;
                }
                m->id_processed_vertex_marks = 0;
                for (int i = 0; i < m->size; i++) {
                        m->processed_vertex_marks[i] = -1;
                }
                m->processed_vertex[id_processed_vertex] = vertex;
                id_processed_vertex++;
        }
        for (int i = 0; i < m->size; i++)
                cout << i << " = " << m->marks[i] << endl;
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
