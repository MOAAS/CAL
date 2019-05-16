#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 
#endif

#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <windows.h>
#include <direct.h>


/** @defgroup utilities utilities
 * @{
 *
 * Diverse utility functions
 */

// Searching and sorting

/**
 * @brief Function objects used to compare pointers 
 * 
 */
namespace comparePtr {
	/**
	 * @brief Function object used to check if two pointers point to an equal value
	 * 
	 * @tparam ValPtr Pointer to a value that overloads the = operator
	 */
	template<typename ValPtr>
	struct equal {
		/**
		 * @brief Checks if the pointed values are equal
		 * 
		 * @param lhs Pointer to value
		 * @param rhs Pointer to value
		 * @return true lhs and rhs point to equal values
		 * @return false lhs and rhs do not point to equal values
		 */
		bool operator() (const ValPtr& lhs, const ValPtr& rhs) const { *lhs == *lhs; }
	};

	/**
	 * @brief Function object used to check if one pointer points to a smaller value than another.
	 * 
	 * @tparam ValPtr Pointer to a value that overloads the < operator
	 */
	template<typename ValPtr>
	struct lessThan {
		/**
		 * @brief Checks if one pointed value is smaller than another.
		 * 
		 * @param lhs Pointer to value
		 * @param rhs Pointer to value
		 * @return true lhs points to a smaller value than rhs
		 * @return false lhs does not point to a smaller value than rhs
		 */
		bool operator() (const ValPtr& lhs, const ValPtr& rhs) const { return *lhs < *rhs; }
	};
}

/**
 * @brief Functions that print vectors (of values or pointers) in a human-friendly way
 * 
 */
namespace printVector {
	/**
	 * @brief Prints a vector of values in a human friendly way
	 * 
	 * @tparam Printable Object type that overloads the << operator
	 * @param os Output stream to which the values will be printed
	 * @param vec Vector whose values will be printed
	 * @param sep Separator to insert inbetween the values
	 */
	template<class Printable>
	inline std::ostream & ofValues(std::ostream & os, const std::vector<Printable> &vec, std::string sep = "\n") {
		if (vec.size() == 0)
			return os;
		for (size_t i = 0; i < vec.size() - 1; i++) {
			os << vec[i] << sep;
		}
		os << vec[vec.size() - 1];
		return os;
	}

	/**
	 * @brief Prints a vector of pointers in a human friendly way
	 * 
	 * @tparam PrintablePtr Pointer to an object type that overloads the << operator
	 * @param os Output stream to which the values will be printed
	 * @param vec Vector whose values will be printed
	 * @param sep Separator to insert inbetween the values
	 */
	template<class PrintablePtr>
	inline std::ostream & ofPointers(std::ostream & os, const std::vector<PrintablePtr> &vec, std::string sep = "\n") {
		if (vec.size() == 0)
			return;
		for (size_t i = 0; i < vec.size() - 1; i++) {
			os << *vec[i] << sep;
		}
		os << *vec[vec.size()-1];
		return os;
	}
}

/**
 * @brief Functions that insert in a vector, values or pointers, in a sorted way.
 * 
 */
namespace insert_sorted {
	/**
	 * @brief Inserts a value in a vector, in a sorted way.
	 * 
	 * @tparam Comparable Class that overloads the < operator
	 * @param vec Vector to which the object will be inserted
	 * @param item Value to be inserted
	 * @return std::vector<Comparable>::iterator Position where the value was inserted
	 */
	template<class Comparable>
	inline typename std::vector<Comparable>::iterator value(std::vector<Comparable> &vec, Comparable item) {
		auto iter = lower_bound(vec.begin(), vec.end(), item);
		if (iter == vec.end())
			vec.push_back(item);
		else vec.insert(iter, item);
		return iter;
	}

	/**
	 * @brief Inserts a pointer in a vector, in a sorted way.
	 * 
	 * @tparam ComparablePtr Pointer to class that overloads the < operator
	 * @param vec Vector to which the pointer will be inserted
	 * @param item Pointer to be inserted
	 * @return std::vector<ComparablePtr>::iterator Position where the pointer was inserted
	 */
	template<class ComparablePtr>
	inline typename std::vector<ComparablePtr>::iterator pointer(std::vector<ComparablePtr> &vec, ComparablePtr item) {
		auto iter = lower_bound(vec.begin(), vec.end(), item, comparePtr::lessThan<ComparablePtr>());
		if (iter == vec.end())
			vec.push_back(item);
		else vec.insert(iter, item);
		return iter;
	}
}

