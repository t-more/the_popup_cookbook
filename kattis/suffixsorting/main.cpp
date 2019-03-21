#include <iostream>
#include <string>
#include "suffix_array.hpp"

using namespace std;

int main () {
    string input;
    input.reserve(100000);

    while (getline(cin, input)) {
        input.push_back('\0');
        popup::SuffixArray sa(input);
        sa.remove_first();

        //sa.print_order();

        size_t num_queries;
        cin >> num_queries;
        for (size_t i = 0; i < num_queries; i++) {
            size_t order;
            cin >> order;
            cout << sa.get_suffix(order) << " ";
        }
        cout << "\n";
        cin.ignore();
    }
}
