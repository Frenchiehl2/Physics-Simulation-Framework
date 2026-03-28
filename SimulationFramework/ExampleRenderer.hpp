#pragma once

#include "OpenGLRenderer.hpp"
#include "constants.hpp"

#include <OpenGLObjects.h>

#include <QElapsedTimer>
#include <QPoint>

#include <Eigen/Core>

#include "Prog1_1Simulation.hpp"
#include "Prog1_2Simulation.hpp"


class ExampleRenderer : public OpenGLRenderer
{
	Q_OBJECT

public:
	ExampleRenderer(QObject * parent);

	void resize(int w, int h) override;
	void render() override;

	void mouseEvent(QMouseEvent * e) override;

    void reset(Prog1_1Simulation::Integration integration,
               Eigen::Vector3d r0 ,
               Eigen::Vector3d dr0,
			   double g,
               double dt) ;

    Prog1_1Simulation simulation;

protected:
	double
		cameraAzimuth = constants::pi<double>,
		cameraElevation = constants::half_pi<double>;
	bool rotateInteraction = false;

	int width = 0, height = 0;

	QPointF lastPos;
	QElapsedTimer timer;
	quint64 lastTimeNS = 0;

	Eigen::Matrix4d
		projectionMatrix,
		viewMatrix;

	gl::Buffer
		icosphereVertexBuffer,
		icosphereIndexBuffer;

	gl::VertexArray
		icosphereVAO,
		skyboxVAO;

	gl::Program
		icosphereProgram,
		skyboxProgram;

	gl::Texture
		earthTexture,
		moonTexture,
		starsCubeMap;

	GLsizei numIcosphereIndices = 0;
};
