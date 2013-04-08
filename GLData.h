#ifndef GLDATA_H
#define GLDATA_H

#include "glew.h"

typedef struct {
		float m[16];
} Matrix;



static const double PI = 3.14159265358979323846;

static const Matrix IDENTITY_MATRIX = {{
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
}};


#endif // GLDATA_H
