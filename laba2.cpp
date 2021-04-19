#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <string>
using namespace std;

class Math {
public:
string math_exp;
string postf_not;
queue <string> q;
stack <string> s1;
stack <double> s2;
};

string initialization (string exp) {
        string str1, str2, str3, init;
        if ((int)exp[0] > 96 && (int)exp[0] < 123 && exp[1] == '=') {
                ofstream fin("data.txt", ios::app);
                fin << exp;
                fin << endl;
                fin.close();
                return "init";
        }
        else {
                ifstream fout("data.txt");
                for (int i = 0; i < exp.length(); i++) {
                        if ((int)exp[i] > 96 && (int)exp[i] < 123 && exp[i+1] != '=') {
                                init.clear();
                                str1.clear();
                                str2.clear();
                                str3.clear();
                                fout.clear();
                                fout.seekg(0);
                                for (int n = 0; n < i; n++) {
                                        str1 += exp[n];
                                }
                                if (fout.is_open()) {
                                        while (fout >> init) {
                                                if (init[0] == exp[i]) {
                                                        for (int k = i+1; k < exp.length(); k++) {
                                                                str3 += exp[k];
                                                        }
                                                        for (int j = 2; j < init.length(); j++) {
                                                                str2 += init[j];
                                                        }
                                                        exp.clear();
                                                        exp = str1 + str2 + str3;
                                                }
                                        }
                                }
                        }
                }
                fout.close();
        }

        return exp;
}

void to_postfix_notation (Math* math) {
        cout << "Infix notation: " << math->math_exp << endl;
        string str;
        int priority1 = 0, priority2 = 0;
        for (int i = 0; i < math->math_exp.length(); i++) {
                if (math->math_exp[i] == '(') {
                        str.clear();
                        str += math->math_exp[i];
                        math->s1.push(str);
                }
                if (((int)(math->math_exp[i]) > 47 && (int)(math->math_exp[i]) < 58) || (i != 0 && math->math_exp[i] == '-' && ((int)(math->math_exp[i-1]) < 48 || (int)(math->math_exp[i-1]) > 57)) || (i == 0 && math->math_exp[i] == '-')) {
                        str.clear();
                        if (math->math_exp[i] == '-') {
                                str += math->math_exp[i];
                                i++;
                        }
                        if (math->math_exp[i+1] == '.' || ((int)(math->math_exp[i+1]) > 47 && (int)(math->math_exp[i+1]) < 58)) {
                                str += math->math_exp[i];
                                i++;
                                while (math->math_exp[i] == '.' || ((int)(math->math_exp[i]) > 47 && (int)(math->math_exp[i]) < 58)) {
                                        str += math->math_exp[i];
                                        i++;
                                }
                                math->q.push(str);
                        }
                        else {
                                str += math->math_exp[i];
                                math->q.push(str);
                        }
                }
                if (math->math_exp[i] == '+' || math->math_exp[i] == '-' || math->math_exp[i] == '*' || math->math_exp[i] == '/') {
                        if (math->math_exp[i] == '+' || math->math_exp[i] == '-')
                                priority1 = 2;
                        if (math->math_exp[i] == '*' || math->math_exp[i] == '/')
                                priority1 = 3;
                        if (math->s1.empty()) {
                                str.clear();
                                str += math->math_exp[i];
                                math->s1.push(str);
                        }
                        else {
                                do {
                                        if (!math->s1.empty()) {
                                                if (math->s1.top() == "(")
                                                        priority2 = 1;
                                                if (math->s1.top() == "+" || math->s1.top() == "-")
                                                        priority2 = 2;
                                                if (math->s1.top() == "*" || math->s1.top() == "/")
                                                        priority2 = 3;
                                        }
                                        else
                                                priority2 = 0;
                                        if (priority2 >= priority1) {
                                                math->q.push(math->s1.top());
                                                math->s1.pop();
                                        }
                                }
                                while (priority2 >= priority1);
                                str.clear();
                                str += math->math_exp[i];
                                math->s1.push(str);
                        }
                }
                if (math->math_exp[i] == ')') {
                        while (math->s1.top() != "(") {
                                math->q.push(math->s1.top());
                                math->s1.pop();
                        }
                        math->s1.pop();
                }
        }
        while (!math->s1.empty()) {
                math->q.push(math->s1.top());
                math->s1.pop();
        }
        while (!math->q.empty()) {
                math->postf_not += math->q.front();
                math->postf_not += ' ';
                math->q.pop();
        }
        cout << "Postfix notation: " << math->postf_not << endl;
}

void stack_machine (Math* math) {
        string s_number;
        double d_number1, d_number2;
        bool error = false;
        for (int i = 0; i < math->postf_not.length(); i++) {
                if (math->postf_not[i] > 96 && math->postf_not[i] < 123) {
                        cout << "Error" << endl;
                        error = true;
                        break;
                }
        }
        if (!error) {
                for (int i = 0; i < math->postf_not.length(); i++) {
                        if ((math->postf_not[i] == '-' && math->postf_not[i+1] == ' ') || math->postf_not[i] == '+' || math->postf_not[i] == '*' || math->postf_not[i] == '/') {
                                d_number1 = math->s2.top();
                                math->s2.pop();
                                d_number2 = math->s2.top();
                                math->s2.pop();
                                if (math->postf_not[i] == '-')
                                        math->s2.push(d_number2-d_number1);
                                if (math->postf_not[i] == '+')
                                        math->s2.push(d_number2+d_number1);
                                if (math->postf_not[i] == '*')
                                        math->s2.push(d_number2*d_number1);
                                if (math->postf_not[i] == '/')
                                        math->s2.push(d_number2/d_number1);
                                i++;
                        }
                        else {
                                s_number.clear();
                                while (math->postf_not[i] != ' ') {
                                        s_number += math->postf_not[i];
                                        i++;
                                }
                                d_number1 = stod(s_number);
                                math->s2.push(d_number1);
                        }
                }
                cout << "Result: " << math->s2.top() << endl;
        }
}

int main (int argc, char const *argv[]) {
        if (argc == 2) {
                Math math;
                math.math_exp = initialization(string(argv[1]));
                if (math.math_exp == "init") {
                        cout << "Successful initialization" << endl;
                }
                else {
                        to_postfix_notation(&math);
                        stack_machine(&math);
                }
        }
        else {
                return -1;
        }
        return 0;
}
