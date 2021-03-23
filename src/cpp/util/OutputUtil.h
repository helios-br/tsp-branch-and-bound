#ifndef OUTPUTUTIL_H
#define OUTPUTUTIL_H

#include <iostream>
#include <string>

inline void nlogn(string text) {
	cout << endl << text << endl;
}

inline void nlog(string text) {
	cout << endl << text;
}

inline void logn(string text) {	
	cout << text << endl;
}

#endif
