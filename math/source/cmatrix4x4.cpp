#include "cmath.h"
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

CMatrix4x4& CMatrix4x4::translate(const CVector3D& v) {
	CMatrix4x4 tmp((*this));
	CVector4D t_col = tmp.column(0) * v[0] + tmp.column(1) * v[1] + tmp.column(2) * v[2] + tmp.column(3);
	set_column(3, t_col);
	return (*this);
}
CMatrix4x4& CMatrix4x4::translate(float x, float y, float z) {
	CMatrix4x4 tmp((*this));
	CVector4D t_col = tmp.column(0) * x + tmp.column(1) * y + tmp.column(2) * z + tmp.column(3);
	set_column(3, t_col);
	return (*this);
}

CMatrix4x4& CMatrix4x4::scale(const CVector3D& v) {
	CMatrix4x4 tmp((*this));
	set_column(0, tmp.column(0) * v[0]);
	set_column(1, tmp.column(1) * v[1]);
	set_column(2, tmp.column(2) * v[2]);
	return (*this);
}
CMatrix4x4& CMatrix4x4::scale(float x, float y, float z) {
	CMatrix4x4 tmp((*this));
	set_column(0, tmp.column(0) * x);
	set_column(1, tmp.column(1) * y);
	set_column(2, tmp.column(2) * z);
	return (*this);
}
CMatrix4x4& CMatrix4x4::scale(float f) {
	CMatrix4x4 tmp((*this));
	set_column(0, tmp.column(0) * f);
	set_column(1, tmp.column(1) * f);
	set_column(2, tmp.column(2) * f);
	return (*this);
}

CMatrix4x4& CMatrix4x4::rotate(float angle, const CVector3D& vector) {
	return (*this);

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

CMatrix4x4& CMatrix4x4::ortho(float width, float aspect_ratio, float near, float far) {
	float height = width / aspect_ratio;
	ortho(-width/2.0f, width / 2.0f, -height/2.0f, height / 2.0f, near, far);
	return (*this);
}
CMatrix4x4& CMatrix4x4::ortho(float left, float right, float bottom, float top, float near, float far) {
	set_to_identity();									// w = 1
	// x
	m[0][0] = 2.0f / (right - left);
	m[3][0] = -(right + left) / (right - left);

	// y
	m[1][1] = 2.0f / (top - bottom);
	m[3][1] = -(top + bottom) / (top - bottom);

	// z
	m[2][2] = -2.0f / (far - near);
	m[3][2] = -(far + near) / (far - near);

	return (*this);

}
CMatrix4x4& CMatrix4x4::ortho_2d(float width, float aspect_ratio) {
	ortho(width, aspect_ratio, -1.0f, 1.0f);
	return (*this);
}
CMatrix4x4& CMatrix4x4::ortho_2d(float left, float right, float bottom, float top) {
	ortho(left, right, bottom, top, -1.0f, 1.0f);
	return (*this);
}
CMatrix4x4& CMatrix4x4::frustum(float left, float right, float bottom, float top, float near, float far) {
	set_to_zero();
	m[0][0] = (2.0f * near) / (right - left);		// x
	m[1][1] = (2.0f * near) / (top - bottom);		// y

	// z
	m[2][2] = -(far + near) / (far - near);
	m[3][2] = -(far * near * 2.0f) / (far - near);

	m[2][3] = -1.0f;								// w
	return (*this);
}
CMatrix4x4& CMatrix4x4::perspective(float vertical_angle, float aspect_ratio, float near, float far) {
	float alpha = 1.0f / std::tan(CMath::deg_to_rad(vertical_angle / 2.0f));
	set_to_zero();
	m[0][0] = alpha / aspect_ratio;					// x
	m[1][1] = alpha;								// y

	// z
	m[2][2] = -(far + near) / (far - near);
	m[3][2] = -(far * near * 2.0f) / (far - near);

	m[2][3] = -1;									// w
	return (*this);
}



