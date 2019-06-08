#ifndef ANIMATION_H
#define ANIMATION_H

#include <glm/gtc/type_ptr.hpp>

class rotation{
public:
	rotation();
	rotation(glm::vec3 _axis_of_rotation, float _rot_speed = 1.0);
	void set_rot_time_move(float mv);
	glm::mat4 rotate(glm::mat4 r_m);
protected:
	glm::vec3 axis_of_rotation = glm::vec3(0.0, 0.0, 1.0);
	float rot_speed = 1.0f;
	float rot_time_move = 0.0f;
};


class move{
public:
	void set_time_move(float tm);
	void set_speed(float sp);
	glm::mat4 translate(glm::mat4 r_m);
	virtual glm::vec3 get_new_location();
	virtual glm::mat4 rotate_for_cool_move(glm::mat4 m);
protected:
	float time_move = 0.0f;
	float speed = 1.0;
};


struct animation{
	rotation* rot;
	move* mv;
};



class curcle_move: public move{
public:
	curcle_move();
	curcle_move(glm::vec3 _axis, float _radius = 1.0);
	glm::vec3 get_new_location() override;
private:
	glm::vec3 axis = glm::vec3(1.0f, 1.0f, 0.0f);
	float radius = 1.0;
};


class equalizer: public move{
public:
	equalizer();
	equalizer(float _this_height);
	glm::vec3 get_new_location() override;
private:
	float this_height = 1.0;
};


class square_move: public move{
public:
	square_move();
	square_move(glm::vec3 _axis, float _radius = 1.0f);
	glm::vec3 get_new_location() override;
	glm::mat4 rotate_for_cool_move(glm::mat4 m) override;
private:
	glm::vec3 axis = glm::vec3(1.0f, 1.0f, 0.0f);
	float radius = 1.0;
	int side = 0;
};



class boom_animation: public move{
public:
	boom_animation();
	boom_animation(glm::vec3 _axis, float _radius = 1.0f);
	void set_back_plate(float _back_plate);
	glm::vec3 get_new_location() override;
	glm::mat4 rotate_for_cool_move(glm::mat4 m) override;
private:
	glm::vec3 axis = glm::vec3(1.0f, 1.0f, 0.0f);
	glm::vec3 rot_axis = glm::vec3(1.0f, 1.0f, 0.0f);
	float radius = 1.0;
	float radius_start = 1.0;
	float pause_time = 0;
	float back_plate = 800.0;
};


class dna_animation: public move{
public:
	dna_animation();
	dna_animation(glm::vec3 _axis, float _radius = 1.0f);
	void set_height(float _h);
	glm::vec3 get_new_location() override;
	int count_of_curcles = 2.0;
	float index = 0;
private:
	glm::vec3 axis = glm::vec3(1.0f, 1.0f, 0.0f);
	float radius = 1.0;
	float height = 10.0;
};

#endif


