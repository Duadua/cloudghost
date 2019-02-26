#pragma once

#include "cvector4d.h"
#include <memory>

class CMatrix4x4 {
public:
	CMatrix4x4() { set_to_identity(); }
	CMatrix4x4(const float* d) { memcpy(m, d, sizeof(m)); }
	CMatrix4x4(const float* d, int cols, int rows);
	inline CMatrix4x4(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
	);

	inline CVector4D column(int index) const;
	inline void set_column(int index, const CVector4D& value);
	inline CVector4D row(int index) const;
	inline void set_row(int index, const CVector4D& value);

	inline float& operator()(int row, int col);						// return m[column][row]
	inline const float& operator()(int row, int col) const;

	void set_to_identity();											// 单位矩阵
	void set_to_zero();												// 0矩阵

	inline const float* data() const { return *m; }
private:
	float m[4][4];													// 列主序 -- m[i][j] -- 第 i 列 第 j 行

};

inline CMatrix4x4::CMatrix4x4 (
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44
) {
	m[0][0] = m11; m[0][1] = m21; m[0][2] = m31; m[0][3] = m41;
    m[1][0] = m12; m[1][1] = m22; m[1][2] = m32; m[1][3] = m42;
    m[2][0] = m13; m[2][1] = m23; m[2][2] = m33; m[2][3] = m43;
    m[3][0] = m14; m[3][1] = m24; m[3][2] = m34; m[3][3] = m44;
}

inline CVector4D CMatrix4x4::column(int index) const {
	return CVector4D(m[index][0], m[index][1], m[index][2], m[index][3]); 
}
inline void CMatrix4x4::set_column(int index, const CVector4D& value) {
	m[index][0] = value[0]; m[index][1] = value[1]; m[index][2] = value[2]; m[index][3] = value[3]; 
}
inline CVector4D CMatrix4x4::row(int index) const {
	return CVector4D(m[0][index], m[1][index], m[2][index], m[3][index]);
}
inline void CMatrix4x4::set_row(int index, const CVector4D& value) {
	m[0][index] = value[0]; m[1][index] = value[1]; m[2][index] = value[2]; m[3][index] = value[3];
}

inline float& CMatrix4x4::operator()(int row, int col) {
	if (row < 0) row = 0; if (row > 3) row = 3; 
	if (col < 0) col = 0; if (col > 3) col = 3; 
	return m[col][row]; 
}
inline const float& CMatrix4x4::operator()(int row, int col) const {
	if (row < 0) row = 0; if (row > 3) row = 3; 
	if (col < 0) col = 0; if (col > 3) col = 3; 
	return m[col][row]; 
}
