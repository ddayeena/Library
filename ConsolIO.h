#pragma once
#include <iostream>
#include <string>
using namespace std;
class ConsolIO {
public:
	static int GetValue(std::istream& is) {
		int value;
		while (true) {
			is >> value;
			if (is.peek() == '\n') {
				is.get();
				break;
			}
			else {
				std::cout << "\n\tInvalid input...Number is expected...Try again: ";
				is.clear();
				while (is.get() != '\n') {};
			}
		}
		return value;
	}	
    static string GetText(std::istream& is) {
        string text;
        while (true) {
            getline(is, text);
            // перевіряємо, чи рядок не порожній і чи не містить пробілів
            bool validInput = true;
			for (char c : text) {
				if (c == ' ') {
					std::cout << "\n\tText must not contain spaces...";
					validInput = false;
					break;
				}
			}
            if (!text.empty() && validInput) 
                break;
			std::cout << "\n\tIncorrect input...Try again:  ";
        }
        return text;
    }
};