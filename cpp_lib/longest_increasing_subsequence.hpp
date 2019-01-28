// Authors: Marcus Östling, Tomas Möre
#include <limits>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <vector>

namespace popup 
{

/**
 *  Finds the longest increasing subsequence given an random access iterator to
 *  a sequence of values.
 *  Returns a vector of the indices in the longest subsequence.
 */
template <class RandomAccessIterator>
std::vector<size_t> longincseq(
        RandomAccessIterator values_begin,
        RandomAccessIterator values_end
) {
    size_t values_length = values_end - values_begin;

    if (values_length <= 0)
        return std::vector<size_t>();
    
    //
    double last[values_length+1];
    for (double& v : last)
        v = std::numeric_limits<double>::infinity();
    last[0] = -std::numeric_limits<double>::infinity();
 
    // last index used to modify last[]
    size_t last_index[values_length+1];    last_index[0] = -1;

    // Keeps track on the previous index used for the longest seqence.
    size_t seq_prev[values_length]; 

    auto start = values_begin;
    size_t subseq_length = 0;
    for (; start != values_end; start++) {

        // Minimize the search by ignoring the tail of infinity values. 
        auto first = std::lower_bound(last, last+subseq_length+1, *(start));
        *(first) = std::min(*(first), *(start));
        
        // Save the current index as last used for a subsequence of lenght first - last
        last_index[first - last] = start - values_begin;

        // Store the previous value of this subsequence
        seq_prev[start - values_begin] = last_index[first - last - 1];
        
        // Save the current longest subsequence
        subseq_length = std::max(subseq_length, (size_t)(first - last));
    }

    // Backtrack over seq_prev to find the indices used
    std::vector<size_t> res(subseq_length);
    res[res.size()-1] = last_index[subseq_length];
    for (int i = res.size()-2; i >= 0; i--) {
        res[i] = seq_prev[res[i+1]];
    }

    return res;
}

}
