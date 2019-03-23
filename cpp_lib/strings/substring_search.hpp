#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include <queue>
#include <set>
#include <array>

namespace popup {


    template<unsigned int S>
    class AlphabetArr
    {

        // This was a halfed arsed attempt to make an interator that became too boring
        // left in case we want to finnish it

        // class AlphabetArrMapIter  : public std::iterator<std::random_access_iterator_tag,
        //                                      std::pair<char, int>,
        //                                      ptrdiff_t,
        //                                      std::pair<char, int>*,
        //                                      std::pair<char, int>&> {
        //     const std::array<int, S>& iter_;
        //     size_t offset_;
        //     AlphabetArrMapIter(const std::array<int, S>& arr, size_t offset) {
        //         iter_ = arr;
        //         offset_ = offset;
        //     }

        //     AlphabetArrMapIter operator++() {
        //         size_t prev_idx = offset_++;

        //         return AlphabetArrMapIter(iter_, offset_);
        //     }

        // }

        // This is appently not a proper iterator... But i do not care, shitty ugly c++.
        class Alphabet {
        public:
            Alphabet(){}

            char cbegin() const {
                return 0;
            }
            char cend() const {
                return S;
            }
        };

        std::array<int, S> container_;
    public:
        AlphabetArr(){
            std::fill(container_.begin(), container_.end(), -1);
        };

        bool initialized(char c) const {
            return container_[c] != -1;
        }

        static Alphabet alphabet()  {
            return Alphabet();
        }

        int& operator[](const char c) {
            return container_[(size_t)c];
        }
    };

    using AsciiArr = AlphabetArr<128>;

    // The automaton will work using the alphabet container to keep track of
    // various insertions. Morespecifically the [] operator i expected to be
    // properly implemented for this. Each character shall be mapped to an index
    // as specified by the automaton. A simple example of this is tue AsciiArr
    // defined above. Also note that there must be a function that retrieves the
    // alphabet o

    //Secondly each matching string in the language will be
    // associated with some value. It is expected that this elemnts froms a
    // 'modified' monoid under '+=' and construction.
    template <typename Char, typename AlphabetContainer, typename Assoc>
    class AhoCorasickAutomaton {
        class Vertex {
        public:
            Assoc assoc_;
            bool leaf_ = false;
            int fail_link_ = -1;
            int exit_link_ = -1;
            AlphabetContainer transition_;
            Vertex() {};
        };

        std::vector<Vertex> automaton = std::vector<Vertex>(1);
        int current_index = 0;

    public:

        void build_automaton() {
            const auto alphabet = AlphabetContainer::alphabet();
            for (auto c = alphabet.cbegin(); c != alphabet.cend(); c++) {
                auto& next = automaton[0].transition_[c];
                if (next == -1) {
                    next = 0;
                }
            }
            std::queue<size_t> q;

            for (auto c = alphabet.cbegin(); c != alphabet.cend(); c++) {
                auto& next = automaton[0].transition_[c];
                if (next != 0) {
                    automaton[next].fail_link_ = 0;
                    q.push(next);
                }
            }

            while (!q.empty()) {
                auto state = q.front();
                q.pop();
                for (auto c = alphabet.cbegin(); c != alphabet.cend(); c++) {
                    auto& next = automaton[state].transition_[c];
                    if (next != -1) {
                        auto fail = automaton[state].fail_link_;
                        while (automaton[fail].transition_[c] == -1) {
                            fail = automaton[fail].fail_link_;
                        }
                        fail = automaton[fail].transition_[c];
                        automaton[next].fail_link_ = fail;
                        q.push(next);
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

        std::optional<std::vector<Assoc>> feed_char(Char c) {
            //current_index = transition((int)current_index, c);
            int next_state = current_index;
            while (automaton[next_state].transition_[c] == -1) {
                next_state = automaton[next_state].fail_link_;
            }
            current_index = automaton[next_state].transition_[c];
            std::vector<Assoc> result;
            std::set<Assoc> s;
            // automaton[state].fail_link_
            if (automaton[current_index].leaf_) {
                int idx = current_index;
                while(idx > 0 && automaton[idx].leaf_) {
                    if (s.find(automaton[idx].assoc_) == s.end()) {
                        result.push_back(automaton[idx].assoc_);
                        s.insert(automaton[idx].assoc_);
                    }
                    idx = automaton[idx].fail_link_;
                }
            }
            if (get_exit(current_index) > 0) {
                int idx = get_exit(current_index);
                while(idx > 0 && automaton[idx].leaf_) {
                    if (s.find(automaton[idx].assoc_) == s.end()) {
                        result.push_back(automaton[idx].assoc_);
                        s.insert(automaton[idx].assoc_);
                    }
                    idx = get_exit(idx);
                }
            }
            if (!result.empty()) {
                return result;
            } else {
                return std::nullopt;
            }
        }

        template <typename ForwardItr>
        void add_string(ForwardItr begin, ForwardItr end, Assoc assoc) {
            int idx = 0;

            for (auto itr = begin; itr != end; itr++) {
                auto& c = *itr;
                if (automaton[idx].transition_[c] == -1) {

                    automaton[idx].transition_[c] = (int)automaton.size();
                    automaton.emplace_back();
                }
                idx = automaton[idx].transition_[c];
            }
            automaton[idx].assoc_ += assoc;
            automaton[idx].leaf_ = true;
        }

    };

    //
    class MatchInfo {
    public:
        size_t start;
        size_t length;
        MatchInfo(size_t start, size_t length) : start(start), length(length) {}
        MatchInfo() : MatchInfo(0,0) {}

        MatchInfo& operator+=(const MatchInfo& other) {
            start = other.start;
            length = other.length;
            return *this;
        }

        bool operator<(const MatchInfo& other) const {
            return (start < other.start) || (start == other.start && length < other.length);
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
        AhoCorasickAutomaton<char, AsciiArr, MatchInfo> automaton;
        std::vector<std::pair<size_t, size_t>> match;

        for (auto itr = pattern_begin; itr != pattern_end; itr++) {
            auto& str = *itr;
            automaton.add_string(
                str.cbegin(),
                str.cend(),
                MatchInfo(
                    (size_t)(itr - pattern_begin),
                    str.size()
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

    // Utility for less need to write
    template <typename Assoc>
    using AsciiAhoCorasickAutomaton = AhoCorasickAutomaton<char, AsciiArr, Assoc>;

} // namespace popup
