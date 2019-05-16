#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "utilities.h"
using namespace std;

constexpr unsigned int BLACK = 0;
constexpr unsigned int BLUE = 1;
constexpr unsigned int GREEN = 2;
constexpr unsigned int CYAN = 3;
constexpr unsigned int RED = 4;
constexpr unsigned int MAGENTA = 5;
constexpr unsigned int BROWN = 6;
constexpr unsigned int LIGHTGRAY = 7;
constexpr unsigned int DARKGRAY = 8;
constexpr unsigned int LIGHTBLUE = 9;
constexpr unsigned int LIGHTGREEN = 10;
constexpr unsigned int LIGHTCYAN = 11;
constexpr unsigned int LIGHTRED = 12;
constexpr unsigned int LIGHTMAGENTA = 13;
constexpr unsigned int YELLOW = 14;
constexpr unsigned int WHITE = 15;

/**
 * @brief Class used to facilitate the creation of a console interface, using the cout and cin streams from the STL. Only has static methods and variables.
 * 
 */
class Menu {
public:
	~Menu() {};
	/**
	 * @brief Displays a message in a certain color
	 * 
	 * @tparam Printable Type that overloads the << operator
	 * @param message Message to be printed
	 * @param color Color for the message to be printed in
	 * @return ostream& Output stream
	 */
	template<class Printable>
	static ostream& displayColored(const Printable& message, unsigned int color);

	/**
	 * @brief Prints a header on the console, in a red color.
	 * 
	 * @param title Header to be printed
	 * @return ostream& Returns cout
	 */
	static ostream & printHeader(const string& title);
	/**
	 * @brief Prints a title on the console (more noticeable header), in a white color.
	 * 
	 * @param title Title to be printed
	 * @param fill Character to be used as a fill to make the title more noticeable
	 * @return ostream& Returns cout
	 */
	static ostream & printTitle(const string& title, char fill);

	/**
	 * @brief Displays a message and gets line input, only accepting a certain set of strings (Case insensitive)
	 * 
	 * @param message Message to be displayed.
	 * @param val String where the value inserted will be stored.
	 * @param validInput Vector of valid inputs (Case insensitive)
	 */
	static void getLineInput_CI(const string& message, string& val, const vector<string>& validInput);

	/**
	 * @brief Displays a message and gets line input.
	 * 
	 * @param message Message to be displayed.
	 * @param val String where the value inserted will be stored.
	 */
	static void getLineInput(const string& message, string& val);

	/**
	 * @brief Displays a message and gets line input, only accepting a certain set of strings (Case sensitive)
	 * 
	 * @param message Message to be displayed.
	 * @param val String where the value inserted will be stored.
	 * @param validInput Vector of valid inputs (Case sensitive)
	 */
	static void getLineInput(const string& message, string& val, const vector<string>& validInput);

	/**
	 * @brief Clears any possible bad input to the cin stream.
	 * 
	 */
	static void clearBadInput();

	/**
	 * @brief Clears End-of-File input.
	 * 
	 */
	static void clearEOF();

	/**
	 * @brief Clears the console.
	 * 
	 */
	static void clearConsole();

	/**
	 * @brief Gets the color that's currently being used for displaying text.
	 * 
	 * @return unsigned int Returns the color that's currently being used for displaying text.
	 */
	static unsigned int getCurrentColor();

	/**
	 * @brief Gets the default text color.
	 * 
	 * @return unsigned int Returns the default text color.
	 */
	static unsigned int getDefaultColor();

	/**
	 * @brief Sets the color to be used to display text.
	 * 
	 * @param color Color to be used
	 */
	static void setTextColor(unsigned int color);

	/**
	 * @brief Sets the default text color.
	 * 
	 * @param color Color to be used
	 */
	static void setDefaultColor(unsigned int color);

	/**
	 * @brief Displays a noun, adds an 's' if the count is different from 1.
	 * 
	 * @param count Number of items.
	 * @param singular Singular form of the noun
	 * @return ostream& Returns cout
	 */
	static ostream & displayNoun(unsigned int count, const string& singular);

	/**
	 * @brief Displays a noun, whose plural form is different from adding an 's'.
	 * 
	 * @param count Number of items.
	 * @param singular Singular form of the noun
	 * @param plural Plural form of the noun.
	 * @return ostream& Returns cout.
	 */
	static ostream & displayNoun(unsigned int count, const string& singular, const string& plural);

