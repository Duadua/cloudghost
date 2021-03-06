#include "cmath.h"
#include "cmatrix3x3.h"
#include "cmatrix4x4.h"
#include <sstream>

CMatrix4x4::CMatrix4x4(const float* d, int cols, int rows) : scaled(1.0f) {
	set_to_identity();
	for (int i = 0; i < cols && i < col_size; ++i) { for (int j = 0; j < rows && j < row_size; ++j) {
			m[i][j] = d[i*cols + j];
	}}
}

CMatrix4x4& CMatrix4x4::set_to_identity() {
	scaled = CVector3D(1.0f);
	for (int i = 0; i < col_size; ++i) { for (int j = 0; j < row_size; ++j) {
		if(i == j) m[i][j]= 1.0f; 
		else m[i][j] = 0.0f;
	}}
	return (*this);
}
CMatrix4x4& CMatrix4x4::set_to_zero() {
	scaled = CVector3D(1.0f);
	for (int i = 0; i < col_size; ++i) { for (int j = 0; j < row_size; ++j) { m[i][j] = 0; }} 
	return (*this);
}
CMatrix4x4& CMatrix4x4::set_to_transpose() {
	for (int i = 0; i < col_size; ++i) { for (int j = 0; j < i; ++j) { std::swap(m[i][j], m[j][i]); } }
	return (*this);
}

float CMatrix4x4::det() const {
	float ans = 0.0f;
	for (int i = 0; i < col_size; ++i) { ans += m[i][0] * cofactor(0, i); }
	return ans;
}
float CMatrix4x4::cofactor(int row, int col) const {
	float tmp[col_size - 1][row_size - 1]{ 0 };
	int cur_c = 0;
	for (int i = 0; i < col_size; ++i) {
		if (i == col) continue;
		int cur_r = 0;
		for (int j = 0; j < row_size; ++j) {
			if (j == row) continue;
			tmp[cur_c][cur_r] = m[i][j];
			++cur_r;
		}
		++cur_c;
	}
	if ((row + col) & 1) return -1.0f * CMatrix3x3(*tmp).det();
	return CMatrix3x3(*tmp).det();
}

CMatrix4x4 CMatrix4x4::adj() const {
	CMatrix4x4 ans;
	for (int i = 0; i < col_size; ++i) {
		for (int j = 0; j < row_size; ++j) { ans(i, j) = cofactor(j, i); } // 伴随矩阵需要转置
	}
	return ans;
}
CMatrix4x4 CMatrix4x4::inverse() const {
	float d = det();
	if (CMath_ins().fcmp(d, CMath_ins().eps) == 0) { return CMatrix4x4(); }
	CMatrix4x4 res = 1.0f / d * adj();
	return res;
}
CMatrix4x4 CMatrix4x4::transpose() const {
	CMatrix4x4 res = (*this);
	return res.set_to_transpose();
}

CMatrix4x4 CMatrix4x4::operator * (float b) const {
	CMatrix4x4 res((*this));
	res.scaled = scaled * b;
	for (int i = 0; i < CMatrix4x4::row_size; ++i) {
		for (int j = 0; j < CMatrix4x4::col_size; ++j) { res(i, j) *= b; }
	}
	return res;
}
CMatrix4x4 CMatrix4x4::operator * (const CMatrix4x4& b) const {
	CMatrix4x4 res;
	res.scaled = scaled * b.scaled;
	for (int i = 0; i < CMatrix4x4::row_size; ++i) {
		for (int j = 0; j < CMatrix4x4::col_size; ++j) {
			res(i, j) = row(i).dot(b.column(j));
		}
	}
	return res;
}
CMatrix4x4 operator * (float a, const CMatrix4x4& b) { return b * a; }
CVector4D operator * (const CMatrix4x4& a, const CVector4D& b) {
	CVector4D res;
	for (int i = 0; i < CMatrix4x4::col_size; ++i) { res += a.column(i)*b[i]; }
	return res;
}
CVector3D operator * (const CMatrix4x4& a, const CVector3D& b) { return (a*CVector4D(b)).xyz(); }

