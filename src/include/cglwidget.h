#pragma once

#include <memory>
#include <QOpenGLWidget>

class CGLWidget : public QOpenGLWidget {
	Q_OBJECT

public:
	CGLWidget(QWidget *parent = Q_NULLPTR);
    ~CGLWidget() override;

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

	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;

public slots:
	void mouse_pressed_over();
	void key_pressed_over();
};
