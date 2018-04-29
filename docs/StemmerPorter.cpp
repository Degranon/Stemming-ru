/**
 * \file
 * \brief Methods of stemming algorithm class
 *
 * This file contains the methods and functions of the stemming algorithm class for keyword search
 */
#include <algorithm>
#include <iterator>
#include <functional>
#include <clocale>
#include "StemmerPorter.h"

const wstring StemmerPorter::const1 = L"����������";
const wstring StemmerPorter::EMPTY = L"";
const wstring StemmerPorter::S1 = L"$1";
const wstring StemmerPorter::S13 = L"$1$3";
const wstring StemmerPorter::SN = L"�";

const wregex StemmerPorter::PERFECTIVEGROUND = wregex(L"(��|����|������|��|����|������|���|�����)$");
const wregex StemmerPorter::REFLEXIVE = wregex(L"(��|��)$");
const wregex StemmerPorter::ADJECTIVE = wregex(L"(��|��|��|��|���|���|��|��|��|��|��|��|��|��|���|���|���|���|��|��|��|��|��|��|��|��)$");
const wregex StemmerPorter::PARTICIPLE = wregex(L"(.*)(���|���|���)$|([��])(��|��|��|��|�)$");
const wregex StemmerPorter::VERB = wregex(L"(.*)(���|���|���|����|����|���|���|���|��|��|��|��|��|��|��|���|���|���|��|���|���|��|��|���|���|���|���|��|�)$|([��])(��|��|���|���|��|�|�|��|�|��|��|��|��|��|��|���|���)$");
const wregex StemmerPorter::NOUN = wregex(L"(�|��|��|��|��|�|����|���|���|��|��|�|���|��|��|��|�|���|��|���|��|��|��|�|�|��|���|��|�|�|��|��|�|��|��|�)$");
const wregex StemmerPorter::I = wregex(L"�$");
const wregex StemmerPorter::P = wregex(L"�$");
const wregex StemmerPorter::NN = wregex(L"��$");
const wregex StemmerPorter::DERIVATIONAL = wregex(L".*[^���������]+[���������].*����?$");
const wregex StemmerPorter::DER = wregex(L"����?$");
const wregex StemmerPorter::SUPERLATIVE = wregex(L"(����|���)$");

const wregex StemmerPorter::PUNCTUATION = wregex(L"[^\\w\\s]$");

StemmerPorter::StemmerPorter() {}

wstring StemmerPorter::get(wstring s) {
  transform(s.begin(), s.end(), s.begin(), towupper);
  replace(s.begin(), s.end(), '�', '�');

  size_t pos = s.find_first_of(const1, 0);

  if (pos != wstring::npos) {

    // Step 1: Search for a PERFECTIVE GERUND ending.
    // If one is found remove it, and that is then the end of step 1.

    wstring pre = s.substr(0, pos + 1);
    wstring rv = s.substr(pos + 1);
    wstring temp = regex_replace(rv, PERFECTIVEGROUND, EMPTY);

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

  transform(s.begin(), s.end(), s.begin(), towlower);
  return s;
}