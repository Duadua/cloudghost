#pragma once

#include "cvector3d.h"
#include <memory>
#include <string>

class CMatrix3x3 {
public:
	static const int row_size = 3;
	static const int col_size = 3;

	CMatrix3x3() { set_to_identity(); }
	CMatrix3x3(const float* d) { memcpy(m, d, sizeof(m)); }
	CMatrix3x3(const float* d, int cols, int rows);
	inline CMatrix3x3(
		float m11, float m12, float m13,
		float m21, float m22, float m23,
		float m31, float m32, float m33
	);

	inline CVector3D column(int index) const;
	inline CMatrix3x3& set_column(int index, const CVector3D& value);
	inline CVector3D row(int index) const;
	inline CMatrix3x3& set_row(int index, const CVector3D& value);

	inline float& operator()(int row, int col);						// return m[column][row]
	inline const float& operator()(int row, int col) const;

	CMatrix3x3 operator * (float b) const;
	CMatrix3x3 operator * (const CMatrix3x3& b) const;
	friend CMatrix3x3 operator * (float a, const CMatrix3x3& b);
	friend CVector3D operator * (const CMatrix3x3& a, const CVector3D& b);

	friend std::ostream& operator << (std::ostream& out, const CMatrix3x3& b);
	std::string to_string() const;

	CMatrix3x3& set_to_identity();									// 单位矩阵
	CMatrix3x3& set_to_zero();										// 0矩阵
	CMatrix3x3& set_to_transpose();									// 转置矩阵

	float det() const;												// 行列式的值
	float cofactor(int row, int col) const;							// 代数余子式

	CMatrix3x3 adj() const;											// 伴随矩阵
	CMatrix3x3 inverse() const;										// 逆矩阵
	CMatrix3x3 transpose() const;									// 转置矩阵										

	inline const float* data() const { return *m; }
private:
	float m[col_size][row_size];													// 列主序 -- m[i][j] -- 第 i 列 第 j 行

};

inline CMatrix3x3::CMatrix3x3(
	float m11, float m12, float m13,
	float m21, float m22, float m23,
	float m31, float m32, float m33
) {
	m[0][0] = m11; m[0][1] = m21; m[0][2] = m31; 
	m[1][0] = m12; m[1][1] = m22; m[1][2] = m32;
	m[2][0] = m13; m[2][1] = m23; m[2][2] = m33;
}


inline CVector3D CMatrix3x3::column(int index) const { return CVector3D(m[index][0], m[index][1], m[index][2]); }
inline CMatrix3x3& CMatrix3x3::set_column(int index, const CVector3D& value) {
	m[index][0] = value[0]; m[index][1] = value[1]; m[index][2] = value[2];
	return (*this);
}
inline CVector3D CMatrix3x3::row(int index) const { return CVector3D(m[0][index], m[1][index], m[2][index]); }
inline CMatrix3x3& CMatrix3x3::set_row(int index, const CVector3D& value) {
	m[0][index] = value[0]; m[1][index] = value[1]; m[2][index] = value[2];
	return (*this);
}

inline float& CMatrix3x3::operator()(int row, int col) {
	if (row < 0) row = 0; if (row >= row_size) row = row_size - 1;
	if (col < 0) col = 0; if (col >= col_size) col = col_size - 1;
	return m[col][row];
}
inline const float& CMatrix3x3::operator()(int row, int col) const {
	if (row < 0) row = 0; if (row >= row_size) row = row_size - 1;
	if (col < 0) col = 0; if (col >= col_size) col = col_size - 1;
	return m[col][row];
}

