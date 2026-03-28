#pragma once

#include <Eigen/Core>

class Prog1_1Simulation
{
public:
	Prog1_1Simulation();
	~Prog1_1Simulation();

	enum class Integration
	{
		Explicit,
		Implicit,
		Verlet
	};

	Eigen::Vector3d step();
	void reset(
		Integration integration,
		Eigen::Vector3d r0,
		Eigen::Vector3d dr0,
		double g,
		double dt
	);

  class ExampleRenderer;

//new calculation vetors
private:

    Eigen::Vector3d r_future;
    Eigen::Vector3d r_current;
    Eigen::Vector3d r_default;
    Eigen::Vector3d r_temp;

    Eigen::Vector3d dr_current;
    Eigen::Vector3d dr_future;

    //unefficient frame capture
    unsigned long long frame = 0;

    double grav;
    double d_time;

    Integration integration;
};
