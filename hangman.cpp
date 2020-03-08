/*
    hangman.cpp
        
    Method implementations for the hangman class.
    
    assignment: CSCI 262 Project - Evil Hangman        

    author:

    last modified: 9/24/2017
*/

#include "hangman.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

// constructor
hangman::hangman() { 
    // Read in and store words from dictionary.txt
    // Initialize game state variables
    ifstream fin;
    fin.open("dictionary.txt");
    while(!fin.eof()) {
        string word;
        fin >> word;
        words.insert(word);
        word_lengths.insert(word.length());
    }
    fin.close();
    word_lengths.erase(0);
    guessed_chars = "";
    correct_letters = 0;
}


// start_new_game()
//
// Setup a new game of hangman.
void hangman::start_new_game(int num_guesses) {
    guesses_remaining = num_guesses;
}


// process_guess()
//
// Process a player's guess - should return true/false depending on whether
// or not the guess was in the hidden word.  If the guess is incorrect, the
// remaining guess count is decreased.
bool hangman::process_guess(char c) {
    guessed_chars += c;
    sort(guessed_chars.begin(), guessed_chars.end());
    for(auto it: word_list) {
        string word_family = "";
        for(int i=0; i<it.length(); i++) {
            if(it[i]==c)
                word_family+="1";
            else
                word_family+="0";
        }
        word_families.insert(word_family);
    }
    for(auto it: word_families) {
        set<string> word_family_members;
        string word_fam;
        for(auto it1: word_list) {
            word_fam = "";
            for(int i=0; i<it1.length(); i++) {
                if(it1[i]==c)
                    word_fam+="1";
                else
                    word_fam+="0";
            }
            if(word_fam==it) {
                word_family_members.insert(it1);
            }
        }
        word_families_map.insert( pair<string,set<string>>(it, word_family_members) );
    }
    int max_word_family_size = 0;
    string max_word_family_first;
    set<string> max_word_family_second;
    for(auto it: word_families_map) {
        if(it.second.size()>max_word_family_size) {
            max_word_family_size = it.second.size();
            max_word_family_first = it.first;
            max_word_family_second = it.second;
            current_word_family = it.first;
        }
    }
    if(words_remaining)
        cout << "Remaining in word list: " << max_word_family_size << endl;
    bool good_guess = current_word_family.find('1')!=string::npos;
    word_families.clear();
    word_families_map.clear();
    word_list = max_word_family_second;
    cout << "You guessed: " << current_guess << endl;
    if(good_guess)
        return true;
    else {
        guesses_remaining--;
        return false;
    }
}


// get_display_word()
//
// Return a representation of the hidden word, with unguessed letters
// masked by '-' characters.
string hangman::get_display_word() {
    if(display_word.empty()) {
        for (int i = 0; i < word_length; i++)
            display_word += "-";
    }
    for(int i=0; i<current_word_family.length(); i++) {
        if(current_word_family[i]=='1') {
            display_word[i] = current_guess;
            correct_letters++;
        }
    }
    return display_word;
}


// get_guesses_remaining()
//
// Return the number of guesses remaining for the player.
int hangman::get_guesses_remaining() {
    return guesses_remaining;
}


// get_guessed_chars()
//
// What letters has the player already guessed?  Return in alphabetic order.
string hangman::get_guessed_chars() {
    return guessed_chars;
}


// was_char_guessed()
//
// Return true if letter was already guessed.
bool hangman::was_char_guessed(char c) {
    return guessed_chars.find(c) != string::npos;
}


// is_won()
//
// Return true if the game has been won by the player.
bool hangman::is_won() {
    if(correct_letters>=word_length && display_word.find('-')==string::npos)
        return true;
    else
        return false;
}


// is_lost()
//
// Return true if the game has been lost.
bool hangman::is_lost() {
    if(guesses_remaining==0)
        return true;
    else
        return false;
}


// get_hidden_word
//
// Return the true hidden word to show the player.
string hangman::get_hidden_word() {
    return "hangman";
}

set<int> hangman::get_word_lengths() {
    return word_lengths;
}

bool hangman::find_word_length(int len) {
    if(word_lengths.find(len)!=word_lengths.end())
        return true;
    else
        return false;
}

void hangman::set_word_list(int le) {
    for(auto it: words) {
        if(it.length()==le) {
            word_list.insert(it);
        }
    }
}

/*void hangman::set_guesses_remaining(int g) {
    guesses_remaining = g;
}*/

void hangman::set_word_length(int l) {
    word_length = l;
}

void hangman::set_current_guess(char c) {
    current_guess = c;
}

void hangman::set_words_remaining(bool b) {
    words_remaining = b;
}

int hangman::get_correct_letters() {
    return correct_letters;
}

int hangman::get_word_length() {
    return word_length;
}
