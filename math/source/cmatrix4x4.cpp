#include "cmatrix4x4.h"

CMatrix4x4::CMatrix4x4(const float* d, int cols, int rows) {
	for (int i = 0; i < cols && i < 4; ++i) { for (int j = 0; j < rows && j < 4; ++j) {
			m[i][j] = d[i*cols + j];
	}}
}

CMatrix4x4& CMatrix4x4::set_to_identity() {
	for (int i = 0; i < 4; ++i) { for (int j = 0; j < 4; ++j) {
		if(i == j) m[i][j]= 1.0f; 
		else m[i][j] = 0.0f;
	}}
	return (*this);
}
CMatrix4x4& CMatrix4x4::set_to_zero() {
	for (int i = 0; i < 4; ++i) { for (int j = 0; j < 4; ++j) { m[i][j] = 0; }} 
	return (*this);
}
CMatrix4x4& CMatrix4x4::set_to_transpose() {
	for (int i = 0; i < 4; ++i) { for (int j = 0; j < i; ++j) { std::swap(m[i][j], m[j][i]); } }
	return (*this);
}

CMatrix4x4 CMatrix4x4::get_transpose() const {
	CMatrix4x4 res = (*this);
	return res.set_to_transpose();
}
CMatrix4x4 operator * (const CMatrix4x4& a, const CMatrix4x4& b) {
	CMatrix4x4 res;
	for (int i = 0; i < 4; ++i) { for (int j = 0; j < 4; ++j) {
			res(i, j) = a.row(i).dot(b.column(j));
	}}
	return res;
}

CMatrix4x4& CMatrix4x4::lookAt(const CVector3D& eye, const CVector3D& center, const CVector3D& up) {

	return (*this);
}


