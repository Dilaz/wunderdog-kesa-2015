/**
 * Wunderdog koodauspähkinä kesä 2015 - Sanojen muhkeus
 * Author: Risto "Dilaz" Viitanen / G-Works Oy
 *
 * This code requires file "alastalon_salissa.txt" to be in the working directory
 * In this file, the charset is changed to ISO-8859-1 or ISO-8859-1 to get rid
 * of multibyte characters (äöå).
 * You can do this by running the following command in just about any *nix system:
 * iconv -f UTF-8 -t ISO-8859-1 alastalon_salissa.txt > converted.txt && mv converted.txt alastalon_salissa.txt
 *
 *
 * Build command: g++ -O3 -std=c++11 foo.cpp -o Fooo
 * Run command: ./Fooo
 */

// Included libs
#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <string>
using std::string;

#include <map>
using std::map;
using std::pair;

#include <set>
using std::set;

#include <vector>
using std::vector;

#include <iterator>
using std::next;
using std::ostream_iterator;

#include <bitset>
using std::bitset;

#include <algorithm>
using std::transform;
using std::copy;

// cstdlib for EXIT_SUCCESS constant
// Because this is how i roll
#include <cstdlib>


// Filename constant
const string FILENAME("alastalon_salissa.txt");

// List of allowed characters
// The last hexa-characters are "äöå" (in order)
const string ALLOWED_CHARS("abcdefghijklmnopqrstuvwvxyz\xe4\xf6\xe5");
const string::size_type NUMBER_OF_CHARS = 30;


// Define some types to make our life more simple
typedef std::bitset<NUMBER_OF_CHARS> Word;

/**
 * Container for word bitset & list of texts matching it
 */
struct WordStorage {
	/**
	 * Empty constructor
	 * Baiscly not used in this but the compiler requires it
	 */
	WordStorage(){};

	/**
	 * Real constructor. This is created to use the WordStorage
	 * With the attributes we want to save
	 * @param word "Word", in this case the std::bitset we use to store the characters in
	 * @param str Original string where we got the characters from. This is used to print the final solution
	 * @return void
	 */
	WordStorage(const Word &word, const std::string &str)
	: key(word) {
		// Add the word to the list
		results.insert(str);
	};

	/**
	 * @param key This is the bitset we use to store the key in
	 */
	const Word key;

	/**
	 * @param results List of strings with the resulting words
	 */
	std::set<std::string> results;
};

// Some typedefs to make our code a bit more elegant
typedef std::map<unsigned long, WordStorage> WordMap;
typedef std::pair<unsigned long, WordStorage> WordPair;
typedef WordMap::iterator WordMapIter;
typedef std::set<std::string> WordSet;
typedef std::string::const_iterator StringIter;
typedef std::vector<std::string> WordVector;

/**
 * Deletes any unwanted characters from the given string
 * @param str String to trim
 */
inline void trim(string &str) {
	string::size_type pos;

	// Loop through it until no unallowed chracters are found
	while ((pos = str.find_first_not_of(ALLOWED_CHARS)) != string::npos) {
		// Delete the character
		str.erase(pos, pos + 1);
	}
}

/**
 * Turn std::string into std::bitset<32> where each bit represends
 * a letter in the word. This way we can count the bits to get the "Muhkeus"
 * @param  str String to convert
 * @return     bitset which we want to use later
 */
inline Word strToBitset(const string &str) {
	// Create new bitset
	Word word;
	string::size_type pos = 0;

	// Loop throught the word
	for (string::size_type i = 0; i < str.length(); i++) {
		char chr = str[i];

		// If current letter is in allowed characters, save the position
		if ((pos = ALLOWED_CHARS.find_first_of(chr)) != string::npos) {
			// Set n:th bit in bitset to 1 to mark that we have the letter
			word.set(pos, 1);
		}
	}

	// Fin.
	return word;
}


/**
 * Program entry point
 * @return int EXIT_SUCCESS or EXIT_FAILURE
 */
