#include "cmatrix4x4.h"

CMatrix4x4::CMatrix4x4(const float* d, int cols, int rows) {
	for (int i = 0; i < cols && i < 4; ++i) { for (int j = 0; j < rows && j < 4; ++j) {
			m[i][j] = d[i*cols + j];
	}}
}

void CMatrix4x4::set_to_identity() {
	for (int i = 0; i < 4; ++i) { for (int j = 0; j < 4; ++j) {
		if(i == j) m[i][j]= 1.0f; 
		else m[i][j] = 0.0f;
	}}
}

void CMatrix4x4::set_to_zero() { for (int i = 0; i < 4; ++i) { for (int j = 0; j < 4; ++j) { m[i][j] = 0; }} }

