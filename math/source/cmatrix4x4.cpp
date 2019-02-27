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

CMatrix4x4& CMatrix4x4::lookAt(const CVector3D& eye, const CVector3D& center, const CVector3D& world_up) {
	CVector3D front = (center - eye).normalize();
	CVector3D right = front.cross(world_up).normalize();
	CVector3D up	= right.cross(front).normalize();

	CMatrix4x4 translate, rotate;

	// 初始化 translate
	translate(0, 3) = -eye.x();
	translate(1, 3) = -eye.y();
	translate(2, 3) = -eye.z();

	// 初始化 rotate
	rotate(0, 0) = -right.x();	rotate(0, 1) = -right.y();	rotate(0, 2) = -right.z();
	rotate(1, 0) = up.x();		rotate(1, 1) = up.y();		rotate(1, 2) = up.z();
	rotate(2, 0) = -front.x();	rotate(2, 1) = -front.y();	rotate(2, 2) = -front.z();

	(*this) = rotate * translate;
	return (*this);
}


