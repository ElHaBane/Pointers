/*
El Hadji Omar Bane
Pointers / KorruPto game
7/28/2022
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h> // gives me access to rand
#include <time.h> // gets access to time
#include <iomanip> // allows me to manipulate cin

using namespace std;

// functions that print
void display_introduction(); // tells the user what is going on and how to play the game
void display_stage(vector<string> const words); // evenly displays 9 words on a 3 by 3 plane
void print_spaces(int max_spaces); // prints spaces until it prints the maximum amount it was told to


// functions that manipulate pointers
void apply_corruption(vector<string> *words, vector<float> corruption_percentages); // gets the word based on how corrupted it should be
void increase_corruption_percentage(const vector<string> words, vector<float>* corruption_percentages, vector<float> const ideal_host_dna, int base_increase = 25); // increases the corruption percentage based on the virus' ideal 'traits'
void edit_string(string *str, const int max_string_length, const bool is_adding_chars = true);


// functions that give a value
bool has_char_in_string(char character, const string str); // declares if a characters is found in a string

double get_inverted_value(double x, int highest_value = 1); // gets an inverted value

vector<float> get_word_dna(const string& word, int max_letter_length); // gets two numbers that represents the word's 'traits'
vector<float> get_virus_ideal_host_dna(); // generates a float vector that represents the ideal host for the virus

int get_longest_strings_length(vector<string> strings); // gets the size of the largest string within a string vector
int get_integer_input(int min = INT_MIN, int max = INT_MAX); // gets valid integer inputs


int main() {

	srand(time(NULL)); // initializes random number generation

	//constants
	vector<string> const NAMES { "adductors987654321", "vaporizer2002", "cleveland", "violet", "chicle323", "boogie222222", "boe639", "jet3", "joe" }; // stores the names needed for the game
	vector<float> const virusIdealHostDNA = get_virus_ideal_host_dna(); // stores the ideal host for the virus

	vector<float> namesCorruptionPercentage(NAMES.size(), 0.0); // stores the level of corruption each name has

	// declarations
	int menuInput; // meant to hold the user's menu inputs
	int selectedWordIndex; // meant to hold the selected word's index
	vector<string> displayedNames; // meant to hold a the corrupted version of NAMES
	bool isAddingChar; // stores if the user is adding characters or not
	
	displayedNames = NAMES; // stores the names that will be displayed

	display_introduction(); // shows the introduction
	cout << endl;

	bool running = true;
	while (running) {

		// adjusts and displays the words
		vector<string>* pDisplayedNames = &displayedNames; // points to displayedNames
		apply_corruption(pDisplayedNames, namesCorruptionPercentage); // applies changes to names based on their corruption level
		
		display_stage(displayedNames); // displays

		// user options and input
		cout << "1.add characters\n2.remove characters\n3.pass time\n4.reset words\n5.quit sim" << endl;
		menuInput = get_integer_input(1, 5); // gets the users input for the menu
		cout << endl;

		// results of input 1 / 2
		if (menuInput == 1 || menuInput == 2) {
			isAddingChar = (menuInput == 1); // stores if the user is adding characters or not

			// shows the user's options
			cout << "which name do you want to edit?" << endl;
			for (int i = 0; i < NAMES.size(); i++) {cout << i + 1 << "." << NAMES[i] << endl;} //shows the user all the names they can edit

			selectedWordIndex = get_integer_input(1, NAMES.size()) - 1; // gets the index of the users option
			system("CLS");

			// editing words
				string* pName = &displayedNames[selectedWordIndex];
				edit_string(pName, get_longest_strings_length(NAMES), isAddingChar);
				system("pause");
		}
		if (menuInput == 3) {
			vector<float> *pCorruptionPercents = &namesCorruptionPercentage;
			increase_corruption_percentage(NAMES, pCorruptionPercents, virusIdealHostDNA);
		}
		if (menuInput == 4) {
			namesCorruptionPercentage = vector<float>(NAMES.size(), 0.0);
			displayedNames = NAMES;
		}
		if (menuInput == 5) { running = false; }
		system("CLS");
	}

	
	return 0;
}

void display_introduction() {
	cout << "A group of names are infected with a virus. The virus has two stages. In the first stage, the names are warped but can still be helped. On the second stage, the names undergo irreversable changes." << endl;
	cout << "The virus prefers certain traits in it's host, but they are unknown to you. Luckily, you know that the virus cares about two things about their host:\n1.Their length\n2.The amount of numbers they have" << endl;
	cout << "Using your ability to edit the words, you can either help the words become resistant to the virus or you can make them the perfect host for the virus." << endl;
	cout << "You are free to do whatever you like with that knowledge. They're no win conditions. Only experimentations on the the unfortunate names" << endl;
	system("pause");
	return;
} 

void edit_string(string* str, const int max_string_length, const bool is_adding_chars) {
	const int min_string_length = 3; // stores the minimun amount of characters a string can have

	cout << "The word: " << (*str) << endl;
	if (is_adding_chars) {
		const int word_limit = max_string_length - (*str).size(); // stores the amount of characters that can be added
		if (word_limit <= 0) { cout << "The word's length is already at the limit" << endl; }
		
		else {
			string added_string; // declares the string the user will add
			cout << "You can add " << word_limit << " characters. Letters and numbers are allowed.\n Your word:"; // gives the user instruction
			cin >> setw(word_limit) >> added_string;

			for (int letter_i = 0; letter_i < word_limit - (word_limit - added_string.size()); letter_i++) { *str += added_string[letter_i]; } // adds the user's string into the word while obeying the word limit
		}

	}

	else {
		string deleted_characters;

		if ((*str).size() <= min_string_length) { cout << "The string is too short to remove characters" << endl; } // tells the user that the word is too short

		else {
			cout << "You can delete: "<< (*str).size() - min_string_length << " Letters.\nInput what characters you would like to delete. Type the characters as if they are one word. The last inputted characters are deleted first\nYour input:" << endl;

			cin >> setw(max_string_length) >> deleted_characters; 
			// deletes characters
			for (int deleted_i = (*str).size() - 1; deleted_i > 0 && (*str).size() > 3; deleted_i--) { // looks through the word as long as it is greater than 3
				if ((*str)[deleted_i] != '*' && has_char_in_string((*str)[deleted_i], deleted_characters)) { (*str).erase((*str).begin() + deleted_i); } // deletes character if user wants it deleted and the word isn't too corrupted 
			}	
		}
	}
	cout << "The word is now: " << *str << endl;
	return;
}

void apply_corruption(vector<string> *words, vector<float> corruption_percentages) {
	vector<string> nubmers{ "1234567890" }; // stores numbers in the form of characters

	float word_corruption_percent;
	string *p_current_word;

	int stage_one_clc; // clc = corruption letter count. tracks how many letters are affected by stage one corruption
	int stage_two_clc;; // tracks how many letters are affected by stage two corruption

	// loops through and applies the corruption to the words
	for (int wordi = 0; wordi < (*words).size(); wordi++) { // loops through all the words in the "words" vector
		
		p_current_word = &(*words)[wordi]; // stores a word's address
		word_corruption_percent = corruption_percentages[wordi]; // stores a word's corruption percentage

		// applies stage one of the corruption
		stage_one_clc = floor((word_corruption_percent / 50) * (*p_current_word).size()); // stores how many stage one words make up the word
		for (int letteri = 0; letteri < (*p_current_word).size(); letteri++) {
			if (stage_one_clc > letteri) { // if stage one corruption affects the letter
				if (has_char_in_string((*p_current_word)[letteri], "1234567890")) { (*p_current_word)[letteri] = '#'; } // replaces corrupted numbers with '#'
				else { (*p_current_word)[letteri] = char(toupper((*p_current_word)[letteri])); } // capitalizes corrupted letters
			}
		}
		
		// applies stage two corruption
		if(word_corruption_percent > 50) { // if corruption is greater that 50%
			stage_two_clc = floor((word_corruption_percent / 100) * (*p_current_word).size()); // stores how many stage one words make up the word			
			for (int letteri = 0; letteri < (*p_current_word).size(); letteri++) {
				if (stage_two_clc > letteri) { // if stage two corruption affects the letter
					(*p_current_word)[letteri] = '*';
				}
			}
		}
	}
	return;
}

void increase_corruption_percentage(const vector<string> words, vector<float>* corruption_percentages, vector<float> const ideal_host_dna, int base_increase) {
	int max_word_length = get_longest_strings_length(words);
	double virus_immunity;

	for (int i = 0; i < words.size(); i++) {
		vector<float> words_dna = get_word_dna(words[i], max_word_length); // stores the dna of the word

		virus_immunity = (abs(words_dna[0] - ideal_host_dna[0]) + abs(words_dna[1] - ideal_host_dna[1])) / 2; // finds the difference between the word's dna and the dna virus' ideal host

		(*corruption_percentages)[i] += get_inverted_value(virus_immunity) * base_increase; // adds the virus' affect on a word to the word's total corruption percentage
	}
	return;
}

double get_inverted_value(double x, int highest_value)
{
	return abs(highest_value - x);
}

vector<float> get_virus_ideal_host_dna(){
	return { float(rand() % 11) / 10, float(rand() % 11) / 10 };
}

int get_longest_strings_length(vector<string> strings)
{
	int largest_size = 0;
	for (auto s = strings.begin(); s < strings.end(); s++) {
		if ((*s).size() > largest_size) { largest_size = int((*s).size()); }
	}
	return largest_size;
}

int get_integer_input(int min, int max){
	int input = 0;

	cin >> setw(1) >> input;

	while (cin.good() == false || input < min || input > max) {// if input was not valid
		cout << "invalid input. Try again" << endl;

		// clears stream
		cin.clear();
		cin.ignore(INT_MAX, '\n');

		// retries retreiving input
		cin >> setw(1) >> input;
	}

	return input;
}

vector<float> get_word_dna(const string& word, int max_letter_length)
{
	float letter_count = 0.0; // stores the number of letters within the word

	for (int i = 0; i < word.size(); i++) { if (has_char_in_string(word[i], "abcdefghijklmnopqrstuvwxyz")) { letter_count++; } } // finds the amount of letters within the word

	float letters_in_max = letter_count / max_letter_length;
	float numbers_in_word = (word.size() - letter_count) / word.size();

	return { letters_in_max, numbers_in_word * 2 };
}

bool has_char_in_string(const char character, const string str) {
	if ((str.find(character)) != string::npos) { return true; }
	return false;
}

void display_stage(vector<string> const words) {
	const int MAX_LENGTH = 100; // the maximum length the stage expands
	const int SPACES_BETWEEN_ROWS = 3;

	// draws the stage
	cout << "\n\n\n\n\n";
	for(int column = 0; column < floor(words.size() / 3); column++) {
		int spaces_used = 0; // stores the amount of spaces already printed

		// stores the first three words of the parameter
		const string& word0 = words[0 + (3 * column)];
		const string& word1 = words[1 + (3 * column)];
		const string& word2 = words[2 + (3 * column)];

		float mid_point = float((word0.size() + (MAX_LENGTH - word2.size() + 1)) / 2); // stores the mid point between word0's end and word1's begining

		print_spaces(4);
		cout << word0; // prints the first word of a column
		print_spaces(int(floor(mid_point - (word1.size() / 2) - 1) - word0.size())); // makes enough spaces to make word1 the center of word0 and word1
		cout << word1;

		print_spaces(int(MAX_LENGTH - word2.size() - floor(mid_point + (word1.size() / 2) + 1))); // makes enough space for word2 to print while obeying the length limit
		cout << word2;

		for (int empty_row = 0; empty_row < SPACES_BETWEEN_ROWS; empty_row++) { cout << endl; } // creates spaces between rows
	}
	return;
}

void print_spaces(int max_spaces) {
	for (int space = 0; space < max_spaces; space++) { cout << " "; }
}
