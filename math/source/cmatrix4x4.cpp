#include "cmath.h"
#include "cmatrix4x4.h"

CMatrix4x4::CMatrix4x4(const float* d, int cols, int rows) : scaled(1.0f) {
	for (int i = 0; i < cols && i < 4; ++i) { for (int j = 0; j < rows && j < 4; ++j) {
			m[i][j] = d[i*cols + j];
	}}
}

CMatrix4x4& CMatrix4x4::set_to_identity() {
	scaled = CVector3D(1.0f);
	for (int i = 0; i < 4; ++i) { for (int j = 0; j < 4; ++j) {
		if(i == j) m[i][j]= 1.0f; 
		else m[i][j] = 0.0f;
	}}
	return (*this);
}
CMatrix4x4& CMatrix4x4::set_to_zero() {
	scaled = CVector3D(1.0f);
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
	res.scaled = a.scaled * b.scaled;
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
	scaled *= v;
	CMatrix4x4 tmp((*this));
	set_column(0, tmp.column(0) * v[0]);
	set_column(1, tmp.column(1) * v[1]);
	set_column(2, tmp.column(2) * v[2]);
	return (*this);
}
CMatrix4x4& CMatrix4x4::scale(float x, float y, float z) {
	scaled *= CVector3D(x, y, z);
	CMatrix4x4 tmp((*this));
	set_column(0, tmp.column(0) * x);
	set_column(1, tmp.column(1) * y);
	set_column(2, tmp.column(2) * z);
	return (*this);
}
CMatrix4x4& CMatrix4x4::scale(float f) {
	scaled *= f;
	CMatrix4x4 tmp((*this));
	set_column(0, tmp.column(0) * f);
	set_column(1, tmp.column(1) * f);
	set_column(2, tmp.column(2) * f);
	return (*this);
}

CMatrix4x4& CMatrix4x4::rotate(float angle, const CVector3D& v) {
	float a = CMath::deg_to_rad(angle);
	float c = std::cos(a);
	float s = std::sin(a);
	
	CVector3D axis(v.get_normalize());
	CVector3D temp((1.0f - c) * axis);					// axis * (1 - cos)

	CMatrix4x4 rotate;
	rotate(0, 0) = c + temp[0] * axis[0];
	rotate(1, 0) = temp[0] * axis[1] + s * axis[2];
	rotate(2, 0) = temp[0] * axis[2] - s * axis[1];

	rotate(0, 1) = temp[1] * axis[0] - s * axis[2];
	rotate(1, 1) = c + temp[1] * axis[1];
	rotate(2, 1) = temp[1] * axis[2] + s * axis[0];

	rotate(0, 2) = temp[2] * axis[0] + s * axis[1];
	rotate(1, 2) = temp[2] * axis[1] - s * axis[0];
	rotate(2, 2) = c + temp[2] * axis[2];

	CMatrix4x4 res(*this);
	CVector4D t_c0 = res.column(0); CVector4D t_c1 = res.column(1);
	CVector4D t_c2 = res.column(2); CVector4D t_c3 = res.column(3);
	set_column(0, t_c0 * rotate(0, 0) + t_c1 * rotate(1, 0) + t_c2 * rotate(2, 0));
	set_column(1, t_c0 * rotate(0, 1) + t_c1 * rotate(1, 1) + t_c2 * rotate(2, 1));
	set_column(2, t_c0 * rotate(0, 2) + t_c1 * rotate(1, 2) + t_c2 * rotate(2, 2));

	return (*this);
}
CMatrix4x4& CMatrix4x4::rotate(float angle, float x, float y, float z) {
	rotate(angle, CVector3D(x, y, z));
	return (*this);
}
CMatrix4x4& CMatrix4x4::rotate_euler(const CVector3D& euler_angle) {
	float x = CMath::deg_to_rad(euler_angle[0]);
	float y = CMath::deg_to_rad(euler_angle[1]);
	float z = CMath::deg_to_rad(euler_angle[2]);
	CVector3D c(std::cos(x), std::cos(y), std::cos(z));
	CVector3D s(std::sin(x), std::sin(y), std::sin(z));

	CMatrix4x4 rotate;
	rotate(0, 0) = c[1] * c[2];
	rotate(1, 0) = c[1] * s[2];
	rotate(2, 0) = -s[1];

	rotate(0, 1) = c[2] * s[0] * s[1] - c[0] * s[2];
	rotate(1, 1) = s[0] * s[1] * s[2] + c[0] * c[2];
	rotate(2, 1) = c[1] * s[0];

	rotate(0, 2) = c[0] * c[2] * s[1] + s[0] * s[2];
	rotate(1, 2) = c[0] * s[1] * s[2] - c[2] * s[2];
	rotate(2, 2) = c[0] * c[1];

	CMatrix4x4 res(*this);
	CVector4D t_c0 = res.column(0); CVector4D t_c1 = res.column(1);
	CVector4D t_c2 = res.column(2); CVector4D t_c3 = res.column(3);
	set_column(0, t_c0 * rotate(0, 0) + t_c1 * rotate(1, 0) + t_c2 * rotate(2, 0));
	set_column(1, t_c0 * rotate(0, 1) + t_c1 * rotate(1, 1) + t_c2 * rotate(2, 1));
	set_column(2, t_c0 * rotate(0, 2) + t_c1 * rotate(1, 2) + t_c2 * rotate(2, 2));
 
	return (*this);
}
CMatrix4x4& CMatrix4x4::rotate_euler(float x, float y, float z) {
	rotate_euler(CVector3D(x, y, z));
	return (*this);
}
CMatrix4x4& CMatrix4x4::rotate_quaternion(const CQuaternion& quaternion) {

	return (*this);
}

CVector3D	CMatrix4x4::get_rotate_euler() const {
	float sy = std::sqrt(m[0][0]*m[0][0] + m[0][1] * m[0][1]);

	float x = (float)std::atan2(m[1][2], m[2][2]);
	float y = (float)std::atan2(-m[0][2], sy);
	float z = (float)std::atan2(m[0][1], m[0][0]);

	/*if (sy < CMath::eps) {
		x = (float)std::atan2(-m[2][1], m[1][1]);
		y = (float)std::atan2(-m[0][2], sy);
		z = 0.0f;
	}
	else {
		x = (float)std::atan2(m[1][2], m[2][2]);
		y = (float)std::atan2(-m[0][2], sy);
		z = (float)std::atan2(m[0][1], m[0][0]);
	}
	*/

	return CVector3D(CMath::rad_to_deg(x), CMath::rad_to_deg(y), CMath::rad_to_deg(z));
}
CVector4D	CMatrix4x4::get_rotate_angle_axis() const {
	return CVector4D();

}
CQuaternion CMatrix4x4::get_rotate_quaternion() const {
	return CQuaternion();
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



