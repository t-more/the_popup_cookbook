#include <iostream>
#include "fenwick_tree.hpp"

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, q;
    cin >> n >> q;
    popup::Fenwicktree f(n);
    while(q-- > 0) {
        char c;
        cin >> c;

        int i,d;

        if(c == '+') {
            cin >> i >> d;
            f.update(i,d);
        } else if(c == '?') {  
            cin >> i;
            cout << f.sum(i) << "\n";
        }

    }

    return 0;
}

