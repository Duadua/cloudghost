#pragma once

#include <QMatrix4x4>

class CameraData {
public:
	CameraData();
	~CameraData();

	QMatrix4x4 get_view_mat();

	void update(QVector3D location, QVector3D rotation);

private:

	QMatrix4x4 look_at();

	QVector3D location;
	QVector3D rotation;
	
	QVector3D front;
	QVector3D right;
	QVector3D up;

	QVector3D world_up;

};
