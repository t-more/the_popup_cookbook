#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include <queue>

namespace popup {

template <typename Assoc>
class AhoCorasickAutomaton {
    const static int ALPHABET_LENGTH = 256;
    class Vertex {
    public:
        Assoc assoc_;
        bool leaf_ = false;
        int fail_link_ = -1;
        int exit_link_ = -1;
        int transition_[ALPHABET_LENGTH];

        Vertex() {
            std::fill(transition_, transition_+ALPHABET_LENGTH, -1);
        };

    };

    std::vector<Vertex> automaton = std::vector<Vertex>(1);
    int current_index = 0;

public:

    void build_automaton() {
        for (size_t c = 0; c < ALPHABET_LENGTH; c++) {
            if (automaton[0].transition_[c] == -1) {
                automaton[0].transition_[c] = 0;
            }
        }
        std::queue<size_t> q;

        for (size_t c = 0; c < ALPHABET_LENGTH; c++) {
            auto next = automaton[0].transition_[c];
            if (next != 0) {
                automaton[next].fail_link_ = 0;
                q.push(next);
            }
        }

        while (!q.empty()) {
            auto state = q.front();
            q.pop();
            for (size_t c = 0; c < ALPHABET_LENGTH; c++) {
                if (automaton[state].transition_[c] != -1) {
                    auto fail = automaton[state].fail_link_;
                    while (automaton[fail].transition_[c] == -1) {
                        fail = automaton[fail].fail_link_;
                    }
                    fail = automaton[fail].transition_[c];
                    automaton[automaton[state].transition_[c]].fail_link_ = fail;
                    q.push(automaton[state].transition_[c]);
                }
            }
        }
    }

    int get_exit(int idx) {
        if(automaton[idx].exit_link_ == -1) {
            int i = automaton[idx].fail_link_;
            while(i > 0) {
                if(automaton[i].leaf_) {
                    automaton[idx].exit_link_ = i;
                    break;
                } else if(automaton[i].exit_link_ != -1) {
                    automaton[idx].exit_link_ = automaton[i].exit_link_;
                    break; 
                } else {
                    i = automaton[i].fail_link_;
                }
            }
            if (automaton[idx].exit_link_ == -1) {
                automaton[idx].exit_link_ = -2;
            }
        }
        return automaton[idx].exit_link_;
    }

    std::optional<std::vector<Assoc>> feed_char(char c) {
        //current_index = transition((int)current_index, c);
        int next_state = current_index;
        while (automaton[next_state].transition_[(int)c] == -1) {
            next_state = automaton[next_state].fail_link_;
        }
        current_index = automaton[next_state].transition_[(int)c];

        // automaton[state].fail_link_
        if (automaton[current_index].leaf_ || get_exit(current_index) > 0) {
             int idx = automaton[current_index].leaf_ ?
                 current_index :
                 get_exit(current_index);
            std::vector<Assoc> result;
            while(idx > 0 && automaton[idx].leaf_) {
                result.push_back(automaton[idx].assoc_);
                idx = automaton[idx].fail_link_;
            }
            return result;
        } else {
            return std::nullopt;
        }
    }

    void add_string(const std::string &str, Assoc assoc) {
        int idx = 0;
        for (char c : str) {
            if (automaton[idx].transition_[(int)c] == -1) {
                automaton[idx].transition_[(int)c] = (int)automaton.size();
                automaton.emplace_back();
            }

            idx = automaton[idx].transition_[(int)c];
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
    automaton.build_automaton();

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
