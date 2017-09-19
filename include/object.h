#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();
    //modified section
    void Update_smallcube(unsigned int dt, glm::mat4 center);

    glm::mat4 GetModel();

    //modified section
    void reverse_rotation();
    void reverse_cube_direction();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float angle;

    //modified section
    bool rotation_direction;
    bool direction_direction;
    float direction;
};

#endif /* OBJECT_H */
