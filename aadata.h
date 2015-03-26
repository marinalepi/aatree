#ifndef AADATA_H
#define AADATA_H

#include <sstream>
#include <string>

using namespace std;

class AADataBase {
	unsigned char * bytes;
	unsigned int size;
public:
	AADataBase() {
		bytes = NULL;
	}
	AADataBase(unsigned char * a, unsigned int s) {
		bytes = a;
		size = s;
	}
	virtual ~AADataBase() {
		if (bytes) {
			delete [] bytes;
		}
	}
	virtual int compare(AADataBase *b) {
		if (!b) {
			return 1;
		}
		for (unsigned int i=0,j=0; i<size && j<b->size; i++, j++) {
			if (bytes[i] < bytes[j]) {
				return -1;
			}
			if (bytes[i] > bytes[j]) {
				return 1;
			}
		}
		if (size < b->size) {
			return -1;
		}
		if (size > b->size) {
			return 1;
		}
		return 0;
	}
	virtual int onequal(AADataBase *b) {
		return 0;
	}
	virtual string toString() const {
		string s;
		for (unsigned int i=0; i<size; i++) {
			char dump[5];
			sprintf(dump, "%x", bytes[i]);		
			s.append(dump);
		}
		return s;
	}
	virtual const unsigned char * getBytes(unsigned int &retSize) const {
		retSize = size;
		return bytes;
	}
};

inline AADataBase *createAADataBase(unsigned char *a, unsigned int s) {
	return new AADataBase(a, s);
}

class AADataInt : public AADataBase {
	int param;
public:
	AADataInt() {
		param = 0;
	}
	AADataInt(int i) {
		param = i;
	}
	int compare(AADataBase *b) {
		int cmpParam = ((AADataInt*)b)->param;
		if (param < cmpParam) {
			return -1;
		}
		if (param > cmpParam) {
			return 1;
		}
		return 0;
	}
	int onequal(AADataBase *b) {
		return 0;
	}
	string toString() const {
		ostringstream oss;
		oss << param;
		return oss.str();	
	}
	const unsigned char * getBytes(unsigned int &retSize) const {
		retSize = 4;
		unsigned char *a = new unsigned char[retSize];
		a[0] = (param >> 24) & 0xFF;
		a[1] = (param >> 16) & 0xFF;
		a[2] = (param >> 8) & 0xFF;
		a[3] = param & 0xFF;				
		return a;
	}
};
inline AADataBase *createAADataInt(unsigned char *a, unsigned int s) {
	int param = 0;
	for (unsigned int i = 0; i < s; i++) {
		param += (a[s - 1 - i] << (i * 8));
	}	
	return new AADataInt(param);
}

#endif //AADATA_H