/**
 * @brief Functions used to find values in unordered vectors
 * 
 */
namespace unorderedVector {
	/**
	 * @brief Looks for a value in an unordered vector
	 * 
	 * @tparam T Value type
	 * @param vec Vector that will be looked at
	 * @param item Item that will be looked for
	 * @return std::vector<T>::const_iterator The position of the value, or vec.end() if it wasn't found
	 */
	template<class T>
	inline typename std::vector<T>::const_iterator findValue(const std::vector<T> &vec, const T& item) {
		return find(vec.begin(), vec.end(), item);
	}

	/**
	 * @brief Looks for a pointer to a value in an unordered vector
	 * 
	 * @tparam T_Ptr Pointer to a value type
	 * @param vec Vector that will be looked at
	 * @param item Pointer to the item that will be looked for
	 * @return std::vector<T_Ptr>::const_iterator The position of the pointer to the value, or vec.end() if it wasn't found
	 */
	template<class T_Ptr>
	inline typename std::vector<T_Ptr>::const_iterator findPointer(const std::vector<T_Ptr> &vec, const T_Ptr item) {
		for (size_t i = 0; i < vec.size(); i++) {
			if (*vec[i] == *item)
				return vec.begin() + i;
		}
		return vec.end();
	}

	/**
	 * @brief Looks for a value in an unordered vector
	 * 
	 * @tparam T Value type
	 * @param vec Vector that will be looked at
	 * @param item Item that will be looked for
	 * @return true Found it
	 * @return false Didn't find it
	 */
	template<class T>
	inline bool hasValue(const std::vector<T> &vec, const T& item) {
		return find(vec.begin(), vec.end(), item) != vec.end();
	}

	/**
	 * @brief Looks for a pointer to a value in an unordered vector
	 * 
	 * @tparam T_Ptr Pointer to a value type
	 * @param vec Vector that will be looked at
	 * @param item Pointer to the item that will be looked for
	 * @return true Found it 
	 * @return false Didn't find it
	 */
	template<class T_Ptr>
	inline bool hasPointer(const std::vector<T_Ptr> &vec, const T_Ptr item) {
		for (size_t i = 0; i < vec.size(); i++) {
			if (*vec[i] == *item)
				return true;
		}
		return false;
	}
}

/**
 * @brief Functions used to find values in ordered vectors
 * 
 */
namespace orderedVector {
	/**
	 * @brief Looks for a value in an ordered vector
	 * 
	 * @tparam Comparable Value type that overloads the < operator
	 * @param vec Vector that will be looked at
	 * @param item Item that will be looked for
	 * @return std::vector<Comparable>::const_iterator The position of the value, or vec.end() if it wasn't found
	 */
	template<class Comparable>
	inline typename std::vector<Comparable>::const_iterator findValue(const std::vector<Comparable> &vec, const Comparable& item) {
		if (binary_search(vec.begin(), vec.end(), item))
			return lower_bound(vec.begin(), vec.end(), item);
		return vec.end();
	}

	/**
	 * @brief Looks for a pointer to a value in an ordered vector
	 * 
	 * @tparam ComparablePtr Pointer to a value type that overloads the < operator
	 * @param vec Vector that will be looked at
	 * @param item Pointer to the item that will be looked for
	 * @return std::vector<ComparablePtr>::const_iterator The position of the pointer to the value, or vec.end() if it wasn't found
	 */
	template<class ComparablePtr>
	inline typename std::vector<ComparablePtr>::const_iterator findPointer(const std::vector<ComparablePtr> &vec, ComparablePtr item) {
		if (binary_search(vec.begin(), vec.end(), item, comparePtr::lessThan<ComparablePtr>()))
			return lower_bound(vec.begin(), vec.end(), item, comparePtr::lessThan<ComparablePtr>());
		return vec.end();
	}

	/**
	 * @brief Looks for a value in an ordered vector
	 * 
	 * @tparam Comparable Value type that overloads the < operator
	 * @param vec Vector that will be looked at
	 * @param item Item that will be looked for
	 * @return true Found it
	 * @return false Didn't find it
	 */
	template<class Comparable>
	inline bool hasValue(const std::vector<Comparable> &vec, const Comparable& item) {
		return binary_search(vec.begin(), vec.end(), item);
	}

