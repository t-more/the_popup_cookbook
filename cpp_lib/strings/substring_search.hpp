// Authors: Tomas Möre, Marcus Östling 2019

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include <queue>
#include <set>
#include <array>
#include <unordered_set>

namespace popup {
    template<unsigned int Start, unsigned int End>
    class Alphabet {
    public:
        Alphabet(){}

        char cbegin() const {
            return Start;
        }
        char cend() const {
            return End;
        }
    };


    template<unsigned int S>
    class AlphabetArr {
        std::array<int, S> container_;
    public:
        AlphabetArr(){
            std::fill(container_.begin(), container_.end(), -1);
        };

        bool initialized(char c) const {
            return container_[c] != -1;
        }

        static Alphabet<0, S> alphabet()  {
            return Alphabet<0, S>();
        }

        int& operator[](const char c) {
            return container_[(size_t)c];
        }
    };

    using AsciiArr = AlphabetArr<128>;

    class LowercaseAscii {
        std::array<int, 26> container_;
    public:
        LowercaseAscii(){
            std::fill(container_.begin(), container_.end(), -1);
        };

        static Alphabet<97, 123> alphabet()  {
            return Alphabet<97, 123>();
        }

        int& operator[](const char c) {
            return container_[(size_t)(c - 'a')];
        }
    };
    /**
    * the automaton will work using the alphabet container to keep track of
    * various insertions. morespecifically the [] operator i expected to be
    * properly implemented for this. each character shall be mapped to an index
    * as specified by the automaton. a simple example of this is tue asciiarr
    * defined above. also note that there must be a function that retrieves the
    * alphabet o

    * secondly each matching string in the language will be
    * associated with some value. it is expected that this elemnts froms a
    * 'modified' monoid under '+=' and construction.
    */
    template <typename Char, typename AlphabetContainer, typename Assoc>
    class AhoCorasickAutomaton {
        friend class machresults;

        class vertex {
        public:
            Assoc assoc_;
            bool leaf_ = false;
            int fail_link_ = -1;
            int exit_link_ = -1;
            AlphabetContainer transition_;
            vertex() {};
        };

    public:
        /**
         * Match results provides a non-ideomatic iterator to the results of a
         * particular matching. The automata that provided the match must not be
         * deleted when this is run as it keeps an internal reference.
         */
        class MatchResults {
            AhoCorasickAutomaton* automaton_;
            int leaf_idx_;
            int exit_idx_;
            std::unordered_set<int> matched_;

            int next_idx_ = 0;
        public:

            MatchResults(AhoCorasickAutomaton* automaton, int index) {
                automaton_ = automaton;
                leaf_idx_ = index;
                exit_idx_ = automaton_->get_exit(index);
            }

            bool has_next() {
                if (leaf_idx_ > 0 && automaton_->automaton[leaf_idx_].leaf_) {
                    while(leaf_idx_ > 0 && automaton_->automaton[leaf_idx_].leaf_) {
                        if (!(matched_.find(leaf_idx_) != matched_.end())) {
                            next_idx_ = leaf_idx_;
                            matched_.insert(leaf_idx_);
                            leaf_idx_ = automaton_->automaton[leaf_idx_].fail_link_;
                            return true;
                        }
                        leaf_idx_ = automaton_->automaton[leaf_idx_].fail_link_;
                    }
                }
                if (exit_idx_ > 0 && automaton_->automaton[exit_idx_].leaf_) {
                    while(exit_idx_ > 0 && automaton_->automaton[exit_idx_].leaf_) {
                        if (!(matched_.find(exit_idx_) != matched_.end())) {
                            next_idx_ = exit_idx_;
                            matched_.insert(exit_idx_);
                            //                            matched_[exit_idx_] = true;
                            exit_idx_ = automaton_->get_exit(exit_idx_);
                            return true;
                        }
                        exit_idx_ = automaton_->get_exit(exit_idx_);
                    }
                }
                return false;
            }

