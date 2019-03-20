#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

#define ALPHABET_SIZE 256
namespace popup {
    class SuffixArray {
        std::vector<size_t> arr_;

    public:
        SuffixArray(const std::string& str) {
            arr_ = std::vector<size_t>(str.size());
            std::vector<size_t>
                position(str.size()),
                classes(str.size()),
                count(std::max(ALPHABET_SIZE, (int)str.size()), 0);

            for (size_t i = 0; i < str.size(); i++) {
                count[str[i]]++;
            }
            for (size_t i = 1; i <ALPHABET_SIZE; i++) {
                count[i] += count[i-1] ;
            }
            for (size_t i = 0; i < str.size(); i++) {
                count[str[i]] -= 1;
                position[count[str[i]]] = i;
            }
            count[position[0]] = 0;
            size_t num_classes = 0;
            for (size_t i = 1; i < str.size(); i++) {
                if (str[position[i]] != str[position[i-1]]) {
                    num_classes += 1;
                }
                count[position[i]] = num_classes - 1;
            }

            std::vector<size_t>
                position_next(str.size()),
                classes_next(str.size());


            for (size_t high = 0; (1 << high)  < str.size(); high++) {
                for (size_t i = 0;  i < str.size(); i++) {
                    position_next[i] = position[i] - (1 << high);
                    if (position_next[i] > std::numeric_limits<size_t>::max() - str.size()) {
                        position_next[i] += str.size();
                    }
                }
                std::fill(count.begin(), count.end(), 0);
                for (size_t i = 0; i < str.size(); i++) {
                    count[classes[position_next[i]]] += 1;
                }
                for (size_t i = 0; i < str.size(); i++) {
                    count[i] += count[i - 1];
                }
                for (size_t i = 0; i < str.size(); i++) {
                    count[classes[position_next[i]]] -= 1;
                    position[count[classes[position_next[i]]]] = position_next[i];
                }
                classes_next[position[0]] = 0;
                size_t num_classes = 1;
                for (size_t i = 1; i < str.size(); i++) {
                    std::pair<size_t, size_t> current_elem = {classes[position[i]],
                                                              classes[(position[i] + (1 << high)) % str.size()]};
                    std::pair<size_t, size_t> previous_elem = {classes[position[i-1]],
                                                              classes[(position[i-1] + (1 << high)) % str.size()]};
                    if (current_elem != previous_elem) {
                        num_classes += 1;
                    }
                    classes_next[i] = num_classes - 1;
                }
                classes.swap(classes_next);
            }
            arr_ = position;
        }


        size_t get_suffix(size_t i) {
            return arr_.at(i);
        }
    };

}
