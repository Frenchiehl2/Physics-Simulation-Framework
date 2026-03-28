
#ifndef SIMULATIONFRAMEWORK_PROG1_2RENDERER_H
#define SIMULATIONFRAMEWORK_PROG1_2RENDERER_H

#include "OpenGLRenderer.hpp"
#include "Prog1_2Simulation.hpp"
#include "ExampleRenderer.hpp"

class Prog1_2Renderer : public ExampleRenderer{


public:
    Prog1_2Renderer(QObject* parent);
    void render() override;

    Prog1_2Simulation* getSimulation();

private:

    Prog1_2Simulation simulation;

};

#endif
