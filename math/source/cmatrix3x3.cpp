#include "cmath.h"
#include "cmatrix2x2.h"
#include "cmatrix3x3.h"
#include <sstream>

CMatrix3x3::CMatrix3x3(const float* d, int cols, int rows) {
	for (int i = 0; i < cols && i < col_size; ++i) {
		for (int j = 0; j < rows && j < row_size; ++j) {
			m[i][j] = d[i*cols + j];
		}
	}
}

CMatrix3x3& CMatrix3x3::set_to_identity() {
	for (int i = 0; i < col_size; ++i) {
		for (int j = 0; j < row_size; ++j) {
			if (i == j) m[i][j] = 1.0f;
			else m[i][j] = 0.0f;
		}
	}
	return (*this);
}
CMatrix3x3& CMatrix3x3::set_to_zero() {
	for (int i = 0; i < col_size; ++i) { for (int j = 0; j < row_size; ++j) { m[i][j] = 0; } }
	return (*this);
}
CMatrix3x3& CMatrix3x3::set_to_transpose() {
	for (int i = 0; i < row_size; ++i) { for (int j = 0; j < i; ++j) { std::swap(m[i][j], m[j][i]); } }
	return (*this);
}

float CMatrix3x3::det() const {
	float ans = 0.0f;
	for (int i = 0; i < col_size; ++i) { ans += m[i][0] * cofactor(0, i); }
	return ans;
}

float CMatrix3x3::cofactor(int row, int col) const {
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
	if ((row + col) & 1) return -1.0f * CMatrix2x2(*tmp).det();
	return CMatrix2x2(*tmp).det();
}

CMatrix3x3 CMatrix3x3::adj() const {
	CMatrix3x3 ans;
	for (int i = 0; i < col_size; ++i) {
		for (int j = 0; j < row_size; ++j) { ans(i, j) = cofactor(j, i); } // 伴随矩阵需要转置
	}
	return ans;
}
CMatrix3x3 CMatrix3x3::inverse() const {
	float d = det();
	if (CMath_ins().fcmp(d, CMath_ins().eps) == 0) { return CMatrix3x3(); }
	float di = 1.0f / d;
	CMatrix3x3 res = di * adj();
	return res;
}
CMatrix3x3 CMatrix3x3::transpose() const {
	CMatrix3x3 res = (*this);
	return res.set_to_transpose();
}

CMatrix3x3 CMatrix3x3::operator * (float b) const {
	CMatrix3x3 res((*this));
	for (int i = 0; i < row_size; ++i) {
		for (int j = 0; j < col_size; ++j) { res(i, j) *= b; }
	}
	return res;
}
CMatrix3x3 CMatrix3x3::operator * (const CMatrix3x3& b) const {
	CMatrix3x3 res;
	for (int i = 0; i < row_size; ++i) {
		for (int j = 0; j < col_size; ++j) {
			res(i, j) = row(i).dot(b.column(j));
		}
	}
	return res;
}
CMatrix3x3 operator * (float a, const CMatrix3x3& b) { return b * a; }
CVector3D operator * (const CMatrix3x3& a, const CVector3D& b) {
	CVector3D res;
	for (int i = 0; i < CMatrix3x3::col_size; ++i) { res += a.column(i)*b[i]; }
	return res;
}

std::ostream& operator << (std::ostream& out, const CMatrix3x3& b) {
	out << "CMatrix3x3 (\n";
	for (int i = 0; i < CMatrix3x3::row_size; ++i) {
		out << "\t\t";
		for (int j = 0; j < CMatrix3x3::col_size; ++j) {
			out << std::to_string(b(i, j)) << " ";
		}
		out << "\n";
	}
	out << "\t)";
	return out;
}
std::string CMatrix3x3::to_string() const {
	std::ostringstream oss;
	oss << (*this);
	return oss.str();
}

