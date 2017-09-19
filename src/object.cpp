#include "object.h"

Object::Object()
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  angle = 0.0f;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  //modified section
  rotation_direction=true;
  direction_direction=true;
  direction = 0.0f;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt)
{
  angle += dt * M_PI/1000;
  direction += dt * M_PI/1000;

 //rotation
  glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));
  if(!rotation_direction)
  {
    angle -= 2 * dt * M_PI/1000;
    rotation_matrix = glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));
  }

//direction
  glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.5*sin(direction/3), 0.0, 4.5*cos(direction/3)));
  if(!direction_direction)
  {
    direction -= 2 * dt * M_PI/1000;
    translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.5*sin(direction/3), 0.0, 4.5*cos(direction/3)));
  }
  
  model = translation_matrix * rotation_matrix;
  
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Update_smallcube(unsigned int dt, glm::mat4 center)
{
  angle += dt * M_PI/1000;
  direction += dt * M_PI/1000;
//need to scale size down, then translate to its position
//scaling
  glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));

//rotation
  glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), (-angle), glm::vec3(0.0, 1.0, 0.0));

  model = scaling_matrix * rotation_matrix;
  model[3]= glm::vec4(center[3][0]+ 3 * sin(direction), center[3][1], center[3][2] + 3 * cos(direction), center[3][3]);

  //glm::mat4 translation_matrix = glm::translate(center, glm::vec3(2*sin(direction/2), 2*cos(direction/2), 0.0));
  
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}


//modified section
void Object::reverse_rotation()
{
  if(rotation_direction==true)
  {
    rotation_direction=false;
  }
  else
  {
    rotation_direction=true;
  }
}

void Object::reverse_cube_direction()
{
  if(direction_direction==true)
  {
    direction_direction=false;
  }
  else
  {
    direction_direction=true;
  }
}


















