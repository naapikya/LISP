#include "functions.h"

int main() {
        std::ifstream i_file;
        i_file.open("gcd_file.txt");
        std::vector<token> objs = parser(i_file);
        interpreter(objs);
}