            Assoc& next() const {
                return automaton_->automaton[next_idx_].assoc_;
            }


        };

    protected:
        std::vector<vertex> automaton = std::vector<vertex>(1);
        int current_index = 0;

    public:

        size_t automaton_size() const {
            return automaton.size();
        }

        /**
         *  Builds the automaton given the strings allready added, warning,
         *  should not be called more then once.
         */
        void build_automaton() {
            const auto alphabet = AlphabetContainer::alphabet();
            std::queue<size_t> q;

            // Adds self loops to the root node for characters that aren't used.
            for (auto c = alphabet.cbegin(); c != alphabet.cend(); c++) {
                auto& next = automaton[0].transition_[c];
                if (next == -1) {
                    next = 0;
                } else {
                    automaton[next].fail_link_ = 0;
                    q.push(next);
                }
            }

            // Performs a bfs from the root node, initalizing the fail links
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

        /**
         * Retrieves the exit link, that is a link to something that will match
         * some part of the prefix from a given node.
         *
         * Every node does not need to * have an exit link. In this case it will
         * have a value of -2.
         */
        int get_exit(int idx) {
            if (automaton[idx].exit_link_ == -1) {
                int i = automaton[idx].fail_link_;
                while (i > 0) {
                    if (automaton[i].leaf_) {
                        automaton[idx].exit_link_ = i;
                        break;
                    } else if (automaton[i].exit_link_ != -1) {
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

        // Sets the interal couting state to the start, aka 0;
        void reset() {
            current_index = 0;
        }

        /**
         *  Manually feeds a character into the automata from a specified
         *  state. Returns the next state and the match results.
         */
        std::pair<MatchResults, int> feed_char(int state, Char c) const {
            int next_state = state;
            while (automaton[next_state].transition_[c] == -1) {
                next_state = automaton[next_state].fail_link_;
            }
            next_state = automaton[next_state].transition_[c];
            return {MatchResults(this, next_state), next_state};
        }

        MatchResults feed_char(Char c) {
            //current_index = transition((int)current_index, c);
            int next_state = current_index;
            while (automaton[next_state].transition_[c] == -1) {
                next_state = automaton[next_state].fail_link_;
            }
            current_index = automaton[next_state].transition_[c];
            return MatchResults(this, current_index);
        }

        void add_string(const std::string& str, Assoc assoc) {
            return add_string(str.cbegin(), str.cend(), assoc);
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

    /**
     * Utility class to keep data about where a particular match is found
     */
    class StringMatchInfo {
    public:
        size_t start;
        size_t length;

        StringMatchInfo(size_t start, size_t length) : start(start), length(length) {}
        StringMatchInfo() : StringMatchInfo(0,0) {}

        StringMatchInfo& operator+=(const StringMatchInfo& other) {
            start = other.start;
            length = other.length;
            return *this;
        }

        bool operator<(const StringMatchInfo& other) const {
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
        AhoCorasickAutomaton<char, AsciiArr, StringMatchInfo> automaton;
        std::vector<std::pair<size_t, size_t>> match;

        for (auto itr = pattern_begin; itr != pattern_end; itr++) {
            auto& str = *itr;
            automaton.add_string(
                str.cbegin(),
                str.cend(),
                StringMatchInfo(
                    (size_t)(itr - pattern_begin),
                    str.size()
                )
            );
        }
        automaton.build_automaton();

        for (auto itr = text_begin; itr != text_end; itr++) {

            auto matches = automaton.feed_char(*itr);
            while (matches.has_next()) {
                auto [pattern_idx, match_size] = matches.next();
                match.push_back(
                    std::make_pair(
                        pattern_idx,
                        (size_t)(itr - text_begin - match_size + 1)
                    )
                );
            }
        }
        return match;
    }

    // Utility for less need to write
    template <typename Assoc>
    using AsciiAhoCorasickAutomaton = AhoCorasickAutomaton<char, AsciiArr, Assoc>;

} // namespace popup
