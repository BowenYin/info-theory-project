#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
using namespace std;

#define weight_fn(i) i < PRIMARY_WORDS ? WEIGHT_FACTOR-(i+0.0)/PRIMARY_WORDS : PRIMARY_WORDS/(i+0.0)/WEIGHT_FACTOR

int main(int argc, char *argv[]) {
  const string FILE_NAME = argc > 1 ? argv[1] : "list_filtered.txt";
  const int NUM_WORDS = argc > 2 ? stoi(argv[2]) : 78735;
  const int PRIMARY_WORDS = argc > 3 ? stoi(argv[3]) : 15000;
  const int WEIGHT_FACTOR = argc > 4 ? stoi(argv[4]) : 100;
  ifstream fin(FILE_NAME);
  string words[NUM_WORDS];
  for (int i = 0; i < NUM_WORDS; i++) {
    fin >> words[i];
  }
  bool test_mode = false;
  string test_word, input, prev_input;
  int guesses = 0;
  vector<char> guessed_ltrs;
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
    int matches = 0;
    if (guesses == 0) {
      bool has_letters[26];
      for (int i = 0; i < NUM_WORDS; i++) {
        int len = words[i].length();
        if (len != word_len) continue;
        fill(has_letters, has_letters+26, false);
        for (int j = 0; j < len; j++) {
          char letter = words[i][j];
          int index = letter-'a';
          if (letter >= 'a' && letter <= 'z' && !has_letters[index]) {
            letter_freq[index] += weight_fn(i);
            has_letters[index] = true;
            matches++;
          }
        }
      }
    } else {
      string regex_str = "";
      string regex2_str = "[^";
      for (int i = 0; i < guesses; i++) {
        regex2_str += guessed_ltrs[i];
      }
      for (int i = 0; i < word_len; i++) {
        if (input[i] == '_') {
          regex_str += regex2_str+"]";
        } else {
          regex_str += tolower(input[i]);
        }
      }
      regex regexp(regex_str);
      bool has_letters[26];
      bool found_match = false;
      for (int i = 0; i < NUM_WORDS; i++) {
        int len = words[i].length();
        if (len != word_len || !regex_match(words[i], regexp)) continue;
        fill(has_letters, has_letters+26, false);
        for (int j = 0; j < len; j++) {
          char letter = words[i][j];
          int index = letter-'a';
          if (letter >= 'a' && letter <= 'z' && !has_letters[index]) {
            letter_freq[index] += weight_fn(i);
            has_letters[index] = found_match = true;
            matches++;
          }
        }
      }
      if (!found_match) {
        fill(letter_freq, letter_freq+26, 0.0);
        for (int i = 0; i < NUM_WORDS; i++) {
          int len = words[i].length();
          if (len != word_len) continue;
          fill(has_letters, has_letters+26, false);
          for (int j = 0; j < len; j++) {
            char letter = words[i][j];
            int index = letter-'a';
            if (letter >= 'a' && letter <= 'z' && !has_letters[index]) {
              letter_freq[index] += weight_fn(i);
              has_letters[index] = true;
            }
          }
        }
      }
    }
    double max_freq = 0.0;
    char max_freq_char;
    for (int i = 0; i < 26; i++) {
      char letter = 'a'+i;
      //cout << letter << ": " << letter_freq[i] << "\t";
      if (letter_freq[i] > max_freq && find(guessed_ltrs.begin(), guessed_ltrs.end(), letter) == guessed_ltrs.end()) {
        max_freq = letter_freq[i];
        max_freq_char = letter;
      }
    }
    //cout << "\nMatches: " << matches << endl;
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