	/**
	 * @brief Displays an exception as an error on the screen, by calling its ::what() method.
	 * 
	 * @tparam Exception Class Object that returns a string upon calling its ::what() method.
	 * @param e Exception to be displayed.
	 */
	template<class Exception>
	static void displayError(Exception &e);

	/**
	 * @brief Gets a code input, through the cin stream, and searches for its respective object in a list.
	 * 
	 * @tparam CodedObject Class type that has a ::getCode() method, which should return a string.
	 * @param objects Vector of Coded Objects.
	 * @param objectName String describing the type of object that  the code represents
	 * @return CodedObject* Returns the object.
	 */
	template<class CodedObject>
	static CodedObject* getObjectInput(const vector<CodedObject*>& objects, const string& objectName);
	
	/**
	 * @brief Gets a code input, through the cin stream, and searches for its respective object in an ordered list.
	 * 
	 * @tparam CodedObject Class type that has a ::getCode() method, which should return a string.
	 * @param objects Vector of Objects, ordered by their code.
	 * @param objectName String describing the type of object that  the code represents
	 * @return CodedObject* Returns the object.
	 */
	template<class CodedObject>
	static CodedObject* getOrderedObjectInput(const vector<CodedObject*>& objects, const string& objectName);

	/**
	 * @brief Displays a message and gets input.
	 * 
	 * @tparam T Value type that overloads the >> operator.
	 * @param message Message to be displayed.
	 * @param val Variable where the inserted value will be stored.
	 */
	template<class T>
	static void getInput(const string& message, T& val);

	/**
	 * @brief Displays a message and gets input.
	 * 
	 * @tparam T Value type that overloads the >> operator.
	 * @param message Message to be displayed.
	 * @param val Variable where the inserted value will be stored.
	 * @param validInput Vector of valid inputs (Case sensitive)
	 */
	template<class T>
	static void getInput(const string& message, T& val, const vector<T>& validInput);

	/**
	 * @brief Displays a message and gets a number input, if it's within defined boundaries.
	 * 
	 * @tparam Number Value type that can be represented as a number.
	 * @param message Message to be displayed.
	 * @param val Variable where the inserted value will be stored.
	 * @param min Minimum value for the input
	 * @param max Maximum value for the input
	 */
	template<class Number>
	static void getInput(const string& message, Number& val, Number min, Number max);
protected:
	/** @brief Default color for the menu output  */
	static unsigned int DefaultColor;
	/** @brief Current color for the menu output  */
	static unsigned int CurrentColor;
};

template<class Printable>
ostream& Menu::displayColored(const Printable& message, unsigned int color) {
	unsigned int prevColor = CurrentColor;
	setTextColor(color);
	cout << message;
	setTextColor(prevColor);
	return cout;
}

template<class Exception>
inline void Menu::displayError(Exception & e) {
	cout << "Error: " << e.what() << endl;
}

template<class CodedObject>
inline CodedObject * Menu::getObjectInput(const vector<CodedObject*>& objects, const string& objectName) {
	string code;
	CodedObject* object;
	if (objects.size() == 0)
		return NULL;
	do {
		getInput("Pick " + objectName + " code: ", code);
		object = getObjectByCode(objects, code);
	} while (!object);
	return object;
}

template<class CodedObject>
inline CodedObject * Menu::getOrderedObjectInput(const vector<CodedObject*>& objects, const string & objectName) {
	string code;
	CodedObject* object;
	if (objects.size() == 0)
		return NULL;
	do {
		getInput("Pick " + objectName + " code: ", code);
		object = getOrderedObjectByCode(objects, code);
	} while (!object);
	return object;
}

template<class T>
inline void Menu::getInput(const string & message, T & val) {
	cout << message;
	while (!(cin >> val)) {
		cout << "Invalid input! " << message;
		clearBadInput();
	}
	clearBadInput();
}

template<class T>
inline void Menu::getInput(const string & message, T & val, const vector<T>& validInput) {
	cout << message;
	while (!(cin >> val) || find(validInput.begin(), validInput.end(), val) == validInput.end()) {
		cout << "Invalid input! " << message;
		clearBadInput();
	}
	clearBadInput();
}

template<class Number>
inline void Menu::getInput(const string & message, Number & val, Number min, Number max) {
	cout << message;
	while (!(cin >> val) || val < min || val > max) {
		cout << "Invalid input! " << message;
		clearBadInput();
	}
	clearBadInput();
}
