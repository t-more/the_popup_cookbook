#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <utility>

namespace popup {

template <typename Assoc>
class AhoCorasickAutomaton {
    const static int ALPHABET_LENGTH = 256;
    class Vertex {
    public:
        Assoc assoc_;
        int parent_ = -1;
        char parent_char_;
        bool leaf_ = false;
        int suffix_link_ = -1;
        int exit_link_ = -1;
        int transition_[ALPHABET_LENGTH];
        int next_[ALPHABET_LENGTH];

        Vertex(int parent=0, char parent_char='$') {
            parent_ = parent;
            parent_char_ = parent_char; 
            std::fill(next_, next_+ALPHABET_LENGTH, -1);
            std::fill(transition_, transition_+ALPHABET_LENGTH, -1);
        };

    };

    std::vector<Vertex> automaton = std::vector<Vertex>(1);
    int current_index = 0;

    int get_exit(int idx) {
        if(automaton[idx].exit_link_ == -1) {
            int i = get_link(idx);
            while(i > 0) {
                if(automaton[i].leaf_) {
                    automaton[idx].exit_link_ = i;
                    break;
                } else {
                    i = get_link(i);
                }
            }
            if (automaton[idx].exit_link_ == -1) {
                automaton[idx].exit_link_ = -2;
            }
        }
        return automaton[idx].exit_link_;
    }

    int get_link(int idx) {
        if (automaton[idx].suffix_link_ == -1) {
            if (idx == 0 || automaton[idx].parent_ == 0) {
                automaton[idx].suffix_link_ = 0;
            } else {
                automaton[idx].suffix_link_ = transition(
                    get_link(automaton[idx].parent_), 
                    automaton[idx].parent_char_
                );
            }
        }
        return automaton[idx].suffix_link_;
    }

    int transition(int idx, char c) {
        if (automaton[idx].transition_[c] == -1) {
            if (automaton[idx].next_[c] != -1) {
                automaton[idx].transition_[c] = automaton[idx].next_[c];
            } else {
                if (idx == 0) {
                    automaton[idx].transition_[c] = 0;
                } else {
                    automaton[idx].transition_[c] = transition(
                        get_link(idx),
                        c
                    );
                }
            }
        }
        return automaton[idx].transition_[c];
    }

public:
    std::optional<std::vector<Assoc>> feed_char(char c) {
        current_index = transition((int)current_index, c);
        if (automaton[current_index].leaf_ || get_exit(current_index) > 0) {
            int idx = automaton[current_index].leaf_ ? 
                current_index :
                get_exit(current_index);
            std::vector<Assoc> result;
            while(idx > 0 && automaton[idx].leaf_) {
                result.push_back(automaton[idx].assoc_);
                idx = get_link(idx);
            }
            return result;
        } else {
            if(exit > 0) {

            }
            return std::nullopt;
        }
    }

    void add_string(const std::string &str, Assoc assoc) {
        int idx = 0; 
        for (char c : str) {
            if (automaton[idx].next_[c] == -1) {
                automaton[idx].next_[c] = (int)automaton.size();
                automaton.emplace_back(idx, c);
            }
            idx = automaton[idx].next_[c];
        }
        automaton[idx].assoc_ = assoc;
        automaton[idx].leaf_ = true;
    }
};

/**
 *  Returns a list of pairs with the index of the pattern and
 *  the occurence index in the text.
 */
template <typename TextItr, typename PatternItr>
std::vector<std::pair<size_t,size_t>> get_occurences(
        TextItr text_begin,
        TextItr text_end,
        PatternItr pattern_begin,
        PatternItr pattern_end
) {
    AhoCorasickAutomaton<std::pair<size_t, size_t>> automaton;
    std::vector<std::pair<size_t, size_t>> match;

    for (auto itr = pattern_begin; itr != pattern_end; itr++) {
        automaton.add_string(
            *itr, 
            std::make_pair(
                (size_t)(itr - pattern_begin), 
                (*itr).size()
            )
        );
    }
    
    for (auto itr = text_begin; itr != text_end; itr++) {
        auto opt_res = automaton.feed_char(*itr);
        if (opt_res.has_value()) {
            for (auto [pattern_idx, match_size] : opt_res.value()) {
                match.push_back(
                    std::make_pair(
                        pattern_idx,
                        (size_t)(itr - text_begin - match_size + 1)
                    )
                );
            }
        }
    }

    return match;
}

} // namespace popup
