#include "object.h"
#include "color.h"
#include "ShaderProgram.h"

#include <stdlib.h>
#include <string>
#include "SOIL/SOIL.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


//object
object::object(){
	loc = glm::vec3(0.0,0.0,0.0);
	scale = glm::vec3(0.5f, 0.5f, 0.5f);
}
object::object(glm::vec3 _loc):loc(_loc){
	scale = glm::vec3(0.5f, 0.5f, 0.5f);
}
object::object(glm::vec3 _loc, glm::vec3 _scale):scale(_scale), loc(_loc){
}

object::~object(){
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
void object::set_color(Color _color){
	color = _color;
}
void object::set_rotation(glm::vec3 _rot, float value){
  const_rotation = _rot;
  const_rotation_value = value;
}  
void object::set_texture(std::string _text_name){
	text_name = _text_name;
	glGenTextures(1, &TBO);GL_CHECK_ERRORS;
    glBindTexture(GL_TEXTURE_2D, TBO);GL_CHECK_ERRORS;    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);GL_CHECK_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);GL_CHECK_ERRORS;
    int width, height;
    unsigned char* image = SOIL_load_image(text_name.c_str(), &width, &height, 0, SOIL_LOAD_RGB);GL_CHECK_ERRORS;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);GL_CHECK_ERRORS;  
    glGenerateMipmap(GL_TEXTURE_2D);GL_CHECK_ERRORS;
    SOIL_free_image_data(image);GL_CHECK_ERRORS;
    glBindTexture(GL_TEXTURE_2D, 0);GL_CHECK_ERRORS;
}
void object::draw(ShaderProgram program) const {
	glBindTexture(GL_TEXTURE_2D, TBO);GL_CHECK_ERRORS;
    glBindVertexArray(VAO);GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);GL_CHECK_ERRORS;

    glm::mat4 trans(1.0f);
    trans = glm::translate(trans, loc + anim.mv->get_new_location());
    trans = anim.rot->rotate(trans);
    trans = glm::rotate(trans, (GLfloat)glm::radians(const_rotation_value),const_rotation);
    trans = anim.mv->rotate_for_cool_move(trans);
    trans = glm::scale(trans, scale);

    program.SetUniform("model", trans);
    program.SetUniform("used_texture", ((text_name=="")?(0):(1)));
    program.SetUniform("material.ambient" , color.ambient);
    program.SetUniform("material.diffuse" , color.diffuse);
    program.SetUniform("material.specular" , color.specular);
    program.SetUniform("material.shininess" , color.shininess);


    glDrawArrays(GL_TRIANGLES, 0, points_count);GL_CHECK_ERRORS;

    glBindTexture(GL_TEXTURE_2D, 0);GL_CHECK_ERRORS;
    glBindVertexArray(0);GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, 0);GL_CHECK_ERRORS;
}
void object::push_to_the_buffer(){
	glGenBuffers(1, &VBO);GL_CHECK_ERRORS;
    glGenVertexArrays(1, &VAO);GL_CHECK_ERRORS;
            
    glBindVertexArray(VAO);GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);GL_CHECK_ERRORS;
            
    glBufferData(GL_ARRAY_BUFFER, points_count*8*sizeof(GLfloat), (GLfloat*)points, GL_STATIC_DRAW);GL_CHECK_ERRORS;
    
    //coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), 0);GL_CHECK_ERRORS;
    glEnableVertexAttribArray(0);GL_CHECK_ERRORS;
    //text_coords
    glVertexAttribPointer(1, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));GL_CHECK_ERRORS;
    glEnableVertexAttribArray(1);GL_CHECK_ERRORS;
    //norms
    glVertexAttribPointer(2, 3, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));GL_CHECK_ERRORS;
    glEnableVertexAttribArray(2);GL_CHECK_ERRORS;

    glBindVertexArray(0);GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, 0);GL_CHECK_ERRORS;
}



