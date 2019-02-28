#include "cquaternion.h"

CQuaternion::CQuaternion(const CVector3D& euler_angle) {

}
CQuaternion::CQuaternion(const CVector4D& angle_axis) {

}

CVector3D CQuaternion::get_euler_angle() const {
	return CVector3D();
}
CVector4D CQuaternion::get_angle_axis() const {
	return CVector4D();

}

CQuaternion CQuaternion::llerp(const CQuaternion& a, const CQuaternion& b) {
	return CQuaternion();
}
CQuaternion CQuaternion::slerp(const CQuaternion& a, const CQuaternion& b) {
	return CQuaternion();
}

void CQuaternion::normalize() {

}
