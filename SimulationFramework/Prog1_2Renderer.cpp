#include "Prog1_2Renderer.hpp"
#include <iostream>
#include "helpers.hpp"
#include "Eigen/Geometry"

using namespace std;
using namespace Eigen;

//simulation variable storage
Prog1_2Renderer::Prog1_2Renderer(QObject* parent) : ExampleRenderer(parent){
    simulation = Prog1_2Simulation();
}

//setup render environment
void Prog1_2Renderer::render(){

    //calculate new delta time
    auto currentTimeNS = timer.nsecsElapsed();
    auto deltaTime = currentTimeNS - this->lastTimeNS;
    //update new old time to be the past current time
    this->lastTimeNS = currentTimeNS;

    int loc = -1;

    //clear depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    //recompute view matrix for camera position and direction from azimuth and elevation
    {
        auto az_sin = sin(this->cameraAzimuth);
        auto az_cos = cos(this->cameraAzimuth);
        auto el_sin = sin(this->cameraElevation);
        auto el_cos = cos(this->cameraElevation);

        auto distance = 5;

        this->viewMatrix =  calculateLookAtMatrix(
            distance* Vector3d{el_sin * az_cos,el_sin * az_sin, el_cos},
            {0,0,0},
            {0,0,1});
    }

    //set parameters for tcosphereProgram and tell the renderer what to render
    //set what shader should be used
    //grab sphere program for manipulation
    glUseProgram(this->icosphereProgram.id());

    //set what texture is used
    loc = glGetUniformLocation(this->icosphereProgram.id(),"colorTexture");
    glUniform1i(loc,0);

    //set projection matrix
    loc = glGetUniformLocation(this->icosphereProgram.id(),"projection");
    glUniformMatrix4fv(loc,1,GL_FALSE,this->projectionMatrix.cast<float>().eval().data());

    //bind the sphere for drawing to the pipeline
    glBindVertexArray(this->icosphereProgram.id());

    auto bodies = simulation.step();

    //calculate the new position of the moon with each step/frame
    for(int i = 0; i < bodies.size();i++){

        //get new positiona nd rotation matrix
        Affine3d ModelTransformation = Translation3d(bodies[i]) * Scaling(.1);

        glActiveTexture(GL_TEXTURE + 0);
        glBindTexture(GL_TEXTURE_2D, this->moonTexture.id());

        //set modelview matrix
        loc  = glGetUniformLocation(this->icosphereProgram.id(),"modelView");
        glUniformMatrix4fv(loc,1,GL_FALSE, (this->viewMatrix * ModelTransformation.matrix()).cast<float>().eval().data());

        //draw the sphere verticies with coresponding indices
        glDrawElements(GL_TRIANGLES,this->numIcosphereIndices,GL_UNSIGNED_INT,nullptr);
    }

    //grab skybox for manipulation
    glUseProgram(this->skyboxProgram.id());

    //set texture of skybox program
    loc = glGetUniformLocation(this->skyboxProgram.id(),"colorTexture");
    glUniform1i(loc,0);

    //apply texture to grabed program
    glBindTexture(GL_TEXTURE_CUBE_MAP,this->starsCubeMap.id());

    loc = glGetUniformLocation(this->skyboxProgram.id(),"modelView");
    glUniformMatrix4fv(loc,1,GL_FALSE,this->viewMatrix.cast<float>().eval().data());

    loc = glGetUniformLocation(this->skyboxProgram.id(),"Projection");
    glUniformMatrix4fv(loc,1,GL_FALSE,this->projectionMatrix.cast<float>().eval().data());

    //draw skybox
    glDepthFunc(GL_EQUAL);
    glBindVertexArray(this->skyboxVAO.id());
    glDrawarOSrays(GL_TRIANGLES,0,3);
    glDepthFunc(GL_LESS);


    //go to next frame
    this->update();

}

Prog1_2Simulation* Prog1_2Renderer::getSimulation(){
    return &simulation;
}



































