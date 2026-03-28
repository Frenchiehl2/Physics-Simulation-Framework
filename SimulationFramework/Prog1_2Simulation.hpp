#pragma once

#include <Eigen/Core>

#include <vector>

class Prog1_2Simulation
{
public:
	Prog1_2Simulation();
	~Prog1_2Simulation();

	std::vector<Eigen::Vector3d> const & step();

    Eigen::Vector3d f(std::vector<Eigen::Vector3d> const & positions, int index);

	void reset(
		std::vector<std::tuple<Eigen::Vector3d, Eigen::Vector3d, double>> const & bodies,
		double G,
		double dt
	);

private:

    //array/vector of Eigen::Vector3d
    std::vector<Eigen::Vector3d> m_pos;
    std::vector<Eigen::Vector3d> m_old_pos;
    std::vector<Eigen::Vector3d> m_velocities;
    std::vector<double> m_mass;
    double grav;
    double dtime;

    unsigned long long frame = 0;
};