int main() {
	// Filestream for the book
	ifstream f(FILENAME.c_str());
	string word;
	unsigned int uniq;
	bool deleted;

	// List of words
	WordMap words;

	// Current word
	Word bWord;
	// Current word in ulong
	unsigned long bNum = 0;

	// List of winning word-pairs
	WordVector winners;
	unsigned int winnerLength = 0;

	// Some iterators
	WordMapIter iter;
	WordMapIter iter2;
	WordMapIter del;

	
	size_t iter1Size;
	size_t iter2Size;

	// Read the whole file
	while (f >> word) {
		// First turn the word into lowercase
		// we need to cast std::lower() because there are 2 options
		// the one we want to use, returns int, is global (*) and takes int as parameter (int)
		// -> int(*)(int)
		transform(word.begin(), word.end(), word.begin(), static_cast<int(*)(int)>(tolower));

		// Remove all unnecessary characters
		trim(word);

		// Turn the word into bitset
		bWord = strToBitset(word);

		// If no characters are left, just move to the next one
		if (word.empty()) {
			continue;
		}

		// Turn bitset into unsigned long int
		// We use this as "hash" key for words with similar muhkeus
		bNum = bWord.to_ulong();

		// Check if the combination of letters already exists
		if (words.find(bNum) != words.end()) {
			// Add the new word to the list for later use
			words[bNum].results.insert(word);
		}
		else {
			// The combination doesnt exist so we create a new cell for it
			words.insert(WordPair(bNum, WordStorage(bWord, word)));
		}

	}

	// Debug-ish print, just to know how many words we have left after first loop
	cout << "Words: " << words.size() << endl;


	// Here we loop the words and calculate the muhkeus of these words by bruteforce
	iter = words.begin();
	deleted = false;
	while (iter != words.end()) {
		iter1Size = iter->second.key.count();

		// Set j to the next word of current one
		iter2 = next(iter);

		// Inner loop from n:th letter to end end
		while (iter2 != words.end()) {
			iter2Size = iter2->second.key.count();

			// Find out the combined muhkeus
			// We use logical OR-operator here to get all the combined bits to 1
			// and then count the 1-bits to get muhkeus
			uniq = (iter->second.key | iter2->second.key).count();

			// If muhkeus is higher than previous winner, replace it
			if (uniq > winnerLength) {
				// Clear all the old words
				winners.clear();
				// Set new winner length to this one
				winnerLength = uniq;

				// Add words to winning words list
				for (auto result1 : iter->second.results) {
					for (auto result2 : iter2->second.results) {
						winners.push_back(result1 + ' ' + result2);
					}
				}
			}

			// Otherwise if muhkeus is same as previous winner
			else if (uniq == winnerLength) {
				// Add more winning word-pairs to the list
				for (auto result1 : iter->second.results) {
					for (auto result2 : iter2->second.results) {
						winners.push_back(result1 + ' ' + result2);
					}
				}
			}

			// Check if combined muhkeus of both of the words we are comparing
			// is smaller than current winning muhkeus.
			// If this is the case, we can delete the smaller of these words
			// because of math!
			else if (iter1Size + iter2Size < winnerLength) {
				// Take the smaller one and remove it
				if (iter1Size < iter2Size) {
					// Save the iterator and move to the next one
					del = iter;
					iter++;
					// Delete the word
					words.erase(del);

					// Set deleted to true so outer loop knows to skip the word too
					deleted = true;
					break;
				}
				// Same thing but if inner word is smaller
				else if (iter1Size > iter2Size) {
					// Save the iter
					del = iter2;

					// Move to the next one
					iter2++;

					// Delete the word
					words.erase(del);

					// Continue with this loop
					continue;
				}
			}

			// Inner loop increment
			iter2++;
		}

		// If word was deleted, skip incrementation
		if (deleted) {
			deleted = false;
		}
		else {
			// Outer loop increment
			iter++;
		}
	}
	cout << words.size() << endl;

	// Print total word-pair count
	cout << "Total number of word-pairs: " << winners.size() << endl << endl;

	// Print all the winning word pairs
	// Hacky method to copy the whole vector to cout split by "\n"
	copy(winners.begin(), winners.end(), ostream_iterator<string>(cout, "\n"));

	// Also print the muhkeus
	cout << endl << "Muhkeus: " << winnerLength << endl;

	// Everything went better than expected
	return EXIT_SUCCESS;
}
