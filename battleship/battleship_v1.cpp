#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
using namespace std;

int main() {
  int board[10][10];
  bool done = false;
  while (!done) {
    //guess
    
    int state = 0;
    if (state == 1) {
      board[0][0] = 2;
    } else if (state == 0) {
      board[0][0] = 1;
    } else {
      board[0][0] = 3;
    }
  }
}