	/**
	 * @brief Looks for a pointer to a value in an ordered vector
	 * 
	 * @tparam ComparablePtr Pointer to a value type that overloads the < operator
	 * @param vec Vector that will be looked at
	 * @param item Pointer to the item that will be looked for
	 * @return true Found it 
	 * @return false Didn't find it
	 */
	template<class ComparablePtr>
	inline bool hasPointer(const std::vector<ComparablePtr> &vec, ComparablePtr item) {
		return binary_search(vec.begin(), vec.end(), item, comparePtr::lessThan<ComparablePtr>());
	}

}


/**
 * @brief Sorts a vector of pointers, considering the values that are pointed by them.
 * 
 * @tparam ComparablePtr Pointer to a value type that overloads the < operator
 * @param vec Vector to be sorted
 */
template<class ComparablePtr>
inline void sortPointerVector(std::vector<ComparablePtr> &vec) { // Bubble sort <- bubble sort best sort
	sort(vec.begin(), vec.end(), comparePtr::lessThan<ComparablePtr>()); // but quicc-sort better sort
}

/**
 * @brief Functions used to get the difference between two vectors
 * 
 */
namespace subtractVectors {
	/**
	 * @brief Returns all the pointers to elements that are in left but are not in right
	 * 
	 * @tparam ComparablePtr Pointer to a value type overloads the == operator
	 * @param left Vector to be subtracted from
	 * @param right Vector that contains the pointers to the elements that will be subtracted
	 * @return std::vector<ComparablePtr> Result of the differente 
	 */
	template<class ComparablePtr>
	inline typename std::vector<ComparablePtr> ofPointers(std::vector<ComparablePtr>& left, const std::vector<ComparablePtr>& right) {
		std::vector<ComparablePtr> result = left;
		for (int i = (int)result.size() - 1; i >= 0; i--) {
			if (unorderedVector::hasPointer(right, result[i]))
				result.erase(result.begin() + i);
		}
		return result;
	}

	/**
	 * @brief Returns all the elements that are in left but are not in right
	 * 
	 * @tparam Comparable Value type overloads the == operator
	 * @param left Vector to be subtracted from
	 * @param right Vector that contains the pointers to the elements that will be subtracted
	 * @return std::vector<Comparable> Result of the differente 
	 */
	template<class Comparable>
	inline typename std::vector<Comparable> ofValues(const std::vector<Comparable>& left, const std::vector<Comparable>& right) {
		std::vector<Comparable> result = left;
		for (size_t i = result.size() - 1; i > 0; i--) {
			if (unorderedVector::hasValue(right, result[i]))
				result.erase(result.begin() + i);
		}
		return result;
	}
}

// Time measuring 

typedef std::chrono::high_resolution_clock::time_point time_pt;

/**
 * @brief Functions used to measure time
 * 
 */
namespace timeMeasuring {
	/**
	 * @brief Returns a time point corresponding to the current moment.
	 * 
	 * @return time_pt 
	 */
	inline time_pt now() {
		return std::chrono::high_resolution_clock::now();
	}

	/**
	 * @brief Display on the console the time elapsed between two points
	 * 
	 * @param start Starting point
	 * @param end Ending point
	 * @return std::ostream& Returns the std::cout output stream
	 */
	inline std::ostream& printDiff(time_pt start, time_pt end) {
		std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0 << " ms" << std::endl;
		return std::cout;
	}

	/**
	 * @brief Gets the time elapsed between two points
	 * 
	 * @param start Starting point
	 * @param end Ending point
	 * @return double Time between two points, in milisseconds
	 */
	inline double getDiff(time_pt start, time_pt end) {
		return  std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
	}
}

// File stream

/**
 * @brief Deletes a folder, and if specified, all the subfolders
 * 
 * @param refcstrRootDirectory The path containing the folder to be deleted
 * @param bDeleteSubdirectories Set to true if it should remove all the containing files
 * @return int Returns 0 on success, non-zero otherwise
 */
