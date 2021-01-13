#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <vector>
using namespace std;

#define weight_fn(i) i < PRIMARY_WORDS ? WEIGHT_FACTOR-(i+0.0)/PRIMARY_WORDS : PRIMARY_WORDS/(i+0.0)/WEIGHT_FACTOR

int main(int argc, char *argv[]) {
  const bool DEBUG = argc > 1 ? stoi(argv[1]) : false;
  const string FILE_NAME = argc > 2 ? argv[2] : "unigram_list.txt";
  const int NUM_WORDS = argc > 3 ? stoi(argv[3]) : 85000;
  const int PRIMARY_WORDS = argc > 4 ? stoi(argv[4]) : 15000;
  const int SECONDARY_WORDS = argc > 4 ? stoi(argv[5]) : 50000;
  const int WEIGHT_FACTOR = argc > 6 ? stoi(argv[6]) : 100;
  ifstream fin(FILE_NAME);
  string words[NUM_WORDS];
  for (int i = 0; i < NUM_WORDS; i++) { // read word list
    fin >> words[i];
  }
  bool test_mode = false; // test mode if actual letters are entered instead of blanks
  string test_phrase, input, prev_input, input_phrase = "";
  vector<string> test_words, input_words;
  int guesses = 0;
  vector<char> guessed_ltrs, wrong_ltrs;
  bool has_lied = false;
  cout << "Enter the word or phrase, replacing any unguessed letters with underscores." << endl;
  chrono::high_resolution_clock::time_point begin;
  while (true) {
    if (!test_mode) { // read each input
      cout << "> ";
      getline(cin, input);
      transform(input.begin(), input.end(), input.begin(), ::toupper);
      regex allowed_chars("[A-Z_ ]");
      for (int i = input.length(); i >= 0; i--) {
        string s;
        s.push_back(input[i]);
        if (!regex_match(s, allowed_chars))
          input.erase(i, 1);
      }
      test_phrase = input;
      if (input == prev_input) wrong_ltrs.push_back(guessed_ltrs[guesses-1]);
      for (int i = 0; i < input.length(); i++)
        input_phrase += input[i] >= 'A' && input[i] <= 'Z' ? '_' : input[i];
      if (DEBUG) begin = chrono::high_resolution_clock::now();
    } else {
      transform(input.begin(), input.end(), input.begin(), ::toupper);
    }
    if (guesses > 0 && input.find("_") == string::npos) break;
    input_words.clear();
    size_t pos = 0;
    while ((pos = input.find(" ")) != string::npos) { // split phrase into word vector
      input_words.push_back(input.substr(0, pos));
      input.erase(0, pos+1);
    }
    input_words.push_back(input);
    int num_words = input_words.size();
    double letter_freq[26] = {0.0};
    int letter_count[26] = {0};
    double sum_weights = 0.0; // sum of weights of matched words
    int matches[num_words]; // number of words that match
    for (int i = 0; i < num_words; i++) matches[i] = 0;
    vector<string> matched_words[num_words];
    bool secondary_ok = false;
    for (int index = 0; index < num_words; index++) {
      input = input_words[index];
      const int word_len = input.length();
      if (input.find("_") == string::npos) {
        if (guesses == 0) { // set test mode
          test_mode = true;
          test_words = input_words;
          input = "";
          for (int i = 0; i < word_len; i++) {
            input += "_";
          }
        }
      }
      if (guesses == 0) { // for the 1st guess, simply counts letter frequencies
        int has_letters[26] = {0};
        for (int i = 0; i < NUM_WORDS; i++) { // go thru word list
          int max_multiplier = 0;
          int len = words[i].length();
          if (len != word_len) continue;
          fill(has_letters, has_letters+26, false);
          for (int j = 0; j < len; j++) { // go thru letters in word
            char letter = words[i][j];
            int index = letter-'a';
            if (letter >= 'a' && letter <= 'z') {
              letter_freq[index] += weight_fn(i);
              letter_count[index]++;
              has_letters[index]++;
              if (has_letters[index] > max_multiplier) max_multiplier = has_letters[index];
            }
          }
          matches[index]++;
          sum_weights += (weight_fn(i))*max_multiplier;
        }
      } else { // for subsequent guesses, uses regex
        double prob_factor = 1;
        for (int scenario = 0; scenario <= wrong_ltrs.size(); scenario++) {
          string regex_str = ""; // for matching whole words
          string regex2_str = "[^"; // matches not previously guessed letters
          for (int i = 0; i < guesses; i++) {
            if (find(wrong_ltrs.begin(), wrong_ltrs.end(), guessed_ltrs[i]) == wrong_ltrs.end())
              regex2_str += guessed_ltrs[i];
          }
          if (scenario > 0) {
            string orig_str = regex_str;
            regex_str = "[";
            regex_str += wrong_ltrs[scenario-1];
            regex_str += orig_str+"]";
          }
          for (int i = 0; i < word_len; i++) {
            if (input[i] == '_') {
              regex_str += regex2_str+"]";
            } else {
              regex_str += tolower(input[i]);
            }
          }
          regex regexp(regex_str);
          int has_letters[26] = {0};
          bool found_match = false;
          for (int i = 0; i < NUM_WORDS; i++) { // go thru word list
            int max_multiplier = 1;
            int len = words[i].length();
            if (len != word_len || !regex_match(words[i], regexp)) continue;
            fill(has_letters, has_letters+26, false);
            for (int j = 0; j < len; j++) { // go thru letters in word
              char letter = words[i][j];
              int index = letter-'a';
              if (letter >= 'a' && letter <= 'z') {
                letter_freq[index] += (weight_fn(i))*prob_factor;
                letter_count[index]++;
                found_match = true;
                has_letters[index]++;
                if (has_letters[index] > max_multiplier) max_multiplier = has_letters[index];
              }
            }
            matches[index]++;
            sum_weights += (weight_fn(i))*max_multiplier*prob_factor;
            if (i < SECONDARY_WORDS || secondary_ok || matched_words[index].size() == 0) {
              matched_words[index].push_back(words[i]);
              if (i >= SECONDARY_WORDS) secondary_ok = true;
            }
          }
          if (!found_match) { // if no matches, fallback to just using letter frequencies
            fill(letter_freq, letter_freq+26, 0.0);
            for (int i = 0; i < NUM_WORDS; i++) {
              int len = words[i].length();
              if (len != word_len) continue;
              for (int j = 0; j < len; j++) {
                char letter = words[i][j];
                int index = letter-'a';
                if (letter >= 'a' && letter <= 'z') {
                  letter_freq[index] += (weight_fn(i))*prob_factor;
                }
              }
            }
          }
          if (has_lied) break;
          prob_factor = 1.0/7;
        }
      }
    }
    double max_freq = 0.0, prev_max_freq = numeric_limits<double>::max();
    char max_freq_char = 0, prev_max_freq_char = 0;
    string word_guess = ""; // if guessing the whole word
    bool done = false;
    for (int i = 0; !done; i++) {
      max_freq = 0.0;
      for (int j = 0; j < 26; j++) { // find max frequency letter closest to 50% of weight sum
        char letter = 'a'+j;
        if (DEBUG) cout << letter << ": " << letter_freq[j] << "\t";
        if (letter_freq[j] > max_freq && letter_freq[j] < prev_max_freq
            && (find(guessed_ltrs.begin(), guessed_ltrs.end(), letter) == guessed_ltrs.end()
            || !has_lied && find(wrong_ltrs.begin(), wrong_ltrs.end(), letter) != wrong_ltrs.end())) {
          max_freq = letter_freq[j];
          max_freq_char = letter;
        }
      }
      if (true || max_freq/sum_weights < 1-(guesses+1.0)/26 || max_freq_char == prev_max_freq_char) {
        done = true;
        if (i > 0) max_freq_char = prev_max_freq_char;
      }
      prev_max_freq = max_freq;
      prev_max_freq_char = max_freq_char;
      for (int j = 0; j < num_words; j++) {
        if (matches[j] == 0) done = true;
      }
      if (DEBUG) cout << endl << max_freq_char << endl;
    }
    if (find(wrong_ltrs.begin(), wrong_ltrs.end(), max_freq_char) != wrong_ltrs.end())
      has_lied = true;
    if (DEBUG) {
      for (int i = 0; i < num_words; i++) {
        cout << "MATCHES: " << matches[i] << endl;
        cout << "FREQ SUM: " << sum_weights << endl;
        if (matches[i] <= 10)
          for (int j = 0; j < matches[i]; j++) cout << matched_words[i][j] << endl;
      }
    }
    guessed_ltrs.push_back(max_freq_char);
    max_freq_char = toupper(max_freq_char);
    cout << "Guess #" << ++guesses << ": ";
    for (int i = 0; i < num_words; i++) {
      if (matched_words[i].size() == 1) word_guess += matched_words[i][0];
      else word_guess = "";
      word_guess += " ";
    }
    word_guess = word_guess.substr(0, word_guess.length()-1);
    transform(word_guess.begin(), word_guess.end(), word_guess.begin(), ::toupper);
    if (word_guess.length() == test_phrase.length()) {
      cout << word_guess << endl;
    } else {
      cout << max_freq_char << endl;
    }
    if (test_mode) {
      if (word_guess == test_phrase) input_phrase = word_guess;
      else for (int i = 0; i < test_phrase.length(); i++) {
        if (test_phrase[i] == max_freq_char) {
          input_phrase[i] = max_freq_char;
        }
      }
      input = input_phrase;
      cout << input << endl;
    }
    prev_input = input;
    if (DEBUG) {
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
      cout << "\tElapsed time: " << duration << " ms" << endl;
    }
  }
  cout << "Total Guesses: " << guesses << endl;
}