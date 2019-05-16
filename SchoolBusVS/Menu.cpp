#include "Menu.h"

unsigned int Menu::DefaultColor = LIGHTGRAY;
unsigned int Menu::CurrentColor = LIGHTGRAY;

void Menu::setDefaultColor(unsigned int color) {
	Menu::DefaultColor = color;
}

ostream & Menu::printHeader(const string& title) {
	ostringstream oss;
	oss << "---- " << title << " -----" << endl;
	displayColored(oss.str(), LIGHTRED);
	return cout;
}

ostream & Menu::printTitle(const string& title, char fill) {
	ostringstream oss;
	size_t length = title.size();
	oss << setw(length + 8) << setfill(fill) << "" << endl;
	oss << fill << fill << "  " << title << "  " << fill << fill << endl;
	oss << setw(length + 8) << setfill(fill) << "" << endl;
	oss << setfill(' ');
	displayColored(oss.str(), WHITE);
	return cout;
}

void Menu::getLineInput_CI(const string & message, string & val, const vector<string>& validInput) {
	while(1) {
		cout << message;
		getline(cin, val);
		clearEOF();
		toUpper(val);
		for (size_t i = 0; i < validInput.size(); i++) {
			if (stringCICompare::equal(validInput[i], val))
				return;
		}
		cout << "Invalid input! ";				
	} 
}

void Menu::getLineInput(const string& message, string& val) {
	cout << message;
	getline(cin, val);
	clearEOF();
}

void Menu::getLineInput(const string& message, string& val, const vector<string>& validInput) {
	clearBadInput();
	while (1) {
		cout << message;
		getline(cin, val);
		clearEOF();
		if (find(validInput.begin(), validInput.end(), val) != validInput.end())
			return;
		cout << "Invalid input! ";
	}
}

void Menu::clearBadInput() {
	if (cin.eof())
		cin.clear();
	else {
		cin.clear();
		cin.ignore(10000, '\n');
	}
}

void Menu::clearEOF() {
	if (cin.eof())
		cin.clear();
}

void Menu::clearConsole() {
	COORD coordScreen = { 0, 0 }; // upper left corner
	DWORD cCharsWritten;
	DWORD dwConSize;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hCon, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	// fill with spaces
	FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hCon, &csbi);
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	// cursor to upper left corner
	SetConsoleCursorPosition(hCon, coordScreen);
}

unsigned int Menu::getCurrentColor() {
	return CurrentColor;
}

unsigned int Menu::getDefaultColor() {
	return DefaultColor;
}

void Menu::setTextColor(unsigned int color) {
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
	CurrentColor = color;
}

ostream & Menu::displayNoun(unsigned int count, const string & singular) {
	if (count == 1)
		cout << singular;
	else cout << singular + "s";
	return cout;
}

ostream & Menu::displayNoun(unsigned int count, const string & singular, const string & plural) {
	if (count == 1)
		cout << singular;
	else cout << plural;
	return cout;
}

