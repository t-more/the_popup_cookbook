// Author: Marcus Östling, Tomas Möre 2019
#pragma once
#include <cstring>
#include <cstdint>

namespace popup
{

    size_t leastSignificatBit(size_t i) {
        return ((i)&(-i));
    }

    /**
     *  Fenwick trees
     *  also known as Binary Indexed Trees (BIT)
     *
     *  Useful for getting the sum of an subset
     *  and counting inversions.
     *
     *   Runtime:
     *   Create: O(n)
     *   Compute prefix sum: O(log n)
     *   Update prefix sum: O(log n)
     */
    class Fenwicktree {
        private:
            size_t m_size;
            int64_t *m_table;

        public:
            explicit Fenwicktree(size_t size);

            size_t size();
            void update(size_t i, int64_t delta);
            int64_t sum(size_t i);
            int64_t range_sum(size_t i, size_t j);
    };

    /**
     *  Constructor that initialize the fenwick tree of
     *  a given size.
     */
    Fenwicktree::Fenwicktree(size_t size) {
        // General idea of fenwick trees does not include
        // index 0, so for the sake of the lab...
        m_size = size+1;
        m_table = new int64_t[m_size];
        std::memset(m_table, 0, m_size*sizeof(int));
    }

    size_t Fenwicktree::size() {
        return m_size;
    }

    /**
     *  Updates the value of an index in the fenwick tree.
     *  O(logn)
     */
    void Fenwicktree::update(size_t i, int64_t delta)
    {
        // General idea of fenwick trees does not include
        // index 0, so for the sake of the lab...
        i += 1;
        while (i < m_size) {
            m_table[i] += delta;
            i += leastSignificatBit(i);
        }
    }

    /**
     * Returns the prefix sum of [0, i).
     * O(logn)
     */
    int64_t Fenwicktree::sum(size_t i){
        int64_t sum = 0;
        while (i > 0) {
            sum += m_table[i];
            i -= leastSignificatBit(i);
        }
        return sum;
    }

} //namespace popup
