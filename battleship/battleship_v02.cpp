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
  cout << "Reply to each guess with 'hit', 'miss', or 'sunk'." << endl;
  while (true) {
    for (int i = 0; i < 10; i++)
      for (int j = 0; j < 10; j++)
        probs[i][j] = 0;
    for (int ship = 0; ship < ships.size(); ship++) {
      for (int i = 0; i < 10; i++) { // horizontal ships
        for (int j = 0; j <= 10-ships[ship]; j++) {
          int hits = 0;
          bool ok = true;
          for (int j2 = j; j2 < j+ships[ship]; j2++) { // check if a ship fits
            if (board[i][j2] != 0 && board[i][j2] != 2) {
              ok = false;
              break;
            } if (board[i][j2] == 2) hits++;
          }
          if (!ok) continue;
          for (int j2 = j; j2 < j+ships[ship]; j2++) { // adjust weights for ship coordinates
            probs[i][j2] += 1+hits*100;
          }
        }
      }
      for (int i = 0; i <= 10-ships[ship]; i++) { // vertical ships
        for (int j = 0; j < 10; j++) {
          int hits = 0;
          bool ok = true;
          for (int i2 = i; i2 < i+ships[ship]; i2++) { // check if a ship fits
            if (board[i2][j] != 0 && board[i2][j] != 2) {
              ok = false;
              break;
            } if (board[i2][j] == 2) hits++;
          }
          if (!ok) continue;
          for (int i2 = i; i2 < i+ships[ship]; i2++) { // adjust weights for ship coordinates
            probs[i2][j] += 1+hits*100;
          }
        }
      }
    }
    int max_prob = 0;
    int max_i, max_j;
    for (int j = 0; j < 10; j++) { // find coordinates with max probability/weight
      for (int i = 0; i < 10; i++) {
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
    cout << "> ";
    cin >> input;
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (input.substr(0, 1) == "m") { // miss
      board[max_i][max_j] = 1;
    } else if (input.substr(0, 1) == "h") // hit
      board[max_i][max_j] = 2;
    else if (input.substr(0, 1) == "s") { // sink
      if (ships.size() <= 1) break;
      board[max_i][max_j] = 3;
      int ship_len = 1;
      for (int i = max_i-1; i >= 0 && i >= max_i-4; i--) // search upwards (in the array) for previous hits
        if (board[i][max_j] == 2) {
          board[i][max_j] = 3;
          ship_len++;
        } else break;
      for (int i = max_i+1; i < 10 && i <= max_i+4; i++) // search downwards
        if (board[i][max_j] == 2) {
          board[i][max_j] = 3;
          ship_len++;
        } else break;
      for (int j = max_j-1; j >= 0 && j >= max_j-4; j--) // search leftwards
        if (board[max_i][j] == 2) {
          board[max_i][j] = 3;
          ship_len++;
        } else break;
      for (int j = max_j+1; j < 10 && j <= max_j+4; j++) // search rightwards
        if (board[max_i][j] == 2) {
          board[max_i][j] = 3;
          ship_len++;
        } else break;
      auto it = find(ships.begin(), ships.end(), ship_len); // remove sunken ship from list
      ships.erase(it);
    }
    if (ships.size() == 0) break;
  }
  cout << "Total Guesses: " << guesses << endl;
}