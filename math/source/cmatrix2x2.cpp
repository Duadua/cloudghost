#include "cmath.h"
#include "cmatrix2x2.h"
#include <sstream>

CMatrix2x2::CMatrix2x2(const float* d, int cols, int rows) {
	for (int i = 0; i < cols && i < col_size; ++i) {
		for (int j = 0; j < rows && j < row_size; ++j) {
			m[i][j] = d[i*cols + j];
		}
	}
}

CMatrix2x2& CMatrix2x2::set_to_identity() {
	for (int i = 0; i < col_size; ++i) {
		for (int j = 0; j < row_size; ++j) {
			if (i == j) m[i][j] = 1.0f;
			else m[i][j] = 0.0f;
		}
	}
	return (*this);
}
CMatrix2x2& CMatrix2x2::set_to_zero() {
	for (int i = 0; i < col_size; ++i) { for (int j = 0; j < row_size; ++j) { m[i][j] = 0; } }
	return (*this);
}
CMatrix2x2& CMatrix2x2::set_to_transpose() {
	for (int i = 0; i < row_size; ++i) { for (int j = 0; j < i; ++j) { std::swap(m[i][j], m[j][i]); } }
	return (*this);
}

CMatrix2x2 CMatrix2x2::get_transpose() const {
	CMatrix2x2 res = (*this);
	return res.set_to_transpose();
}

CMatrix2x2 operator * (const CMatrix2x2& a, const CMatrix2x2& b) {
	CMatrix2x2 res;
	for (int i = 0; i < CMatrix2x2::row_size; ++i) {
		for (int j = 0; j < CMatrix2x2::col_size; ++j) {
			res(i, j) = a.row(i).dot(b.column(j));
		}
	}
	return res;
}
CVector2D operator * (const CMatrix2x2& a, const CVector2D& b) {
	CVector2D res;
	for (int i = 0; i < CMatrix2x2::col_size; ++i) { res += a.column(i)*b[i]; }
	return res;
}

std::ostream& operator << (std::ostream& out, const CMatrix2x2& b) {
	out << "CMatrix2x2 (\n";
	for (int i = 0; i < CMatrix2x2::row_size; ++i) {
		out << "\t\t";
		for (int j = 0; j < CMatrix2x2::col_size; ++j) {
			out << std::to_string(b(i, j)) << " ";
		}
		out << "\n";
	}
	out << "\t)";
	return out;
}
std::string CMatrix2x2::to_string() const {
	std::ostringstream oss;
	oss << (*this);
	return oss.str();
}

