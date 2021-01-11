#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
using namespace std;

#define NUM_WORDS 20000
#define FILE_NAME "20k.txt"

int main() {
  ifstream fin(FILE_NAME);
  string words[NUM_WORDS];
  for (int i = 0; i < NUM_WORDS; i++) {
    fin >> words[i];
  }
  bool test_mode = false;
  string test_word, input;
  int guesses = 0;
  vector<char> guessed_ltrs;
  while (true) {
    if (!test_mode) {
      cin >> input;
    }
    int word_len = input.length();
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
      //cout << letter << ": " << letter_freq[i] << endl;
      if (letter_freq[i] > max_freq && find(guessed_ltrs.begin(), guessed_ltrs.end(), letter) == guessed_ltrs.end()) {
        max_freq = letter_freq[i];
        max_freq_char = letter;
      }
    }
    guessed_ltrs.push_back(max_freq_char);
    max_freq_char = toupper(max_freq_char);
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