// Authors: Marcus Östling, Tomas Möre 2019
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

#define ALPHABET_SIZE 256
namespace popup {

    /**
     * Cycle array will keep an sorted array of the cycles that can be created
     * from the input string. Complexity O(n * log n)
     */
    class CycleArray {
        std::vector<size_t> arr_;
    public:
        CycleArray(const std::string& str) {
            arr_ = std::vector<size_t>(str.size());
            std::vector<size_t>
                position(str.size()),
                classes(str.size()),
                count(std::max(ALPHABET_SIZE, (int)str.size()), 0);

            // First we run a counting sort on each element individualy, this is
            // mosly a startup run.
            for (size_t i = 0; i < str.size(); i++) {
                count[str[i]]++;
            }
            for (size_t i = 1; i < ALPHABET_SIZE; i++) {
                count[i] += count[i-1] ;
            }
            for (size_t i = 0; i < str.size(); i++) {
                size_t next_count = --count[str[i]];
                position[next_count] = i;
            }

            size_t num_classes = 1;
            for (size_t i = 1; i < str.size(); i++) {
                if (str[position[i]] != str[position[i-1]]) {
                    num_classes += 1;
                }
                classes[position[i]] = num_classes - 1;
            }

            std::vector<size_t>
                position_next(str.size()),
                classes_next(str.size());

            for (size_t high = 0; ((size_t)1 << high)  < str.size(); high++) {

                // Shifts the start of each string to sort
                for (size_t i = 0;  i < str.size(); i++) {
                    position_next[i] = position[i] - ((size_t)1 << high);
                    if (position_next[i] >= std::numeric_limits<size_t>::max() - str.size()) {
                        position_next[i] += str.size();
                    }
                }

                // Perform the counting sort
                std::fill(count.begin(), count.begin() + num_classes, 0);
                for (size_t i = 0; i < str.size(); i++) {
                    count[classes[position_next[i]]] += 1;
                }

                for (size_t i = 1; i < num_classes; i++) {
                    count[i] += count[i - 1];
                }
                for (int i = str.size() - 1; i >= 0; i--) {
                    auto new_count = --count[classes[position_next[i]]];
                    position[new_count] = position_next[i];
                }
                classes_next[position[0]] = 0;

                // Updates classes for next iteration
                num_classes = 1;
                for (size_t i = 1; i < str.size(); i++) {
                    std::pair<size_t, size_t> current_elem = {classes[position[i]],
                                                              classes[(position[i] + (1 << high)) % str.size()]};
                    std::pair<size_t, size_t> previous_elem = {classes[position[i-1]],
                                                              classes[(position[i-1] + (1 << high)) % str.size()]};
                    if (current_elem != previous_elem) {
                        num_classes += 1;
                    }
                    classes_next[position[i]] = num_classes - 1;
                }
                classes.swap(classes_next);
            }
            arr_ = position;
        }

        void remove_first() {
            arr_.erase(arr_.begin());
        }

        size_t at(const size_t i) const {
            return arr_.at(i);
        }

        size_t operator[](const size_t i) const {
            return at(i);
        }

        std::vector<size_t> as_vector() const {
            return arr_;
        }
    };

    /**
     * Version of cycle array that sorts according to the order of the
     * prefixes.
     *
     * O( n * log n)
     */
    class SuffixArray {
        std::vector<size_t> arr_;
        std::string string_;
    public:

        SuffixArray(const std::string& str) {
            string_ = std::string(str);
            string_.push_back('\0');
            arr_ = CycleArray(string_).as_vector();
            arr_.erase(arr_.begin());
        }
        size_t at(size_t i) const {
            return arr_.at(i);
        }
        size_t operator[](size_t i) const {
            return at(i);
        }
        std::vector<size_t> as_vector() const {
            return arr_;
        }
    };
}
