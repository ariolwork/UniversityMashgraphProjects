#include "animation.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

//rotation
rotation::rotation(){}
rotation::rotation(glm::vec3 _axis_of_rotation, float _rot_speed ){
	axis_of_rotation = _axis_of_rotation;
	rot_speed = _rot_speed;
}
void rotation::set_rot_time_move(float mv){
	rot_time_move = mv;
}
glm::mat4 rotation::rotate(glm::mat4 r_m){
	auto static_move =  (GLfloat)glm::radians(rot_time_move);
	return glm::rotate(r_m, ((GLfloat)glfwGetTime() + static_move)*(float)glm::radians(50.0)*rot_speed, axis_of_rotation);
}

//move
void move::set_time_move(float tm){
	time_move = tm;
}
void move::set_speed(float sp){
	speed = sp;
}
glm::vec3 move::get_new_location(){return glm::vec3(0.0f, 0.0f, 0.0f);}
glm::mat4 move::translate(glm::mat4 r_m){
	return glm::translate(r_m, get_new_location());
}
glm::mat4 move::rotate_for_cool_move(glm::mat4 m){
	return glm::rotate(m, 0.0f ,glm::vec3(0.0, 0.0, 1.0));
}


//curcle_nove
curcle_move::curcle_move(){}
curcle_move::curcle_move(glm::vec3 _axis, float _radius){
	axis = _axis;
	radius = _radius;
}
glm::vec3 curcle_move::get_new_location(){
	auto static_move =  (GLfloat)glm::radians(time_move);
	if (axis.x != 0){
		if(axis.y != 0)
			return glm::vec3(axis.x * radius * glm::sin(((GLfloat)glfwGetTime() + static_move)*speed), 
							 axis.y * radius * glm::cos(((GLfloat)glfwGetTime() + static_move)*speed),
							 0.0f);
		else
			return glm::vec3(axis.x * radius * glm::sin(((GLfloat)glfwGetTime() + static_move)*speed), 
							 0.0f,
							 axis.z * radius * glm::cos(((GLfloat)glfwGetTime() + static_move)*speed));
	} else {
		return glm::vec3(0.0f, 
						 axis.y * radius * glm::sin(((GLfloat)glfwGetTime() + static_move)*speed),
						 axis.z * radius * glm::cos(((GLfloat)glfwGetTime() + static_move)*speed));
	}
}



//equalizer
equalizer::equalizer(){}
equalizer::equalizer(float _this_height){
	this_height = _this_height;
}
glm::vec3 equalizer::get_new_location(){
	auto static_move =  (GLfloat)glm::radians(time_move);
	float now_height = this_height*glm::abs(glm::cos(((GLfloat)glfwGetTime() + static_move))*speed);
	return glm::vec3(0.0,now_height, 0.0);
}



//square_move
square_move::square_move(){}
square_move::square_move(glm::vec3 _axis, float _radius){
	radius = _radius;
	axis = _axis;
}
glm::vec3 square_move::get_new_location(){
	auto static_move =  (GLfloat)glm::radians(time_move);
	float now = ((GLfloat)glfwGetTime()*speed + time_move);
	float mv = now - radius*(int)(now / radius);
	float x = 0;
	float y = 0;
	float z = 0;
	int where = (int)(now / radius) % 4;
	side = where + 1;
	if(where == 0){
		if(axis.x != 0){
			x = -radius/2;
			if(axis.y != 0)
				y = mv - radius/2;
			else 
				z = mv - radius/2;
		}else{
			y = -radius/2;
			z = mv - radius/2;
		}
	} else if (where == 1){
		if(axis.x != 0){
			x = mv - radius/2;
			if(axis.y != 0)
				y = radius/2;
			else
				z = radius/2;
		} else {
			y = mv - radius/2;
			z = radius/2;
		}
	} else if (where == 2){
		if(axis.x != 0){
			x = radius/2;
			if(axis.y != 0)
				y = radius/2 - mv;
			else 
				z = radius/2 - mv;
		}else{
			y = radius/2;
			z = radius/2 - mv;
		}
	} else if(where == 3){
		if(axis.x != 0){
			x = radius/2 - mv;
			if(axis.y != 0)
				y = -radius/2;
			else
				z = -radius/2;
		} else {
			y =  radius/2 - mv;
			z = -radius/2;
		}
	}
	//float now_height = this_height*glm::abs(glm::cos()*speed);
	return glm::vec3(x,y,z);
}
glm::mat4 square_move::rotate_for_cool_move(glm::mat4 m){
	glm::vec3 ax;
	if(axis.x != 0)ax.x = 0.0f; else ax.x = 1.0f;
	if(axis.y != 0)ax.y = 0.0f; else ax.y = 1.0f;
	if(axis.z != 0)ax.z = 0.0f; else ax.z = 1.0f; 
	return glm::rotate(m, (GLfloat)glm::radians(90.0*side),ax);
}


