#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

struct List {
        pair <int, int> vrtx_wght;
        List *next;
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
void init (ifstream &f)
{
        cout << "lists" << endl;
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

int main (int argc, char const *argv[]) {
        string type;
        ifstream fout("graph.txt");
        getline(fout, type);
        Graph* g;
        if (type == "matrix")
                g = new Graph(new Matrix);
        else
                g = new Graph(new ArrayLists);
        g->init(fout);
        return 0;
}
