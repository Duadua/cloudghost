#include "cvector2d.h"
#include "cvector3d.h"

CVector2D::CVector2D(const CVector3D& abc) { _x = abc.x(); _y = abc.y(); }

float CVector2D::dot(const CVector2D& b) const { return _x * b.x() + _y * b.y(); }
float CVector2D::cross(const CVector2D& b) const { return _x * b.y() - b.x()*_y; }
