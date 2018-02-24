#include "StemmerPorter.h"
#include <cctype>
#include <algorithm>
#include <iterator>
#include <functional>

const string StemmerPorter::const1 = "����������";
const string StemmerPorter::EMPTY = "";
const string StemmerPorter::S1 = "$1";
const string StemmerPorter::S13 = "$1$3";
const string StemmerPorter::SN = "�";

const regex StemmerPorter::PERFECTIVEGROUND = regex("(��|����|������|��|����|������|���|�����)$");
const regex StemmerPorter::REFLEXIVE = regex("(��|��)$");
const regex StemmerPorter::ADJECTIVE = regex("(��|��|��|��|���|���|��|��|��|��|��|��|��|��|���|���|���|���|��|��|��|��|��|��|��|��)$");
const regex StemmerPorter::PARTICIPLE = regex("(.*)(���|���|���)$|([��])(��|��|��|��|�)$");
const regex StemmerPorter::VERB = regex("(.*)(���|���|���|����|����|���|���|���|��|��|��|��|��|��|��|���|���|���|��|���|���|��|��|���|���|���|���|��|�)$|([��])(��|��|���|���|��|�|�|��|�|��|��|��|��|��|��|���|���)$");
const regex StemmerPorter::NOUN = regex("(�|��|��|��|��|�|����|���|���|��|��|�|���|��|��|��|�|���|��|���|��|��|��|�|�|��|���|��|�|�|��|��|�|��|��|�)$");
const regex StemmerPorter::I = regex("�$");
const regex StemmerPorter::P = regex("�$");
const regex StemmerPorter::NN = regex("��$");
const regex StemmerPorter::DERIVATIONAL = regex(".*[^���������]+[���������].*����?$");
const regex StemmerPorter::DER = regex("����?$");
const regex StemmerPorter::SUPERLATIVE = regex("(����|���)$");

const regex StemmerPorter::PUNCTUATION = regex("[^\\w\\s]$");

StemmerPorter::StemmerPorter() {
	setlocale(0, "");
}

string StemmerPorter::get(string s) {

	transform(s.begin(), s.end(), s.begin(), ::toupper);
	replace(s.begin(), s.end(), '�', '�');

	size_t pos = s.find_first_of(const1, 0);

	if (pos != string::npos) {

		// Step 1: Search for a PERFECTIVE GERUND ending.
		// If one is found remove it, and that is then the end of step 1.

		string pre = s.substr(0, pos + 1);
		string rv = s.substr(pos + 1);
		string temp = regex_replace(rv, PERFECTIVEGROUND, EMPTY);

		if (rv.size() != temp.size()) {
			rv = temp;
		}

		// Otherwise try and remove a REFLEXIVE ending, and then search in turn for 
		// (1) an ADJECTIVAL, (2) a VERB or (3) a NOUN ending. 
		// As soon as one of the endings (1) to (3) is found remove it, and terminate step 1.
		else {
			rv = regex_replace(rv, REFLEXIVE, EMPTY);
			temp = regex_replace(rv, ADJECTIVE, EMPTY);

			if (rv.size() != temp.size()) {
				rv = temp;
				rv = regex_replace(rv, PARTICIPLE, S13);
			}

			else {
				temp = regex_replace(rv, VERB, S13);
				if (rv.size() != temp.size()) {
					rv = temp;
				}
				else {
					rv = regex_replace(temp, NOUN, EMPTY);
				}
			}
		}

		// Step 2: If the word ends with � (i), remove it. 
		rv = regex_replace(rv, I, EMPTY);

		// Step 3: Search for a DERIVATIONAL ending in R2 
		// (i.e. the entire ending must lie in R2), and if one is found, remove it. 

		if (regex_match(rv, DERIVATIONAL)) {
			rv = regex_replace(rv, DER, EMPTY);
		}

		// Step 4: (1) Undouble � (n), or, (2) if the word ends with a SUPERLATIVE ending, 
		// remove it and undouble � (n), or (3) if the word ends � (') (soft sign) remove it. 
		temp = regex_replace(rv, P, EMPTY);

		if (temp.length() != rv.length()) {
			rv = temp;
		}

		else {
			rv = regex_replace(rv, SUPERLATIVE, EMPTY);
			rv = regex_replace(rv, NN, SN);
		}
		s = pre + rv;
	}

	transform(s.begin(), s.end(), s.begin(), tolower);
	return s;
}