//triangle
triangle::triangle():object(){need_things();}
triangle::triangle(glm::vec3 _loc):object(_loc){need_things();}
triangle::triangle(glm::vec3 _loc, glm::vec3 _scale):object(_loc, _scale){need_things();}
void triangle::set_points() {
	points_count = 3;
	GLfloat coords[] = {
       -0.5f,-0.5f,0.0f,  0.0f,1.0f,  0.0f,0.0f,-1.0f,
        0.5f,-0.5f,0.0f,  1.0f,1.0f,  0.0f,0.0f,-1.0f, 
        0.0f, 0.37f,0.0f,  0.5f,0.0f,  0.0f,0.0f,-1.0f 
    };
    int num_count = sizeof(coords);
    points = (GLfloat*)malloc(num_count*sizeof(GLfloat));
    for (int i = 0; i < num_count; i++) points[i] = coords[i];
}
void triangle::need_things(){
	set_points();
	push_to_the_buffer();
}



//square
square::square():object(){need_things();}
square::square(glm::vec3 _loc):object(_loc){need_things();}
square::square(glm::vec3 _loc, glm::vec3 _scale):object(_loc, _scale){need_things();}
void square::set_points() {
	points_count = 6;
	GLfloat coords[] = {
       -0.5f,-0.5f,0.0f,  0.0f,1.0f,  0.0f,0.0f,-1.0f,
        0.5f,-0.5f,0.0f,  1.0f,1.0f,  0.0f,0.0f,-1.0f,
        0.5f, 0.5f,0.0f,  1.0f,0.0f,  0.0f,0.0f,-1.0f,
        0.5f, 0.5f,0.0f,  1.0f,0.0f,  0.0f,0.0f,-1.0f,
       -0.5f, 0.5f,0.0f,  0.0f,0.0f,  0.0f,0.0f,-1.0f,
       -0.5f,-0.5f,0.0f,  0.0f,1.0f,  0.0f,0.0f,-1.0f
    };
    int num_count = sizeof(coords);
    points = (GLfloat*)malloc(num_count*sizeof(GLfloat));
    for (int i = 0; i < num_count; i++) points[i] = coords[i];
}
void square::need_things(){
	set_points();
	push_to_the_buffer();
}


//hexagon 
hexagon::hexagon():object(){need_things();}
hexagon::hexagon(glm::vec3 _loc):object(_loc){need_things();}
hexagon::hexagon(glm::vec3 _loc, glm::vec3 _scale):object(_loc, _scale){need_things();}
void hexagon::set_points() {
	points_count = 18;
	GLfloat coords[] = {
       -0.5f,   0.0f,  0.0f,  0.0f, 0.5f,  0.0f,0.0f,-1.0f,
        0.0f,   0.0f,  0.0f,  0.5f, 0.5f,  0.0f,0.0f,-1.0f,
       -0.25f, -0.433f,0.0f,  0.25f,0.0f,  0.0f,0.0f,-1.0f,

        0.5f,   0.0f,  0.0f,  1.0f, 0.5f,  0.0f,0.0f,-1.0f,
        0.0f,   0.0f,  0.0f,  0.5f, 0.5f,  0.0f,0.0f,-1.0f,
        0.25f, -0.433f,0.0f,  0.75f,0.0f,  0.0f,0.0f,-1.0f,

        0.25f, -0.433f,0.0f,  0.75f,0.0f,  0.0f,0.0f,-1.0f,
        0.0f,   0.0f,  0.0f,  0.5f, 0.5f,  0.0f,0.0f,-1.0f,
       -0.25f, -0.433f,0.0f,  0.25f,0.0f,  0.0f,0.0f,-1.0f,

	     -0.5f,   0.0f,  0.0f,  0.0f, 0.5f,  0.0f,0.0f,-1.0f,
        0.0f,   0.0f,  0.0f,  0.5f, 0.5f,  0.0f,0.0f,-1.0f,
       -0.25f,  0.433f,0.0f,  0.25f,1.0f,  0.0f,0.0f,-1.0f,

        0.5f,   0.0f,  0.0f,  1.0f, 0.5f,  0.0f,0.0f,-1.0f,
        0.0f,   0.0f,  0.0f,  0.5f, 0.5f,  0.0f,0.0f,-1.0f,
        0.25f,  0.433f,0.0f,  0.75f,1.0f,  0.0f,0.0f,-1.0f,

        0.25f,  0.433f,0.0f,  0.75f, 1.0f,  0.0f,0.0f,-1.0f,
        0.0f,   0.0f,  0.0f,  0.5f, 0.5f,  0.0f,0.0f,-1.0f,
       -0.25f,  0.433f,0.0f,  0.25f,1.0f,  0.0f,0.0f,-1.0f 
    };
    int num_count = sizeof(coords);
    points = (GLfloat*)malloc(num_count*sizeof(GLfloat));
    for (int i = 0; i < num_count; i++) points[i] = coords[i];
}
void hexagon::need_things(){
	set_points();
	push_to_the_buffer();
}


