# Info Theory Project

## Hangman

#### hangman/hangman_v01.cpp
Read from text file, count number of occurrences of each letter after regex matching

#### hangman/hangman_v02.cpp
Count number of words containing each letter instead, and ignores duplicate letters within a word

#### hangman/hangman_v03.cpp
Take into account previous wrong guesses when regex matching

#### hangman/hangman_v04.cpp
Assign weights to each word in word list based on frequency

#### hangman/hangman_v05.cpp
Fallback to all words of the same length if no regex matches are found

#### hangman/hangman_v06.cpp
New weight computation algorithm, strongly favors first words and reduces weight of later words

#### hangman/hangman_v03-1.cpp
Modified v3 so it ignores words with previously guessed letters that are in the wrong positions

#### hangman/hangman_v07.cpp
Copied over changes from v3-1 and no longer uses frequency values

#### hangman/hangman_v08.cpp
Added ability to guess the entire word if only one match is found. Also added debug mode with more stats.

#### hangman/hangman_v09.cpp
Chooses letters near a certain threshold of frequency/total instead of always choosing the largest frequency

#### hangman/hangman_v8-1.cpp
Modified v8 so that it counts all letters in the frequency instead of just one letter per word

#### hangman/hangman_v10.cpp
Support multiple word phrases instead of just single words. Each word is evaluated independently.

#### hangman/hangman_v11.cpp
Fixed some edge case bugs, cleaned up some code, and improved some user experience.

#### hangman/hangman_v11-1.cpp
Fixed bug where the weight sum undercounted duplicate letters in a word.

#### hangman/hangman_v12.cpp
Allows up to one lie, where the user says a letter is not in the word when it actually is.

#### hangman/hangman_v13.cpp _(240 lines; 238 sloc)_
Modification to exclude words after a certain point in the list from being guessed in full.

### Benchmarking

Here are a few sample words/phrases and the corresponding number of guesses taken:

- `information theory` (6 guesses)
- `theoretical` (4 guesses)
- `entropy` (4 guesses)
- `calculus` (6 guesses)
- `derivative` (5 guesses)
- `integral` (4 guesses)
- `differential equation` (5 guesses)
- `polynomial division` (5 guesses)
- `partial differentiation` (10 guesses)
- `channel capacity` (7 guesses)
- `raze` (11 guesses)
- `rate` (5 guesses)
- `zoom` (13 guesses)
- `room` (7 guesses)
- `leave a star` (7 guesses)

### Caveats

The program currently doesn't consider the logical relationship between words if more than one word is given as input, i.e. `information theory` and `information banana` would have the same likelihood if both of the second words had the same likelihood. However, it does consider each word's invididual likelihood in the English language, i.e. the freestanding word `theory` is more common than `banana`.

**Time:** `O(n)`, n = length of word list

## Battleship

#### battleship/battleship_v01.cpp
Just testing out different methods of determining a ship that's sunk.

#### battleship/battleship_v02.cpp _(109 lines; 107 sloc)_
Simplified code and algorithm, uses computed probability densities.

### Notes
The program will find the point that can be occupied by the greatest number of ships given the current state of the game, with a weight bonus for ships occupying a position already marked as a hit. Assuming a random placement of ships, this will be the optimal position (the algorithm is deterministic).

**Time:** `O(n²)`, n = size of game board
![](https://87f7290bbb154c8753a737c7b24a6d1e.m.pipedream.net/info-theory-project)