#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

namespace popup {

class AhoCorasickAutomaton {
    const static int ALPHABET_LENGTH = 255;
    class Vertex {
    public:
        size_t depth_;
        int parent_ = -1;
        char parent_char_;
        bool leaf_ = false;
        int suffix_link_ = -1;
        int transition_[ALPHABET_LENGTH];
        int next_[ALPHABET_LENGTH];

        Vertex(int parent=0, char parent_char='$', size_t depth=0) {
            parent_ = parent;
            parent_char_ = parent_char; 
            depth_ = depth;
            std::fill(next_, next_+ALPHABET_LENGTH, -1);
            std::fill(transition_, transition_+ALPHABET_LENGTH, -1);
        };

    };

    std::vector<Vertex> automaton = std::vector<Vertex>(1);
    size_t current_index = 0;

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
    std::optional<size_t> feed_char(char c) {
        current_index = transition((int)current_index, c);
        if (automaton[current_index].leaf_) {
            return automaton[current_index].depth_;
        } else {
            return std::nullopt;
        }
    }

    void add_string(const std::string &str) {
        int idx = 0; 
        for (char c : str) {
            if (automaton[idx].next_[c] == -1) {
                automaton[idx].next_[c] = (int)automaton.size();
                automaton.emplace_back(idx, c, automaton[idx].depth_+1);
            }
            idx = automaton[idx].next_[c];
        }
        automaton[idx].leaf_ = true;
    }
};

template <typename TextItr, typename PatternItr>
std::vector<size_t> get_occurences(
        TextItr text_begin,
        TextItr text_end,
        PatternItr pattern_begin,
        PatternItr pattern_end
) {
    AhoCorasickAutomaton automaton;
    std::vector<size_t> match_indices;

    for (auto itr = pattern_begin; itr != pattern_end; itr++) {
        automaton.add_string(*itr);
    }
    
    for (auto itr = text_begin; itr != text_end; itr++) {
        auto opt_depth = automaton.feed_char(*itr);
        if (opt_depth.has_value()) {
            match_indices.push_back(itr - text_begin - opt_depth.value()
                    + 1);
        }
    }

    return match_indices;
}

} // namespace popup