//cube 
cube::cube():object(){need_things();}
cube::cube(glm::vec3 _loc):object(_loc){need_things();}
cube::cube(glm::vec3 _loc, glm::vec3 _scale):object(_loc, _scale){need_things();}
void cube::set_points() {
	points_count = 36;
	GLfloat coords[] = {
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
       	0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
    };
    int num_count = sizeof(coords);
    points = (GLfloat*)malloc(num_count*sizeof(GLfloat));
    for (int i = 0; i < num_count; i++) points[i] = coords[i];
}
void cube::need_things(){
	set_points();
	push_to_the_buffer();
}






//model 
model::model(std::string a):object(){_a = a;need_things();}
model::model(std::string a, glm::vec3 _loc):object(_loc){_a = a;need_things();}
model::model(std::string a, glm::vec3 _loc, glm::vec3 _scale):object(_loc, _scale){_a = a;need_things();}
void model::need_things(){
  set_points();
  push_to_the_buffer();
}
void model::set_points() {
    std::vector<GLfloat> all;
    float bav = 0.0;
    Assimp::Importer importer; 
    const aiScene* scene = importer.ReadFile( _a,  
      aiProcess_CalcTangentSpace       |  
      aiProcess_Triangulate            | 
      aiProcess_JoinIdenticalVertices  | 
      aiProcess_SortByPType); 

    if(!scene) { 
      std::cout<< "Couldn't load model ", "Error Importing Asset"; 
      return; 
    } 

    const int iVertexTotalSize = sizeof(aiVector3D)*2+sizeof(aiVector2D); 

    int iTotalVertices = 0; 

    for(int i =0;i<scene->mNumMeshes;i++) { 
      aiMesh* mesh = scene->mMeshes[i]; 
      int iMeshFaces = mesh->mNumFaces; 
      for(int j = 0; j< iMeshFaces;j++) { 
         const aiFace& face = mesh->mFaces[j];
         aiVector3D pos[3];
         aiVector3D uv[3];
         aiVector3D normal[3];
         int has_norm = 0;
         for(int k=0;k<3;k++) 
         {
            pos[k] = mesh->mVertices[face.mIndices[k]]; 
            uv[k] = mesh->mTextureCoords[0][face.mIndices[k]]; 
            normal[k] = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
            if(normal[k] != aiVector3D(1.0f, 1.0f, 1.0f)) has_norm = 1;
         }
         if(has_norm == 0){
          for(int k=0;k<3;k++){
            normal[k].x = (pos[1].y - pos[0].y)*(pos[2].z - pos[0].z) - (pos[2].y - pos[0].y)*(pos[2].z - pos[0].z);
            normal[k].y = (pos[1].x - pos[0].x)*(pos[2].z - pos[0].z) - (pos[2].x - pos[0].x)*(pos[2].z - pos[0].z);
            normal[k].z = (pos[1].x - pos[0].x)*(pos[2].y - pos[0].y) - (pos[2].x - pos[0].x)*(pos[1].y - pos[0].y);
          }
         }
         for(int k=0;k<3;k++){
            bav = ((bav>glm::abs(pos[k].x))?(bav):(glm::abs(pos[k].x)));
            bav = ((bav>glm::abs(pos[k].x))?(bav):(glm::abs(pos[k].y)));
            bav = ((bav>glm::abs(pos[k].x))?(bav):(glm::abs(pos[k].z)));
            all.push_back(pos[k].x);all.push_back(pos[k].y);all.push_back(pos[k].z); 
            all.push_back(uv[k].x);all.push_back(uv[k].y);
            all.push_back(normal[k].x);all.push_back(normal[k].y);all.push_back(normal[k].z);
         }

      } 
   } 
   int num_count = all.size();
   points_count = num_count/8;
   std::cout<<_a<<":   point count:"<<points_count<<"\n";
   points = (GLfloat*)malloc(num_count*sizeof(GLfloat));
   for (int i = 0; i < num_count; i++) {points[i] = all[i];if(i%8<3)points[i]/=bav;}
   std::cout<<"end"<<"\n";
}
