#pragma once

#include "cvector2d.h"
#include <memory>
#include <string>

class CMatrix2x2 {
public:
	static const int row_size = 2;
	static const int col_size = 2;

	CMatrix2x2() { set_to_identity(); }
	CMatrix2x2(const float* d) { memcpy(m, d, sizeof(m)); }
	CMatrix2x2(const float* d, int cols, int rows);
	inline CMatrix2x2(
		float m11, float m12, 
		float m21, float m22
	);

	inline CVector2D column(int index) const;
	inline CMatrix2x2& set_column(int index, const CVector2D& value);
	inline CVector2D row(int index) const;
	inline CMatrix2x2& set_row(int index, const CVector2D& value);

	inline float& operator()(int row, int col);						// return m[column][row]
	inline const float& operator()(int row, int col) const;

	friend CMatrix2x2 operator * (const CMatrix2x2& a, const CMatrix2x2& b);
	friend CVector2D operator * (const CMatrix2x2& a, const CVector2D& b);
	friend std::ostream& operator << (std::ostream& out, const CMatrix2x2& b);
	std::string to_string() const;

	CMatrix2x2& set_to_identity();									// 单位矩阵
	CMatrix2x2& set_to_zero();										// 0矩阵
	CMatrix2x2& set_to_transpose();									// 转置矩阵

	CMatrix2x2 get_transpose() const;								// 转置矩阵										

	inline const float* data() const { return *m; }
private:
	float m[col_size][row_size];													// 列主序 -- m[i][j] -- 第 i 列 第 j 行

};

inline CMatrix2x2::CMatrix2x2(
	float m11, float m12,
	float m21, float m22 ){
	m[0][0] = m11; m[0][1] = m21;
	m[1][0] = m12; m[1][1] = m22;
}

inline CVector2D CMatrix2x2::column(int index) const { return CVector2D(m[index][0], m[index][1]); }
inline CMatrix2x2& CMatrix2x2::set_column(int index, const CVector2D& value) {
	m[index][0] = value[0]; m[index][1] = value[1]; 
	return (*this);
}
inline CVector2D CMatrix2x2::row(int index) const { return CVector2D(m[0][index], m[1][index]); }
inline CMatrix2x2& CMatrix2x2::set_row(int index, const CVector2D& value) {
	m[0][index] = value[0]; m[1][index] = value[1]; 
	return (*this);
}

inline float& CMatrix2x2::operator()(int row, int col) {
	if (row < 0) row = 0; if (row >= row_size) row = row_size - 1;
	if (col < 0) col = 0; if (col >= col_size) col = col_size - 1;
	return m[col][row];
}
inline const float& CMatrix2x2::operator()(int row, int col) const {
	if (row < 0) row = 0; if (row >= row_size) row = row_size - 1;
	if (col < 0) col = 0; if (col >= col_size) col = col_size - 1;
	return m[col][row];
}

