#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
using namespace std;

#define weight_fn(i) i < PRIMARY_WORDS ? WEIGHT_FACTOR-(i+0.0)/PRIMARY_WORDS : PRIMARY_WORDS/(i+0.0)/WEIGHT_FACTOR

int main(int argc, char *argv[]) {
  const bool DEBUG = argc > 1 ? stoi(argv[1]) : false;
  const string FILE_NAME = argc > 2 ? argv[2] : "unigram_list.txt";
  const int NUM_WORDS = argc > 3 ? stoi(argv[3]) : 50000;
  const int PRIMARY_WORDS = argc > 4 ? stoi(argv[4]) : 15000;
  const int WEIGHT_FACTOR = argc > 5 ? stoi(argv[5]) : 100;
  ifstream fin(FILE_NAME);
  string words[NUM_WORDS];
  for (int i = 0; i < NUM_WORDS; i++) { // read word list
    fin >> words[i];
  }
  bool test_mode = false; // test mode if actual letters are entered instead of blanks
  string test_word, input, prev_input;
  int guesses = 0;
  vector<char> guessed_ltrs;
  chrono::high_resolution_clock::time_point begin;
  while (true) {
    if (!test_mode) { // read each input
      cout << "> ";
      cin >> input;
      if (DEBUG) begin = chrono::high_resolution_clock::now();
    }
    const int word_len = input.length();
    if (input.find("_") == string::npos) { // set test mode
      if (guesses == 0) {
        test_mode = true;
        test_word = input;
        transform(test_word.begin(), test_word.end(), test_word.begin(), ::toupper);
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
    vector<string> matched_words;
    if (guesses == 0) { // for the 1st guess, simply counts letter frequencies
      bool has_letters[26];
      for (int i = 0; i < NUM_WORDS; i++) { // go thru word list
        int len = words[i].length();
        if (len != word_len) continue;
        fill(has_letters, has_letters+26, false);
        bool is_match = false;
        for (int j = 0; j < len; j++) { // go thru letters in word
          char letter = words[i][j];
          int index = letter-'a';
          if (letter >= 'a' && letter <= 'z' && !has_letters[index]) {
            letter_freq[index] += weight_fn(i);
            has_letters[index] = is_match = true;
          }
        }
        if (is_match) matches++;
      }
    } else { // for subsequent guesses, uses regex
      string regex_str = ""; // for matching whole words
      string regex2_str = "[^"; // matches previously guessed letters
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
      for (int i = 0; i < NUM_WORDS; i++) { // go thru word list
        int len = words[i].length();
        if (len != word_len || !regex_match(words[i], regexp)) continue;
        fill(has_letters, has_letters+26, false);
        bool is_match = false;
        for (int j = 0; j < len; j++) { // go thru letters in word
          char letter = words[i][j];
          int index = letter-'a';
          if (letter >= 'a' && letter <= 'z' && !has_letters[index]) {
            letter_freq[index] += weight_fn(i);
            has_letters[index] = is_match = found_match = true;
          }
        }
        if (is_match) {
          matches++;
          matched_words.push_back(words[i]);
        }
      }
      if (!found_match) { // if no matches, fallback to just using letter frequencies
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
    string word_guess; // if guessing the whole word
    for (int i = 0; i < 26; i++) { // calculate max frequency letter
      char letter = 'a'+i;
      if (DEBUG) cout << letter << ": " << letter_freq[i] << "\t";
      if (letter_freq[i] > max_freq && find(guessed_ltrs.begin(), guessed_ltrs.end(), letter) == guessed_ltrs.end()) {
        max_freq = letter_freq[i];
        max_freq_char = letter;
      }
    }
    if (DEBUG) cout << "\nMATCHES: " << matches << endl;
    if (DEBUG && matches <= 10)
      for (int i = 0; i < matches; i++) cout << matched_words[i] << endl;
    guessed_ltrs.push_back(max_freq_char);
    max_freq_char = toupper(max_freq_char);
    prev_input = input;
    cout << "Guess #" << ++guesses << ": ";
    if (matched_words.size() == 1) {
      word_guess = matched_words[0];
      transform(word_guess.begin(), word_guess.end(), word_guess.begin(), ::toupper);
      cout << word_guess << endl;
    } else {
      cout << max_freq_char << endl;
    }
    if (test_mode) {
      if (test_word == word_guess) input = word_guess;
      else for (int i = 0; i < word_len; i++) {
        if (test_word[i] == max_freq_char) {
          input[i] = max_freq_char;
        }
      }
      cout << input << endl;
    }
    if (DEBUG) {
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
      cout << "\tElapsed time: " << duration << " ms" << endl;
    }
  }
  cout << "Total Guesses: " << guesses << endl;
}