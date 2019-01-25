#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_core>

class CGLManager : public QOpenGLWidget {
	Q_OBJECT

public:
	CGLManager(QWidget *parent = Q_NULLPTR);
	~CGLManager();

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

private:
	QOpenGLFunctions_3_3_Core * core;

	class GameObject* cube_object;
	class CameraComponent* main_camera;
};
