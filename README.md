# info-theory-project

## Hangman

#### hangman/hangman_v1.cpp
Read from text file, count number of occurrences of each letter after regex matching

#### hangman/hangman_v2.cpp
Count number of words containing each letter instead, and ignores duplicate letters within a word

#### hangman/hangman_v3.cpp
Take into account previous wrong guesses when regex matching

#### hangman/hangman_v4.cpp
Assign weights to each word in word list based on frequency

#### hangman/hangman_v5.cpp
Fallback to all words of the same length if no regex matches are found

#### hangman/hangman_v6.cpp
New weight computation algorithm, strongly favors first words and reduces weight of later words

#### hangman/hangman_v3-1.cpp
Modified v3 so it ignores words with previously guessed letters that are in the wrong positions

#### hangman/hangman_v7.cpp
Copied over changes from v3-1 and no longer uses frequency values

#### hangman/hangman_v8.cpp
Added ability to guess the entire word if only one match is found. Also added debug mode with more stats.

## Battleship
