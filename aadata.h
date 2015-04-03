#ifndef AADATA_H
#define AADATA_H

#include <sstream>
#include <string>

using namespace std;

class AADataBase {
	char * bytes;
	size_t size;
public:
	AADataBase() {
		bytes = NULL;
	}
	AADataBase(char * a, size_t s) {
		bytes = a;
		size = s;
	}
	virtual ~AADataBase() {
		if (bytes) {
			delete [] bytes;
		}
	}
	static int compareBytes(const char *a, size_t aLen, const char *b, size_t bLen)  {
		if (!a && !b) {
			return 0;
		}
		if (!a) {
			return -1;
		}
		if (!b) {
			return 1;
		}
		for (size_t i=0; i<aLen && i<bLen; i++) {
			if (a[i] < b[i]) {
				return -1;
			}
			if (a[i] > b[i]) {
				return 1;
			}
		}
		return aLen < bLen ? -1 : ((aLen > bLen) ? 1 : 0);
	}
	
	virtual int compare(const AADataBase *b)  const {
		return compareBytes(this->bytes, this->size, b->bytes, b->size);
	}
	virtual int onequal(AADataBase *b) {
		return 0;
	}
	virtual string toString() const {
		string s;
		for (size_t i=0; i<size; i++) {
			char dump[5];
			sprintf(dump, "%x", bytes[i]);		
			s.append(dump);
		}
		return s;
	}
	virtual const char * getBytes(size_t &retSize) const {
		retSize = size;
		return bytes;
	}
	static AADataBase *setBytes(char *a, size_t s) {
		return new AADataBase(a, s);
	}
};

class AADataInt : public AADataBase {
	int param;
public:
	AADataInt() {
		param = 0;
	}
	AADataInt(int i) {
		param = i;
	}
	int compare(const AADataBase *b) const {
		if (!b) {
			return 1;
		}
		int cmpParam = ((AADataInt*)b)->param;
		return param < cmpParam ? -1 : (param > cmpParam ? 1 : 0);
	}
	int onequal(AADataBase *b) {
		return 0;
	}
	string toString() const {
		ostringstream oss;
		oss << param;
		return oss.str();	
	}
	const char * getBytes(size_t &retSize) const {
		retSize = 4;
		char *a = new char[retSize];
		a[0] = (param >> 24) & 0xFF;
		a[1] = (param >> 16) & 0xFF;
		a[2] = (param >> 8) & 0xFF;
		a[3] = param & 0xFF;				
		return a;
	}
	static AADataBase *setBytes(char *a, size_t s) {
		int param = 0;
		for (size_t i = 0; i < s; i++) {
			param += (a[s - 1 - i] << (i * 8));
		}	
		return new AADataInt(param);
	}
};

#endif //AADATA_H