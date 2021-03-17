#include <iostream>
#include <string>
using namespace std;

struct Binary {
        int* integer;
        int count_int;
        int* fractional;
        int count_frac;
        int sign;
        int* exp;
        int* mantissa;
        int* byte;
};

struct Realis {
        string number;
        int integer;
        double fractional;
};

union {
        float f;
        double d;
        char cf[sizeof(float)];
        char cd[sizeof(double)];
} un;

void separation(Realis* realis, Binary* binary) {
        int i = 0;
        string integer, fractional = "0";
        if (realis->number[0] == '-') {
                binary->sign = 1;
                i++;
        }
        else
                binary->sign = 0;
        while (realis->number[i] != '.') {
                integer += realis->number[i];
                i++;
        }
        realis->integer = stoi(integer);
        for (; i < realis->number.length(); i++) {
                fractional += realis->number[i];
        }
        realis->fractional = stof(fractional);
        cout << "Entered number: " << realis->number << endl;
        cout << "Integer part: " << realis->integer << endl;
        cout << "Fractional part: " << realis->fractional << endl;
}

void to_binary(Realis* realis, Binary* binary, string type) {
        int count_integer = 0, integer = realis->integer, count_mantissa;
        if (type == "f") {
                count_mantissa = 23;
        }
        else {
                count_mantissa = 52;
        };
        if (realis->integer != 0) {
                while (integer != 0) {
                        integer = integer / 2;
                        count_integer++;
                }
                binary->integer = new int[count_integer];
                integer = realis->integer;
                for (int j = count_integer - 1; j >= 0; j--) {
                        binary->integer[j] = integer % 2;
                        integer = integer / 2;
                }
        }
        else {
                count_integer = 1;
                binary->integer = new int[1];
                binary->integer[0] = 0;
        }
        int count_fractional = 0;
        double fractional = realis->fractional;
        for (int i = 0; i < count_mantissa; i++) {
                if (fractional != 1.0) {
                        fractional = fractional * 2;
                        if (fractional > 1.0) {
                                fractional--;
                        }
                        count_fractional++;
                }
        }
        binary->fractional = new int[count_fractional];
        fractional = realis->fractional;
        for (int i = 0; i < count_fractional; i++) {
                fractional = fractional * 2;
                if (fractional >= 1.0) {
                        fractional--;
                        binary->fractional[i] = 1;
                }
                else
                        binary->fractional[i] = 0;
        }
        binary->count_int = count_integer;
        binary->count_frac = count_fractional;
        cout << "Entered number in binary: ";
        for (int i = 0; i < count_integer; i++) {
                cout << binary->integer[i];
        }
        cout << '.';
        for (int i = 0; i < count_fractional; i++) {
                cout << binary->fractional[i];
        }
}

void init_exp_mant(Binary* binary, string type) {
        int rank, exp, count_exp, count_mantissa, k;
        if (type == "f") {
                count_exp = 8, count_mantissa = 23, k = 127;
        }
        else {
                count_exp = 11, count_mantissa = 52, k = 1023;
        }
        binary->exp = new int[count_exp];
        binary->mantissa = new int[count_mantissa];
        if (binary->count_int != 1 && binary->integer[0] != 0) {
                for (int i = 0; i < binary->count_int; i++) {
                        if (binary->integer[i] == 1) {
                                rank = binary->count_int - i - 1;
                                int j = 0;
                                i++;
                                while (i < binary->count_int && j < count_mantissa) {
                                        binary->mantissa[j] = binary->integer[i];
                                        i++;
                                        j++;
                                }
                                int k = 0;
                                while (k < binary->count_frac && j < count_mantissa) {
                                        binary->mantissa[j] = binary->fractional[k];
                                        k++;
                                        j++;
                                }
                                while (j < count_mantissa) {
                                        binary->mantissa[j] = 0;
                                        j++;
                                }
                        }
                }
        }
        else {
                for (int i = 0; i < binary->count_frac; i++) {
                        while (binary->fractional[i] != 1) {
                                i++;
                        }
                        i++;
                        rank = i;
                        int j = 0;
                        while (i < binary->count_frac && j < count_mantissa) {
                                binary->mantissa[j] = binary->fractional[i];
                                i++;
                                j++;
                        }
                        while (j < count_mantissa) {
                                binary->mantissa[j] = 0;
                                j++;
                        }
                }
        }
        exp = rank + k;
        for (int j = count_exp-1; j >= 0; j--) {
                binary->exp[j] = exp % 2;
                exp = exp / 2;
        }
        cout << endl << "Number in IBM memory: " << '[' << binary->sign << "] [";
        for (int j = 0; j < count_exp; j++) {
                cout << binary->exp[j];
        }
        cout << "] [";
        for (int j = 0; j < count_mantissa; j++) {
                cout << binary->mantissa[j];
        }
        cout << ']' << endl;
}

void transformation (Binary* binary, string type) {
        int count_byte, exp, mantissa;
        if (type == "f") {
                count_byte = 4, exp = 8, mantissa = 23;
        }
        else {
                count_byte = 8, exp = 11, mantissa = 52;
        }
        binary->byte = new int [exp+mantissa+1];
        for (int i = 0; i < count_byte; i++) {
                int k = 128, j = exp+mantissa + 1 - 8 * (1 + i);
                while (k > 0) {
                        if (type == "f")
                                binary->byte[j] = bool(un.cf[i]&k);
                        else
                                binary->byte[j] = bool(un.cd[i]&k);
                        j++;
                        k = k >> 1;
                };
        }
        binary->exp = new int[exp];
        binary->mantissa = new int[mantissa];
        binary->sign = binary->byte[0];
        int count = 0;
        for (int i = 0; i < exp; i++) {
                binary->exp[i] = binary->byte[i + 1];
                count++;
        }
        for (int i = 0; i < mantissa; i++) {
                binary->mantissa[i] = binary->byte[count];
                count++;
        }
        cout << "Number in IBM memory: " << '[' << binary->sign << "] [";
        for (int j = 0; j < exp; j++) {
                cout << binary->exp[j];
        }
        cout << "] [";
        for (int j = 0; j < mantissa; j++) {
                cout << binary->mantissa[j];
        }
        cout << ']' << endl;
}

int main (int argc, char const *argv[]) {
        if (argc == 4) {
                string type = string(argv[2]);
                string method = string(argv[3]);
                if (method == "classic") {
                        Realis realis = {argv[1]};
                        Binary binary;
                        separation(&realis, &binary);
                        to_binary(&realis, &binary, type);
                        init_exp_mant(&binary, type);
                        delete[] binary.integer;
                        delete[] binary.fractional;
                        delete[] binary.exp;
                        delete[] binary.mantissa;
                }
                else {
                        Binary binary;
                        cout << "Entered number: " << argv[1] << endl;
                        if (type == "f")
                                un.f = stof(argv[1]);
                        else
                                un.d = stod(argv[1]);
                        transformation(&binary, type);
                        delete[] binary.byte;
                        delete[] binary.exp;
                        delete[] binary.mantissa;
                }
                return 0;
        }
        else
                return -1;
}
