
#pragma once

#include <iostream>

#include <vector>
#include <string>

using std::vector;
using std::string;


using std::string;

int getRawKey();

int checkForSpecials(string userInput);

void youEnteredFunc(string userInput);

string trim(const string s);

string read_line(vector<string>);

void arrow_manipulate(string& currentInput);
