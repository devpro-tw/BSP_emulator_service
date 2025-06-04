// Hexer.cpp
//#include "StdAfx.h"
#include "Hexer.h"

// implement

	Hexer::Hexer(const string x):ptr(0), len(0) {
		string s = x;
		if (x.length() & 0x01) s += "0";
		len = s.length() >> 1;
		ptr = new char[len];
		for (int i=0; i < len; i++) {
			ptr[i] = value(s[i<<1]) << 4;
			ptr[i] |= value(s[(i<<1) + 1]);
		}
	}

	void Hexer::set(const char*p, size_t l) {
		if (!l) return;
		delete [] ptr;
		len = l;
		ptr = new char[l];
		memcpy(ptr, p, l);
	}
	
	char Hexer::value(char c) const {
		if (c >= '0' && c <= '9')
           return c - 0x30;
      if (c >= 'A' && c <= 'Z')
           return c - 0x37;
      if (c >= 'a' && c <= 'z')
           return c - 0x57;
      return 0;
	}
	char Hexer::ascii(char c) const {
		if (c < 10) return c + 0x30;
		if (c < 16) return c + 0x37;
		return 0x20;
	}
	string Hexer::toString() const {
		char* buf = new char[(len << 1) + 1];
		for (int i=0; i < len; i++) {
			buf[i << 1] = ascii((ptr[i] >> 4) & 0x0f);
			buf[(i << 1) + 1] = ascii(ptr[i] & 0x0f);
		}
		buf[len << 1] = 0;
		string s(buf);
		delete [] buf;
		return s;
	}
	char* Hexer::detach(int& l) {
		l = len; len = 0;
		char* p = ptr;	ptr = 0;
		return p;
	}
	int Hexer::copyContent(char* buffer, int buf_len) const {
		memcpy(buffer,ptr,min(len,buf_len));
		return min(len,buf_len);
	}

	void Hexer::pack() {
		int nl = len >> 1;
		if (!nl) return;
		char* pPack = new char[nl];
		char* p = ptr;

		for (int i = 0; i < nl ; i++) {
		*(pPack+i) = (*p++) << 4;
		*(pPack+i) |= ((*p++) & 0x0f);
		}
		delete [] ptr;
		ptr = pPack;
		len = nl;
	}

	void Hexer::unpack() {
		int nl = len << 1;
		if (!nl) return;
		char* pUnpk = new char[nl];
		char* p = pUnpk;

		for (int i = 0; i < len; i++) {
		*p++ = ((ptr[i] >> 4) & 0x0f | 0x30);
		*p++ = (ptr[i] & 0x0f | 0x30);
		}
		delete [] ptr;
		ptr = pUnpk;
		len = nl;
	}

	void Hexer::setLength(int length, char pad) {
		if (len < length) {
			char* p = new char [length];
			memset(p, pad, length);
			memcpy(p, ptr, len);
			delete [] ptr;
			ptr = p;
		}
		len = length;
	}

	Hexer Hexer::operator ~() {
		Hexer x(*this);
		for (int i = 0; i < len; i++)
			*(x.ptr+i) = ~(*(ptr+i));
		return x;
	}

	
	ostream& operator << (ostream& os, const Hexer& x) {
			string s = x.toString();
			os << s;
			return os;
	}
	
	istream& operator >> (istream& is, Hexer& x) {
		string s;
		is >> s;
		x=Hexer(s);
		return is;
	}
	
	Hexer operator ^(const Hexer& op1, const Hexer& op2) {
		int l = max(op1.len, op2.len);
		char* buf = new char[l];
		for (int i = 0; i < l; i++)
			buf[i] = op1.ptr[i] ^ op2.ptr[i];
		Hexer x;
		x.set(buf, l);
		delete [] buf;
		return x;
	}

	Hexer operator +(const Hexer& op1, const Hexer& op2) {
		char* buf = new char [op1.len + op2.len];
		memcpy(buf, op1.ptr, op1.len);
		memcpy(buf+op1.len,op2.ptr,op2.len);
		Hexer x;
		x.set(buf, op1.len+op2.len);
		delete [] buf;
		return x;
	}

	bool operator ==(const Hexer& left, const Hexer& right) {
		if (right.len != left.len) return false;
		return memcmp(left.ptr, right.ptr, left.len) == 0;
	}			


