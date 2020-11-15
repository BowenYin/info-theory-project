#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;

int main() {
  ifstream fin("frequency-alpha-gcide.txt");
  ofstream fout("freq_gcide.txt");
  string word, temp, freq_str;
  for (int i = 0; i < 65537; i++) {
    fin >> temp;
    fin >> word;
    fin >> freq_str;
    fin >> temp;
    fin >> temp;
    if (i == 0) continue;
    freq_str.erase(remove(freq_str.begin(), freq_str.end(), ','), freq_str.end());
    fout << word << " " << freq_str << "\n";
  }
  cout << "Done" << "\n";
}