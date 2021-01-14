#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
using namespace std;

#define contains(vec, el) (find(vec.begin(), vec.end(), el) != vec.end())

int main() {
  int board[10][10]; // 0 for unvisited, 1 for miss, 2 for hit, 3 for sink
  int probs[10][10];
  vector<int> ships = {2, 3, 3, 4, 5};
  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++)
      board[i][j] = 0;
  int guesses = 0;
  cout << "Reply to each guess with 'hit', 'miss', or 'sink'." << endl;
  while (true) {
    for (int i = 0; i < 10; i++)
      for (int j = 0; j < 10; j++)
        probs[i][j] = 0;
    for (int ship = 0; ship < ships.size(); ship++) {
      for (int i = 0; i < 10; i++) { // horizontal ships
        for (int j = 0; j <= 10-ships[ship]; j++) {
          int hits = 0;
          for (int j2 = j; j2 < j+ships[ship]; j2++) {
            if (board[i][j2] != 0 || board[i][j2] != 2) break;
            if (board[i][j2] == 2) hits++;
          }
          for (int j2 = j; j2 < j+ships[ship]; j2++) {
            probs[i][j2] += 1+hits*100;
          }
        }
      }
      for (int i = 0; i <= 10-ships[ship]; i++) { // vertical ships
        for (int j = 0; j < 10; j++) {
          int hits = 0;
          for (int i2 = i; i2 < i+ships[ship]; i2++) {
            if (board[i2][j] != 0 || board[i2][j] != 2) break;
            if (board[i2][j] == 2) hits++;
          }
          for (int i2 = i; i2 < i+ships[ship]; i2++) {
            probs[i2][j] += 1+hits*100;
          }
        }
      }
    }
    int max_prob = 0;
    int max_i, max_j;
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        if (board[i][j] == 0 && probs[i][j] > max_prob) {
          max_prob = probs[i][j];
          max_i = i;
          max_j = j;
        }
      }
    }
    guesses++;
    cout << "Guess #" << guesses << ": (" << 10-max_j << ", " << 10-max_i << ")" << endl;
    string input;
    cin >> input;
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (input.substr(0, 1) == "m") {
      board[max_i][max_j] = 1;
      for (int i = max_i-1; i >= 0 && i >= max_i-4; i--) {
        if (board[i][max_j] != 2) break;
        if (!contains(ships, max_i-i+1)) {
          //fill sunk
        }
        if (i == 0 || board[i-1][max_j] == 1) {
          if (max_j == 0 || max_j == 9 || board[i][max_j-1] == 1 && board[i][max_j+1] == 1) {
            
          }
        }
      }
      // see if the area surrounding ships guaranteed sunk
    } else if (input.substr(0, 1) == "h")
      board[max_i][max_j] = 2;
    else if (input.substr(0, 1) == "s") {
      if (ships.size() <= 1) break;
      board[max_i][max_j] = 3;
      
    }
    if (ships.size() == 0) break;
  }
  cout << "Total guesses: " << guesses << endl;
}