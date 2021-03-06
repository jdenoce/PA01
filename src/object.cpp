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
*/
//Vertex geometry[12];
loadOBJ("cube_export.obj");
//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


//end of file reading
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
 unsigned int ang = 30;
 angle = 30;
  
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


bool Object::loadOBJ(const char * path)
{
  FILE * file = fopen(path, "r");
  if(file==NULL){
  std::cout<<"OBJ file cannot be opened"<<std::endl;
  }
  glm::vec3 varray[15];
  unsigned int iarray[59];
  int i = 0, z=0;
  while( 1 )
  {
    
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF){
        break;
    }
    if ( strcmp( lineHeader, "v" ) == 0 ){
    glm::vec3 vertex;
    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
    varray[i] = vertex;
    i++;
    }
    if ( strcmp( lineHeader, "f" ) == 0 ){
    unsigned int vcode1, vcode2, vcode3, normal;
    fscanf(file, "%d//%d %d//%d %d//%d\n", &vcode1, &normal, &vcode2, &normal, &vcode3, &normal );
    iarray[z]=vcode1;
    z++;
    iarray[z]=vcode2;
    z++;
    iarray[z]=vcode3;
    z++;
    }
  
  }//end while loop

  //sets Indicies
  Indices = {iarray[0],iarray[1],iarray[2],iarray[3],iarray[4],iarray[5],iarray[6],iarray[7],iarray[8],iarray[9],iarray[10],iarray[11],iarray[12],iarray[13],iarray[14],iarray[15],iarray[16],iarray[17],iarray[18],iarray[19],iarray[20],iarray[21],iarray[22],iarray[23],iarray[24],iarray[25],iarray[26],iarray[27],iarray[28],iarray[29],iarray[30],iarray[31],iarray[32],iarray[33],iarray[34],iarray[35],iarray[36],iarray[37],iarray[38],iarray[39],iarray[40],iarray[41],iarray[42],iarray[43],iarray[44],iarray[45],iarray[46],iarray[47],iarray[48],iarray[49],iarray[50],iarray[51],iarray[52],iarray[53],iarray[54],iarray[55],iarray[56],iarray[57],iarray[58],iarray[59]};

  //sets Verticies
  glm::vec3 red = {1.0, 0.0 , 0.0}, yellow = {1.0,1.0,0.0}, blue = {0.0,0.0,1.0};
  Vertices = {{varray[0], yellow}, {varray[1], yellow}, {varray[2], yellow}, {varray[3], yellow}, {varray[4], yellow}, {varray[5], yellow}, {varray[6], yellow}, {varray[7], yellow}, {varray[8], red}, {varray[9], red},{varray[10], red},{varray[11], red}};
  
}//end OBJ loader

/*
bool Object::loadOBJ(const char * path, std::vector < glm::vec3 > & out_vertices, std::vector < glm::vec2 > & out_uvs, std::vector < glm::vec3 > & out_normals)
{
std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< glm::vec3 > temp_vertices;				
std::vector< glm::vec2 > temp_uvs;
std::vector< glm::vec3 > temp_normals;

FILE * file = fopen(path, "r");
if(file == NULL)
{
  std::cout<<"cannot open obj file"<<std::endl;
}

while( 1 ){

    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF){
        break;
}
if ( strcmp( lineHeader, "v" ) == 0 ){
    glm::vec3 vertex;
    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
    temp_vertices.push_back(vertex);
}else if ( strcmp( lineHeader, "vt" ) == 0 ){
    glm::vec2 uv;
    fscanf(file, "%f %f\n", &uv.x, &uv.y );
    temp_uvs.push_back(uv);
}else if ( strcmp( lineHeader, "vn" ) == 0 ){
    glm::vec3 normal;
    fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
    temp_normals.push_back(normal);
}else if ( strcmp( lineHeader, "f" ) == 0 ){
    std::string vertex1, vertex2, vertex3;
    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
    int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

    if (matches != 9){
        printf("File can't be read by our simple parser : ( Try exporting with other options\n");
    }

    vertexIndices.push_back(vertexIndex[0]);
    vertexIndices.push_back(vertexIndex[1]);
    vertexIndices.push_back(vertexIndex[2]);
    uvIndices.push_back(uvIndex[0]);
    uvIndices.push_back(uvIndex[1]);
    uvIndices.push_back(uvIndex[2]);
    normalIndices.push_back(normalIndex[0]);
    normalIndices.push_back(normalIndex[1]);
    normalIndices.push_back(normalIndex[2]);
}
}
  for( unsigned int i=0; i<vertexIndices.size(); i++ )
  {
    unsigned int vertexIndex = vertexIndices[i];
    glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
    out_vertices.push_back(vertex);
  }
}
*/