std::ostream& operator << (std::ostream& out, const CMatrix4x4& b) {
	out << "CMatrix4x4 (\n";
    for (int i = 0; i < CMatrix4x4::row_size; ++i) {
		out << "\t\t";
		for (int j = 0; j < CMatrix4x4::col_size; ++j) {
			out << std::to_string(b(i, j)) << " ";
		}
		out << "\n";
	}
	out << "\t)";
	return out;
}
std::string CMatrix4x4::to_string() const {
	std::ostringstream oss;
	oss << (*this);
	return oss.str();
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
	float a = CMath_ins().deg_to_rad(angle);
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
	float x = CMath_ins().deg_to_rad(euler_angle[0]);
	float y = CMath_ins().deg_to_rad(euler_angle[1]);
	float z = CMath_ins().deg_to_rad(euler_angle[2]);
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
	rotate(1, 2) = c[0] * s[1] * s[2] - c[2] * s[0];
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
	float a = quaternion.w;
	float b = quaternion.x;
	float c = quaternion.y;
	float d = quaternion.z;

	CMatrix4x4 rotate;
	rotate(0, 0) = 1.0f - 2.0f * (c*c + d*d);
	rotate(1, 0) = 2.0f * (b*c + a*d);
	rotate(2, 0) = 2.0f * (b*d - a*c);

	rotate(0, 1) = 2.0f * (b*c - a*d);
	rotate(1, 1) = 1.0f - 2.0f * (b*b + d*d);
	rotate(2, 1) = 2.0f * (a*b + c*d);

	rotate(0, 2) = 2.0f * (a*c + b*d);
	rotate(1, 2) = 2.0f * (c*d - a*b);
	rotate(2, 2) = 1.0f - 2.0f * (b*b + c*c);

	CMatrix4x4 res(*this);
	CVector4D t_c0 = res.column(0); CVector4D t_c1 = res.column(1);
	CVector4D t_c2 = res.column(2); CVector4D t_c3 = res.column(3);
	set_column(0, t_c0 * rotate(0, 0) + t_c1 * rotate(1, 0) + t_c2 * rotate(2, 0));
	set_column(1, t_c0 * rotate(0, 1) + t_c1 * rotate(1, 1) + t_c2 * rotate(2, 1));
	set_column(2, t_c0 * rotate(0, 2) + t_c1 * rotate(1, 2) + t_c2 * rotate(2, 2));

	return (*this);
}

CVector3D	CMatrix4x4::get_rotate_euler() const {
	float sy = std::sqrt(m[0][0]*m[0][0] + m[0][1] * m[0][1]);

    float x = static_cast<float>(std::atan2(m[1][2], m[2][2]));
    float y = static_cast<float>(std::atan2(-m[0][2], sy));
    float z = static_cast<float>(std::atan2(m[0][1], m[0][0]));

	/*if (sy < CMath_ins().eps) {
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

	return CVector3D(CMath_ins().rad_to_deg(x), CMath_ins().rad_to_deg(y), CMath_ins().rad_to_deg(z));
}
CVector4D	CMatrix4x4::get_rotate_angle_axis() const {
	return CVector4D();

}
CQuaternion CMatrix4x4::get_rotate_quaternion() const {
	return CQuaternion();
}

CMatrix4x4& CMatrix4x4::lookAt(const CVector3D& eye, const CVector3D& center, const CVector3D& world_up) {
	CVector3D front = (eye - center).normalize();
	CVector3D right = world_up.cross(front).normalize();
	CVector3D up	= front.cross(right).normalize();

	CMatrix4x4 translate, rotate;

	// 初始化 translate
	translate(0, 3) = -eye.x();
	translate(1, 3) = -eye.y();
	translate(2, 3) = -eye.z();

	// 初始化 rotate
	rotate(0, 0) = right.x();	rotate(0, 1) = right.y();	rotate(0, 2) = right.z();
	rotate(1, 0) = up.x();		rotate(1, 1) = up.y();		rotate(1, 2) = up.z();
	rotate(2, 0) = front.x();	rotate(2, 1) = front.y();	rotate(2, 2) = front.z();

	(*this) = rotate * translate;
	return (*this);
}

CMatrix4x4& CMatrix4x4::lookAt_left(const CVector3D& eye, const CVector3D& center, const CVector3D& world_up) {
	CVector3D front = (center - eye).normalize();
	CVector3D right = world_up.cross(front).normalize();
	CVector3D up	= front.cross(right).normalize();

	CMatrix4x4 translate, rotate;

	// 初始化 translate
	translate(0, 3) = -eye.x();
	translate(1, 3) = -eye.y();
	translate(2, 3) = -eye.z();

	// 初始化 rotate
	rotate(0, 0) = right.x();	rotate(0, 1) = right.y();	rotate(0, 2) = right.z();
	rotate(1, 0) = up.x();		rotate(1, 1) = up.y();		rotate(1, 2) = up.z();
	rotate(2, 0) = front.x();	rotate(2, 1) = front.y();	rotate(2, 2) = front.z();

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
	float alpha = 1.0f / std::tan(CMath_ins().deg_to_rad(vertical_angle / 2.0f));
	set_to_zero();
	m[0][0] = alpha / aspect_ratio;					// x
	m[1][1] = alpha;								// y

	// z
	m[2][2] = -(far + near) / (far - near);
	m[3][2] = -(far * near * 2.0f) / (far - near);

	m[2][3] = -1;									// w
	return (*this);
}


