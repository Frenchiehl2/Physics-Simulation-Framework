#pragma once

#include <QObject>
#include "Prog1_1Simulation.hpp"
#include "Eigen/Core"

class QMouseEvent;

class OpenGLRenderer : public QObject
{
	Q_OBJECT

public:
	using QObject::QObject;

//Virtual declarations can be overriden in other classes
	virtual void resize(int w, int h) = 0;
	virtual void render() = 0;

	virtual void mouseEvent(QMouseEvent * e) = 0;

signals:
	void update();
};
