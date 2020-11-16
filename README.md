# info-theory

#### hangman/hangman_v1.cpp
Reads from text file, counts number of occurrences of each letter after regex matching

#### hangman/hangman_v2.cpp
Counts number of words containing each letter instead, and ignores duplicate letters within a word

#### hangman/hangman_v3.cpp
Takes into account previous wrong guesses when regex matching

#### hangman/hangman_v4.cpp
Assigns weights to each word in word list based on frequency

#### hangman/hangman_v5.cpp
Fallback to all words of the same length if no regex matches are found

#### hangman/hangman_v6.cpp
New weight computation algorithm, strongly favors first words and reduces value of later words

#### hangman/hangman_v3-1.cpp
Modifies v3 so it ignores words with previously guessed letters that are not in the correct positions

#### hangman/hangman_v7.cpp
Copies over changes from v3-1 and stops using letter frequency input