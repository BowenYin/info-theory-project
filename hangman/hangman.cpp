#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
using namespace std;

#define NUM_WORDS 25322

int main() {
  ios_base::sync_with_stdio(0); cin.tie(0);
  ifstream fin("popular.txt");
  string words[NUM_WORDS];
  for (int i = 0; i < NUM_WORDS; i++) {
    fin >> words[i];
  }
  //for (int i = 0; i < 26; i++) cout << letter_freq[i] << "\n";
  bool done = false;
  int guesses = 0;
  vector<char> guessed_ltrs;
  while (true) {
    string input;
    cin >> input;
    if (input.find("_") == string::npos) {
      break;
    }
    int word_len = input.length();
    int letter_freq[26] = {0};
    int total_letters = 0;
    if (guesses == 0) {
      for (int i = 0; i < NUM_WORDS; i++) {
        int len = words[i].length();
        if (len != word_len) continue;
        total_letters += len;
        for (int j = 0; j < len; j++) {
          char letter = words[i][j];
          if (letter >= 'a' && letter <= 'z') {
            letter_freq[letter-'a']++;
          }
        }
      }
    } else {
      string regex_str = "";
      for (int i = 0; i < word_len; i++) {
        if (input[i] == '_') {
          regex_str += ".";
        } else {
          regex_str += tolower(input[i]);
        }
      }
      regex regexp(regex_str);
      for (int i = 0; i < NUM_WORDS; i++) {
        int len = words[i].length();
        if (len != word_len || !regex_match(words[i], regexp)) continue;
        total_letters += len;
        for (int j = 0; j < len; j++) {
          char letter = words[i][j];
          if (letter >= 'a' && letter <= 'z') {
            letter_freq[letter-'a']++;
          }
        }
      }
    }
    int max_freq = 0;
    char max_freq_char;
    for (int i = 0; i < 26; i++) {
      char letter = 'a'+i;
      cout << letter << "\t" << letter_freq[i] << endl;
      if (letter_freq[i] > max_freq && find(guessed_ltrs.begin(), guessed_ltrs.end(), letter) == guessed_ltrs.end()) {
        max_freq = letter_freq[i];
        max_freq_char = letter;
      }
    }
    guessed_ltrs.push_back(max_freq_char);
    cout << "Guess #" << ++guesses << ": " << max_freq_char << endl;
  }
  cout << "Total Guesses: " << guesses << endl;
}