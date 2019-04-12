#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin, x.end()
#define sx(x) (int)(x).size()
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;

int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);

  size_t num_persons;
  cin >> num_persons;
  cin.ignore();
  unordered_map<string, string> person_party;
  unordered_map<string, int> person_votes;

  for (size_t p = 0; p < num_persons; p++) {
      string person, party;
      getline(cin, person);
      getline(cin, party);
      person_votes[person] = 0;
      person_party[person] = party;
  }

  size_t num_votes;
  cin >> num_votes;
  cin.ignore();

  rep(n, 0, num_votes) {
      string person;
      getline(cin, person);
      auto pp = person_votes.find(person);
      if (pp != person_votes.end()) {
          pp->second += 1;
      }
  }

  string best_name;
  int best_score = 0;
  bool tie = tie;
  for (auto p : person_votes) {
      if (p.second > best_score) {
          best_score = p.second;
          best_name = p.first;
          tie = false;
      } else if (p.second == best_score) {
          tie = true;
      }
  }

  if (tie) {
      cout << "tie\n";
  } else {
      cout << person_party[best_name] << "\n";
  }

}
