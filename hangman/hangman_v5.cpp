#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
  const string FILE_NAME = argc > 1 ? argv[1] : "/workspace/info-theory/hangman/unigram_freq.txt";
  const int NUM_WORDS = argc > 2 ? stoi(argv[2]) : 100000;
  const int WEIGHT_CAP_VALS = argc > 3 ? stoi(argv[3]) : 600;
  ifstream fin(FILE_NAME);
  string words[NUM_WORDS];
  long weights[NUM_WORDS];
  for (int i = 0; i < NUM_WORDS; i++) {
    fin >> words[i];
    fin >> weights[i];
  }
  const float MAX_WEIGHT = weights[0];
  const float MIN_WEIGHT = weights[NUM_WORDS-1];
  for (int i = 0; i < WEIGHT_CAP_VALS; i++) {
    weights[i] = weights[WEIGHT_CAP_VALS-1];
  }
  bool test_mode = false;
  string test_word, input, prev_input;
  int guesses = 0;
  vector<char> guessed_ltrs;
  vector<char> bad_ltrs;
  while (true) {
    if (!test_mode) {
      cout << "> ";
      cin >> input;
    }
    const int word_len = input.length();
    if (input.find("_") == string::npos) {
      if (guesses == 0) {
        test_mode = true;
        test_word = input;
        input = "";
        for (int i = 0; i < word_len; i++) {
          input += "_";
        }
      } else {
        break;
      }
    }
    double letter_freq[26] = {0.0};
    int total_letters = 0;
    if (guesses == 0) {
      bool has_letters[26];
      for (int i = 0; i < NUM_WORDS; i++) {
        int len = words[i].length();
        if (len != word_len) continue;
        total_letters += len;
        fill(has_letters, has_letters+26, false);
        for (int j = 0; j < len; j++) {
          char letter = words[i][j];
          int index = letter-'a';
          if (letter >= 'a' && letter <= 'z' && !has_letters[index]) {
            letter_freq[index] += log2(weights[i]/MIN_WEIGHT);
            has_letters[index] = true;
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
      if (input == prev_input) {
        bad_ltrs.push_back(guessed_ltrs.back());
      }
      string regex2_str = "[";
      int len = bad_ltrs.size();
      for (int i = 0; i < len; i++) {
        regex2_str += bad_ltrs[i];
      }
      regex regexp2(regex2_str+"]");
      bool has_letters[26];
      for (int i = 0; i < NUM_WORDS; i++) {
        int len = words[i].length();
        if (len != word_len || !regex_match(words[i], regexp) || regex_search(words[i], regexp2)) continue;
        total_letters += len;
        fill(has_letters, has_letters+26, false);
        for (int j = 0; j < len; j++) {
          char letter = words[i][j];
          int index = letter-'a';
          if (letter >= 'a' && letter <= 'z' && !has_letters[index]) {
            letter_freq[index] += log2(weights[i]/MIN_WEIGHT);
            has_letters[index] = true;
          }
        }
      }
    }
    int max_freq = 0;
    char max_freq_char;
    for (int i = 0; i < 26; i++) {
      char letter = 'a'+i;
      cout << letter << ": " << letter_freq[i] << "\t";
      if (letter_freq[i] > max_freq && find(guessed_ltrs.begin(), guessed_ltrs.end(), letter) == guessed_ltrs.end()) {
        max_freq = letter_freq[i];
        max_freq_char = letter;
      }
    }
    cout << endl;
    guessed_ltrs.push_back(max_freq_char);
    max_freq_char = toupper(max_freq_char);
    prev_input = input;
    cout << "Guess #" << ++guesses << ": " << max_freq_char << endl;
    if (test_mode) {
      for (int i = 0; i < word_len; i++) {
        if (toupper(test_word[i]) == max_freq_char) {
          input[i] = max_freq_char;
        }
      }
      cout << input << endl;
    }
  }
  cout << "Total Guesses: " << guesses << endl;
}