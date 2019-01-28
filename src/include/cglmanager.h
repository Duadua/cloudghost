#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_core>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

class CGLManager : public QOpenGLWidget {
	Q_OBJECT

public:
	CGLManager(QWidget *parent = Q_NULLPTR);
	~CGLManager();

protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;

protected:
	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
	QOpenGLFunctions_3_3_Core * core;

	class GameObject* cube_object;
	class CameraComponent* main_camera;

	//class InputThread* input_thread;
public slots:
	void mouse_sgclick();
};
