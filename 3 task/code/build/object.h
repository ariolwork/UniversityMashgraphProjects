#ifndef OBJECT_H
#define OBJECT_H

#include "common.h"
#include "ShaderProgram.h"
#include "color.h"
#include "animation.h"
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <string>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class object{
public:
	object();
	object(glm::vec3 _loc);
	object(glm::vec3 _loc, glm::vec3 _scale);
	~object();
	void set_color(Color _color);
	void draw(ShaderProgram program) const;
	void set_texture(std::string _text_name);
	void set_rotation(glm::vec3 _rot, float value=1.0);
	animation anim;
protected:
	glm::vec3 loc = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 const_rotation = glm::vec3(1.0f, 0.0f, 0.0f);
	float const_rotation_value = 0.0f;
	GLuint VBO = 0, VAO = 0, TBO = 0;
	float* points;
	int points_count;
	std::string text_name = "";
	Color color = Color();

	void push_to_the_buffer();
	virtual void set_points() = 0;

};



class triangle: public object{
public:
	triangle();
	triangle(glm::vec3 _loc);
	triangle(glm::vec3 _loc, glm::vec3 _scale);
private:
	void set_points() override;
	void need_things();
};

class square: public object{
public:
	square();
	square(glm::vec3 _loc);
	square(glm::vec3 _loc, glm::vec3 _scale);
private:
	void set_points() override;
	void need_things();
};

class hexagon: public object{
public:
	hexagon();
	hexagon(glm::vec3 _loc);
	hexagon(glm::vec3 _loc, glm::vec3 _scale);
private:
	void set_points() override;
	void need_things();
};

class cube: public object{
public:
	cube();
	cube(glm::vec3 _loc);
	cube(glm::vec3 _loc, glm::vec3 _scale);
private:
	void set_points() override;
	void need_things();
};

class model: public object{
public:
	model(std::string a);
	model(std::string a, glm::vec3 _loc);
	model(std::string a, glm::vec3 _loc, glm::vec3 _scale = glm::vec3(0.5f));
private:
	void set_points() override;
	void need_things();
	std::string _a;
};
#endif