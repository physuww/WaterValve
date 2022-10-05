#ifndef CONTAINERS_H_
#define CONTAINERS_H_

#include "Arduino.h"
#include "LiquidCrystal.h"

#define MAX_STR_LEN 8

template<typename T, int SZ> struct vector {

	int8_t n;
	T data[SZ];

	vector() : n(0) {}
	vector(uint8_t n) : n(n) {}
	vector& operator= (vector& other) {
		n = other.n;
		for(uint8_t i = 0; i < n; ++ i)
			data[i] = other.data[i];
		return *this;
	}
	bool operator== (vector& other) {
		if(n != other.n)
			return false;
		for(uint8_t i = 0; i < n; ++ i)
			if(data[i] != other.data[i])
				return false;
		return true;
	}
	bool operator!= (vector& other) {
		if(n != other.n)
			return true;
		for(uint8_t i = 0; i < n; ++ i)
			if(data[i] != other.data[i])
				return true;
		return false;
	}
	T& operator[] (uint8_t i) {
		if(i < n)
			return data[i];
		else
			return data[n - 1];
	}
	void push_back(T el){
		if(n == SZ)
			return;
		data[n] = el;
		++ n;
	}
	void pop_back() {
		if(n == 0)
			return;
		-- n;
	}
	void clear() {
		n = 0;
	}

};

struct MyString : vector<uint8_t, MAX_STR_LEN> {

	void print(LiquidCrystal lcd) {

		for(uint8_t i = 0; i < n; i ++)
			lcd.write(data[i]);

	}
};

template<typename T1, typename T2> struct pair {

	T1 first;
	T2 second;

	pair(T1 t1, T2 t2) : first(t1), second(t2) {}
	pair& operator= (pair& other){
		first = other.first;
		second = other.second;
		return *this;
	}
	bool operator== (pair& other) {
		return first == other.first && second == other.second;
	}
	bool operator!= (pair& other) {
		return first != other.first || second != other.second;
	}
	bool operator> (pair& other) {
		return first > other.first || (first == other.first && second > other.second);
	}
	bool operator< (pair& other) {
		return first < other.first || (first == other.first && second < other.second);
	}


};

#endif /* CONTAINERS_H_ */
