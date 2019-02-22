#include "cameradata.h"
#include <QtMath>
#include <QVector3D>

IMPLEMENT_CLASS(CameraData)

CameraData::CameraData() {}
CameraData::~CameraData() {}

QMatrix4x4 CameraData::get_view_mat() { return look_at(); }

void CameraData::update(QVector3D l, QVector3D r) {
	location = l;
	rotation = r;

	float yaw = qDegreesToRadians(rotation.y());
	float pitch = qDegreesToRadians(rotation.x());
	float roll = qDegreesToRadians(rotation.z());

	// 计算 world_up
	world_up.setZ(0.0f);
	world_up.setX(qSin(roll));
	world_up.setY(qCos(roll));
	world_up.normalize();

	// 计算 front -- 初始方向 (0.0, 0.0, 0.0)
	front.setX(qSin(yaw) * qCos(pitch));
	front.setZ(qCos(yaw) * qCos(pitch));
	front.setY(qSin(pitch));
	front.normalize();

	// 计算 right
	right = QVector3D::crossProduct(front, world_up);
	right.normalize();

	// 计算 up
	up = QVector3D::crossProduct(right, front);
	up.normalize();
}

QMatrix4x4 CameraData::look_at() {
	QMatrix4x4 translate, rotate;

	// 初始化 translate
	translate(0, 3) = -location.x();
	translate(1, 3) = -location.y();
	translate(2, 3) = -location.z();

	// 初始化 rotate
	rotate(0, 0) = -right.x();	rotate(0, 1) = -right.y();	rotate(0, 2) = -right.z(); 
	rotate(1, 0) = up.x();		rotate(1, 1) = up.y();		rotate(1, 2) = up.z(); 
	rotate(2, 0) = -front.x();	rotate(2, 1) = -front.y();	rotate(2, 2) = -front.z(); 

	return rotate * translate;
}
