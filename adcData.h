#ifndef ADCDATA_H
#define ADCDATA_H

typedef struct {
		int nodeNumber;
		float x;
		float y;
		float z;
} Node;


typedef struct {
		int elementNumber;
		unsigned int n1;
		unsigned int n2;
		unsigned int n3;
		unsigned int n4;
} Element;

#endif // ADCDATA_H