inline int DeleteDirectory(const std::string &refcstrRootDirectory, bool bDeleteSubdirectories = true) {
	bool bSubdirectory = false;					 // Flag, indicating whether
												 // subdirectories have been found
	HANDLE          hFile;                       // Handle to directory
	std::string     strFilePath;                 // Filepath
	std::string     strPattern;                  // Pattern
	WIN32_FIND_DATA FileInformation;             // File information


	strPattern = refcstrRootDirectory + "\\*.*";
	hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
	if (hFile != INVALID_HANDLE_VALUE) {
		do {
			if (FileInformation.cFileName[0] != '.') {
				strFilePath.erase();
				strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;
				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if (bDeleteSubdirectories) {
						// Delete subdirectory
						int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
						if (iRC)
							return iRC;
					}
					else bSubdirectory = true;
				}
				else {
					// Set file attributes
					if (::SetFileAttributes(strFilePath.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE)
						return ::GetLastError();
					// Delete file
					if (::DeleteFile(strFilePath.c_str()) == FALSE)
						return ::GetLastError();
				}
			}
		} while (::FindNextFile(hFile, &FileInformation) == TRUE);
		// Close handle
		::FindClose(hFile);
		DWORD dwError = ::GetLastError();
		if (dwError != ERROR_NO_MORE_FILES)
			return dwError;
		else {
			if (!bSubdirectory) {
				// Set directory attributes
				if (::SetFileAttributes(refcstrRootDirectory.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE)
					return ::GetLastError();
				// Delete directory
				if (::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE)
					return ::GetLastError();
			}
		}
	}

	return 0;
}

/**
 * @brief Helper functions that assist in file reading
 * 
 */
namespace fileRead {
	/**
	 * @brief Gets a value in the specified format: "Type: Value"
	 * Example: "Type: V a l u e" would return "V a l u e"
	 * 
	 * @param file File stream to be read
	 * @param value Variable that will store the value
	 */
	inline void getValue(std::ifstream& file, std::string& value) {
		std::string line;
		std::getline(file, line);
		std::istringstream iss(line);
		std::string temp;
		iss >> temp;
		try {
			value = line.substr(temp.size() + 1, line.size() - temp.size() - 1);
		}
		catch (...) {
			std::cout << "Invalid reading: " << line << std::endl;
			value = "";
		}
	}

	/**
	 * @brief Gets a value in the specified format: "Type: Value"
	 * 
	 * @tparam Number Any number type (int, double, float, etc)
	 * @param file File stream to be read
	 * @param num Variable that will store the value
	 */
	template<class Number>
	void getValue(std::ifstream& file, Number& num) {
		std::string line;
		std::getline(file, line);
		std::istringstream iss(line);
		std::string temp;
		iss >> temp;
		try {
			num = (Number)stod(line.substr(temp.size() + 1, line.size() - temp.size() - 1));
		}
		catch (...) {
			std::cout << "Invalid reading: " << line << std::endl;
		}
	}

	/**
	 * @brief Processes a string into two values. String must be in the format: "Type1: Value1 Separator Type2: Value2"
	 * 
	 * @param line String to be processed
	 * @param val1 Variable that will store the first value
	 * @param val2 Variable that will store the second value
	 */
	inline void getValuePair(std::string& line, std::string &val1, std::string &val2) {
		std::istringstream iss(line);
		std::string temp, sep;
		iss >> temp >> val1 >> sep >> temp >> val2;
	}
}

/**
 * @brief Collects a list of all the files on a directory
 * 
 * @param dirName Directory path
 * @param files Vector that will store the file names
 */
inline void collectAllFiles(const std::string& dirName, std::vector<std::string> &files) {
	std::string pattern(dirName);
	pattern.append("/*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			if ((std::string)data.cFileName == "." || (std::string)data.cFileName == "..")
				continue;
			files.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
}

/**
 * @brief Creates a folder
 * 
 * @param path Respective path
 */
inline void createFolder(const std::string& path) {
	_mkdir(path.c_str());
}

// Math

/**
 * @brief Rounds a number
 * 
 * @param x Number to be rounded
 * @param PRECISION Number of decimal figures of the result
 * @return double Result
 */
inline double round(double x, const unsigned int PRECISION) {
	double tenPower = pow(10, PRECISION);
	return (round(x*tenPower) / tenPower);
}

// Strings

/**
 * @brief Functions used to compare strings, in a case insensitive way
 * 
 */
namespace stringCICompare {
	/**
	 * @brief Checks if two strings are equal (Case insensitive)
	 * 
	 * @param str1 String1
	 * @param str2 String2
	 * @return true String1 and String2 are equal (Case insensitive)
	 * @return false String1 and String2 are not equal (Case insensitive)
	 */
	inline bool equal(const std::string& str1, const std::string& str2) {
		size_t size = str1.size();
		if (str2.size() != size)
			return false;
		for (unsigned int i = 0; i < size; ++i)
			if (tolower(str1[i]) != tolower(str2[i]))
				return false;
		return true;
	}

	/**
	 * @brief Checks if one string is smaller than another one (Case insensitive)
	 * 
	 * @param str1 String1
	 * @param str2 String2
	 * @return true String1 is smaller than String2 (Case insensitive)
	 * @return false String1 is not smaller than String2 (Case insensitive)
	 */
	inline bool lessThan(const std::string& str1, const std::string& str2) {
		size_t size = str1.size();
		if (str2.size() != size)
			return false;
		for (unsigned int i = 0; i < size; ++i)
			if (tolower(str1[i]) > tolower(str2[i]))
				return false;
			else if (tolower(str1[i]) < tolower(str2[i]))
				return true;
		return false;
	}
}

/**
 * @brief Converts a string to uppercase
 * 
 * @param str string to be converted
 */
inline void toUpper(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

/**
 * @brief Converts a string to lowercase
 * 
 * @param str string to be converted
 */
inline void toLower(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

/**
 * @brief Checks if a string is numeric
 * 
 * @param str String to be checked
 * @return true String is numeric
 * @return false String is not numeric
 */
inline bool isNumeric(const std::string& str) {
	return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

/**
 * @brief Converts a string to an integer, if numeric.
 * 
 * @param str String to convert.
 * @return int Returns the integer, if numeric string, 0 otherwise.
 */
inline int strToInt(const std::string& str) {
	if (isNumeric(str))
		return 0;
	return stoi(str);
}

/**
 * @brief Returns the ordinal number corresponding to a certain integer
 * Example: "1" returns "1st"
 * 
 * @param num Number to be converted
 * @return std::string Ordinal number, in a string format
 */
inline std::string getOrdinal(unsigned int num) {
	int units = num % 10;
	int tens = (num / 10) % 10;
	if (tens == 1)
		return std::to_string(num) + "th";
	else switch (units) {
	case 1: return std::to_string(num) + "st";
	case 2: return std::to_string(num) + "nd";
	case 3: return std::to_string(num) + "rd";
	default: return std::to_string(num) + "th";
	}
}

// Codes

/**
 * @brief Looks for a code in an ordered vector of pointers to coded objects.
 * A coded object is an object that has a code member, therefore, one coded object is smaller than another if its code is smaller.
 * 
 * @tparam CodedObject Object that has the member function getCode(), that returns a string. Must also have the < operator overloaded, which should simply compare the codes of two objects.
 * @param objects Vector that contains a list of coded objects
 * @param code Code that will be matched
 * @return CodedObject* The respective object, if it was found, NULL otherwise.
 */
template <class CodedObject>
CodedObject* getOrderedObjectByCode(const std::vector<CodedObject*>& objects, const std::string& code) {
	size_t first = 0, count = objects.size(), middle, step;
	while (count > 0) {
		middle = first; step = count / 2; middle = middle + step;
		if (objects[middle]->getCode() < code) {
			first = ++middle;
			count -= step + 1;
		}
		else count = step;
	}
	if (first == objects.size() || !stringCICompare::equal(objects[first]->getCode(), code))
		return NULL;
	else return objects[first];
}

/**
 * @brief Looks for a code in an unordered vector of pointers to coded objects.
 * A coded object is an object that has a code member.
 * 
 * @tparam CodedObject Object that has the member function getCode(), that returns a string.
 * @param objects Vector that contains a list of coded objects
 * @param code Code that will be matched
 * @return CodedObject* The respective object, if it was found, NULL otherwise.
 */
template <class CodedObject>
CodedObject* getObjectByCode(const std::vector<CodedObject*>& objects, const std::string& code) {
	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i]->getCode() == code)
			return objects[i];
	}
	return NULL;
}

// Polymorphism

/**
 * @brief Check if a class is derived from another class
 * 
 * @tparam Base Base class
 * @tparam Derived Derived class
 * @param base Pointer to the base class
 * @return true base is indeed a pointer to Derived 
 * @return false base is not a pointed to Derived
 */
template<class Base, class Derived>
bool isDerived(Base* base) {
	Derived* derived = dynamic_cast<Derived*>(base);
	return derived != NULL;
}

/**
 * @brief Gets the Derived pointer to a base class pointer
 * 
 * @tparam Base Base class
 * @tparam Derived Derived class
 * @param base Pointer to the base class
 * @return Derived* Pointer to the same object, but to the derived class
 */
template<class Base, class Derived>
Derived* getDerived(Base* base) {
	return dynamic_cast<Derived*>(base);
}