//boom_animation
boom_animation::boom_animation(){
	glm::vec3 ax1;
	ax1.x = ((int)std::rand())%1000*10;
	ax1.y = ((int)std::rand())%1000*10;
	ax1.z = ((int)std::rand())%1000*10;
	rot_axis = ax1;
}
boom_animation::boom_animation(glm::vec3 _axis, float _radius){
	radius = radius_start = _radius;
	axis = _axis;
	glm::vec3 ax1;
	ax1.x = ((int)std::rand())%1000*10;
	ax1.y = ((int)std::rand())%1000*10;
	ax1.z = ((int)std::rand())%1000*10;
	rot_axis = ax1;
}
void boom_animation::set_back_plate(float _back_plate){
	back_plate = _back_plate;
}
glm::vec3 boom_animation::get_new_location(){
	float time = (GLfloat)glfwGetTime();
	glm::vec3 new_loc;
	if(pause_time == 0)
		pause_time = time;
	time-=pause_time;
	if(time < 5.1){
		radius = radius_start;
		return glm::vec3(0.0f,0.0f,0.0f);
	}
	if(time < 12.1){
		time-=5.1;
		new_loc.x = -axis.x*radius*time*speed*0.05;
		new_loc.y = -axis.y*radius*time*speed*0.05;
		new_loc.z = -axis.z*radius*time*speed*0.05;
		if(new_loc.z < -back_plate){
			new_loc.z = -back_plate;
			new_loc.y = -back_plate/axis.z*axis.y;
			new_loc.x = -back_plate/axis.z*axis.x;
		}
		return new_loc;
	}
	if(time > 24.1 || radius > 3*radius){
		pause_time += time;
	}
	//std::cout<<time<<"  ";
	//std::cout<<radius<<"\n";
	radius += radius*0.0018;
	new_loc.x = axis.x*radius*glm::abs(glm::cos(time))*speed;
	new_loc.y = axis.y*radius*glm::abs(glm::cos(time))*speed;
	new_loc.z = axis.z*radius*glm::abs(glm::cos(time))*speed;
	if(new_loc.z < -back_plate) {
		new_loc.z = -back_plate;
		new_loc.y = -back_plate/axis.z*axis.y;
		new_loc.x = -back_plate/axis.z*axis.x;
	}
	return new_loc;
}
glm::mat4 boom_animation::rotate_for_cool_move(glm::mat4 m){
	float time = (GLfloat)glfwGetTime();
	float new_loc = axis.z*radius*glm::abs(glm::cos(time))*speed;
	if(new_loc < -back_plate){
		return glm::rotate(m , 0.0f, rot_axis);
	}
	if(pause_time == 0)
		pause_time = time;
	time-=pause_time;
	if(time < 5.1){
		radius = radius_start;
		return glm::rotate(m, 0.0f,rot_axis);
	}
	if(time < 12.1){
		time-=5.1;
		return glm::rotate(m, (GLfloat)glm::radians(time*40.0),rot_axis);
	}
	time-=12.1;
	return glm::rotate(m, (GLfloat)time*radius*4,rot_axis);
}





//dna_animation
dna_animation::dna_animation(){}
dna_animation::dna_animation(glm::vec3 _axis, float _radius){
	radius = _radius;
	axis = _axis;
}
glm::vec3 dna_animation::get_new_location(){
	auto static_move =  (GLfloat)glm::radians(time_move);
	float time = (GLfloat)glfwGetTime();

    int h = height/count_of_curcles;
    auto now = time*0.20 + index*height;
    float mv = now - height*(int)(now / height) - height/2;

	return glm::vec3(mv,
					 radius * glm::sin(time*speed + static_move),
                     radius * glm::cos(time*speed + static_move));
}
void dna_animation::set_height(float _h){
	height = _h;
}

