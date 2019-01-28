#include "longincseq.hpp"
#include <iostream>
#include <vector>

int main()
{
    int n;
    while(std::cin >> n)
    {
        //std::cout << n << std::endl;
        std::vector<double> values(n);
        for(double& v : values)
            std::cin >> v;
       
        std::vector<size_t> res = popup::longincseq(values.begin(), values.end());
        std::cout << res.size() << "\n";
        for(double v : res)
            std::cout << v << " ";
        std::cout << "\n";

    }

}
