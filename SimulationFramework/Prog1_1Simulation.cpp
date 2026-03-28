#include "Prog1_1Simulation.hpp"
#include <iostream>
#include <Eigen/LU>

using namespace Eigen;
using namespace std;


Prog1_1Simulation::Prog1_1Simulation()
{
    r_temp = r_default = r_current = r_future = Vector3d{0,1,0};
    dr_current = dr_future = Vector3d{3,0,0};

    grav = 9.0;
    d_time = 0.01;
}

Prog1_1Simulation::~Prog1_1Simulation() = default;

Vector3d Prog1_1Simulation::step()
{

    switch(integration){

    case Integration::Explicit:
    {

        //new vector calculation for y(t + delta t)
        Vector3d r_new = (r_current + (d_time * dr_current));
        Vector3d dr_new = (dr_current - d_time * (grav / pow(r_current.norm(),3))*r_current);

        r_current = r_new;
        dr_current = dr_new;

        return r_current;
    }

    case Integration::Implicit:
    {

        //create and fill matrix
        Matrix<double,6,6> mat = Matrix<double,6,6>();

        //Jacobian matrix with partial dirivatives for each element
        mat.block<3,3>(0,0) = Matrix3d::Zero();
        mat.block<3,3>(0,3) = Matrix3d::Identity();
        mat.block<3,3>(3,3) = Matrix3d::Zero();
        mat.block<3,3>(3,0) <<  (-2 * pow(r_current[0],2) + pow(r_current[1],2) + pow(r_current[2],2)),
            (-3 * r_current[0]*r_current[1]),
            (-3 * r_current[0]*r_current[2]),
            (-3 * r_current[0]*r_current[1]),
            (pow(r_current[0],2) -2 * pow(r_current[1],2)+pow(r_current[2],2)),
            (-3 * r_current[1]*r_current[2]),
            (-3 * r_current[0]*r_current[2]),
            (-3 * r_current[1]*r_current[2]),
            (pow(r_current[0],2) + pow(r_current[1],2) -2 * pow(r_current[2],2));

        mat.block<3,3>(3,0) = (-grav * pow(r_current[0] * r_current[1] * r_current[1] * r_current[2] * r_current[2], -2.5) * mat.block<3,3>(3,0));

        Matrix<double,6,6> leftHand = Matrix<double,6,6>::Identity() - d_time * mat;

        Vector<double,6> vec = Vector<double,6>();
        vec.block<3,1>(0,0) = dr_current;
        vec.block<3,1>(3,0) = (-grav * pow(r_current.norm(), -3)*r_current);

        Vector<double,6> rightHand = d_time * vec;
        Vector<double,6> ret = leftHand.fullPivLu().solve(rightHand);

        r_current = r_current + ret.block<3,1>(0,0);
        dr_current = dr_current + ret.block<3,1>(3,0);

        return r_current;
    }

    case Integration::Verlet:
    {

        //initializer
        if(frame == 0){
            r_current = ((r_default + d_time * dr_current)+(0.5* pow(d_time,2))*(-grav * pow(r_default.norm(),-3))*r_default);
        }else{
        //shift to new system by editing default settings
            //each new rate of change is a differance between the past calulation and the future calculation x(t - delta t) = -x(t - delta t)
            r_default = (2* r_current - r_temp + pow(d_time,2) * (-grav * pow(r_current.norm(),-3)) * r_current);
            r_temp = r_current;
            r_current = r_default;
        }
        frame++;
        return r_current;
    }

    default:
        throw runtime_error("Faulty integration parameters!");
    }

}

void Prog1_1Simulation::reset(
	Integration integration,
	Eigen::Vector3d r0,
	Eigen::Vector3d dr0,
	double g,
	double dt
)
{
    this->integration = integration;
    r_temp = r_current = r_future =  r0;
    dr_current = dr_future = dr0;
    this->grav = g;
    this->d_time = dt;
    frame = 0;

}
