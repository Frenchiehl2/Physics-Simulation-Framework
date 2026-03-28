#include "Prog1_2Simulation.hpp"
#include <iostream>
//#include "helpers.hpp"
#include "Eigen/Geometry"

Prog1_2Simulation::Prog1_2Simulation()
{
    //Initialize position matrices
    m_pos = std::vector<Eigen::Vector3d>();
    m_pos.emplace_back(-1,0,0);
    m_pos.emplace_back(1,0,0);
    m_pos.emplace_back(0,1,0);
    m_pos.emplace_back(0,-1,0);

    //Initialize old position matrix
    m_old_pos = std::vector<Eigen::Vector3d>(m_pos);

    //Inialize veloceties
    m_velocities = std::vector<Eigen::Vector3d>();
    m_velocities.emplace_back(0,-1,0);
    m_velocities.emplace_back(0,1,0);
    m_velocities.emplace_back(-1,0,0);
    m_velocities.emplace_back(1,0,0);

    m_mass = std::vector<double>();
    m_mass.emplace_back(1);
    m_mass.emplace_back(1);
    m_mass.emplace_back(1);
    m_mass.emplace_back(1);

    grav = 1.5;
    dtime = 0.01;
    frame=0;
}

Prog1_2Simulation::~Prog1_2Simulation() = default;

std::vector<Eigen::Vector3d> const & Prog1_2Simulation::step()
{
    //verlet calculation method
    if(frame == 0){
        for(int i = 0; i < m_pos.size();i++){
            m_pos[i] = m_old_pos[i] + dtime * m_velocities[i] + 0.5 * std::pow(dtime,2) * f(m_old_pos,i);
        }
    }else{
        auto new_pos = std::vector<Eigen::Vector3d>(m_pos);

        for(int i = 0 ; i < m_pos.size();i++){
            new_pos[i] = 2* m_pos[i] - m_old_pos[i] + std::pow(dtime,2) * f(m_pos,i);
        }

        m_old_pos = m_pos;
        m_pos = new_pos;
    }

    frame++;
    return m_pos;
}

Eigen::Vector3d Prog1_2Simulation::f(std::vector<Eigen::Vector3d> const & pos,int index){

    Eigen::Vector3d f = Eigen::Vector3d::Zero();
    for(int i = 0 ; i < pos.size() ; i++){
        if(i != index){
            auto r = (pos[i] - pos[index]);
            f = m_mass[i] * r / std::pow(r.norm(),3);
        }
    }
    return f * 6;
}

void Prog1_2Simulation::reset(
	std::vector<std::tuple<Eigen::Vector3d, Eigen::Vector3d, double>> const & bodies,
	double G,
	double dt
)
{
    m_pos.clear();
    m_old_pos.clear();
    m_velocities.clear();
    m_mass.clear();

    //reset body variables
    for(auto const & body : bodies){
        m_pos.push_back(std::get<0>(body));
        m_old_pos.push_back(std::get<0>(body));
        m_velocities.push_back(std::get<1>(body));
        m_mass.push_back(std::get<2>(body));
    }

    this->grav = grav;
    this->dtime = dtime;
    frame = 0;
}
