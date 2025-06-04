#ifndef __HEXER_H__
#define __HEXER_H__

#pragma once

//#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

class Hexer
{
private:
	char value(char) const;
	char ascii(char) const;
	char* ptr;
	int len;

public:
	friend Hexer operator ^(const Hexer&, const Hexer&);
	friend Hexer operator &(const Hexer&, const Hexer&);
	friend Hexer operator |(const Hexer&, const Hexer&);
	friend Hexer operator +(const Hexer&, const Hexer&);
	friend bool operator ==(const Hexer&, const Hexer&);
	friend ostream& operator << (ostream&, const Hexer&);
	friend istream& operator >> (istream&, Hexer&);
	Hexer():ptr(0),len(0) {}		// default constructor
	Hexer(const string x);
	Hexer(const Hexer& x);
	~Hexer() { delete [] ptr; }	// noisy destructor
	void set(const char* hexstring, size_t byte_count);
	char* detach(int&);
	string toString() const;
	int copyContent (char* buffer, int buf_len) const;
	void pack();
	void unpack();
	Hexer& operator =(const Hexer&);
	Hexer operator ~();
	int getlength() const { return len; }
	void setLength(int length, char padding);
};

inline bool operator !=(const Hexer& l, const Hexer& r) { return !(l == r); }
inline Hexer::Hexer(const Hexer& x):ptr(0),len(0) { set(x.ptr, x.len); }
inline Hexer& Hexer::operator =(const Hexer& x)
{
	if (*this != x) set(x.ptr, x.len);
	return *this;
}

ostream& operator << (ostream& os, const Hexer& x);
istream& operator >> (istream& is, Hexer& x);
Hexer operator ^(const Hexer&, const Hexer&);
Hexer operator &(const Hexer&, const Hexer&);
Hexer operator |(const Hexer&, const Hexer&);
Hexer operator +(const Hexer&, const Hexer&);
bool operator ==(const Hexer&, const Hexer&);

#endif __HEXER_H__
