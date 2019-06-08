//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "object.h"
#include "color.h"
#include "animation.h"
//External dependencies
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static const GLsizei WIDTH = 1000, HEIGHT = 1000; //размеры окна
int screen_saver_number = 0;
int start_carnaval = 0;

int initGL();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void free_screen_saver(std::vector<object*> a);

std::vector<object*> screen_saver1(my_color_getter& c_g);

int main(int argc, char** argv)
{
	std::srand(1332);
	if(!glfwInit())
    return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 
  	GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL basic sample", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetKeyCallback(window, key_callback);
	if(initGL() != 0) 
		return -1;
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();
	std::unordered_map<GLenum, std::string> shaders;
	shaders[GL_VERTEX_SHADER]   = "vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "fragment.glsl";
	ShaderProgram program(shaders); GL_CHECK_ERRORS;
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
  	
	std::vector<std::vector<object*> > screen_savers;

	my_color_getter c_g;//color getter

	//load_screen_savers
	screen_savers.push_back(screen_saver1(c_g));
	


	
	glm::mat4 projection = glm::perspective( 45.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
		glViewport  (0, 0, WIDTH, HEIGHT);                  GL_CHECK_ERRORS;
    	program.StartUseShader();                           GL_CHECK_ERRORS;

    	if(start_carnaval == 1){
			GLfloat radius = 3.0f;
			GLfloat camX = sin(glfwGetTime()) * radius;
			GLfloat camZ = cos(glfwGetTime()) * radius;
			cameraPos = glm::vec3(camX, 1.0, camZ);
			cameraFront = -cameraPos;
			cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
    	} else {
			cameraPos = glm::vec3(0.0f, 1.0f,  3.0f);
			cameraFront = glm::vec3(0.0f, -0.8f, -3.0f);
			cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
		}
		auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


    	program.SetUniform("lightPos", glm::vec3(0.0f, 3.0, -4.0));
    	program.SetUniform("viewPos", cameraPos);
    	program.SetUniform("projection", projection);
    	program.SetUniform("view", view);

    	for(auto& i: screen_savers[screen_saver_number]){
    		i->draw(program);
    	}

    	program.StopUseShader();
		glfwSwapBuffers(window); 
	}
	for(auto& i: screen_savers)
		free_screen_saver(i);
	
	glfwTerminate();
	return 0;
}


int initGL() {
	int res = 0;
	//грузим функции opengl через glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: "  << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE){
	    glfwSetWindowShouldClose(window, GL_TRUE);
	}else if (key == GLFW_KEY_9){
  		if(action == GLFW_PRESS) start_carnaval = 1-start_carnaval;
	}else if (key == GLFW_KEY_0 || key == GLFW_KEY_1){
  		if(action == GLFW_PRESS){
  			screen_saver_number = 0;
  			//start_carnaval = 0;
  		}
	}else if (key == GLFW_KEY_2){
  		if(action == GLFW_PRESS){
  			screen_saver_number = 1;
  			//start_carnaval = 1;
  		}
	}else if (key == GLFW_KEY_3){
  		if(action == GLFW_PRESS){
  			screen_saver_number = 2;
  			//start_carnaval = 0;
  		}
	}
}

void free_screen_saver(std::vector<object*> a){
	for(auto &i: a){
		free(i->anim.mv);
		free(i->anim.rot);
		free(i);
	}
}


std::vector<object*> screen_saver1(my_color_getter& c_g){
	std::vector<object*> v;
	object* a;

 	a = new square(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	a->anim.mv = (move*)(new move());
	a->set_texture("img/desk.jpg");
 	a->anim.rot = new rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
 	a->set_color(c_g.get_color_by_name("silver"));
 	v.push_back(a);

	for(int i=0;i<3;i++){
		a = new hexagon(glm::vec3(-2.0f, 0.01f*i, -2.5f), glm::vec3(1.5f, 1.5f, 1.5f));
		a->anim.mv = (move*)(new move());
		a->set_texture("img/mramor.jpg");
 		a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.5f);
 		a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
 		a->set_color(c_g.get_color_by_name("silver"));
 		v.push_back(a);
	} 	
	a = new model("mesh/House.obj", glm::vec3(-2.0, 0.015, -2.5), glm::vec3(0.25f));
 	a->anim.mv = (move*)(new move());
 	a->set_texture("img/mramor.jpg");
 	a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.5f);
 	a->set_rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
 	a->set_color(c_g.get_color_by_name("gold"));
 	v.push_back(a);


 	for(int i=0;i<3;i++){
		a = new hexagon(glm::vec3(2.0f, 0.01f*i, -2.5f), glm::vec3(1.5f, 1.5f, 1.5f));
		a->anim.mv = (move*)(new move());
		a->set_texture("img/mramor.jpg");
 		a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), -0.5f);
 		a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
 		a->set_color(c_g.get_color_by_name("silver"));
 		v.push_back(a);
	} 	
	a = new model("mesh/01Alocasia_obj.obj", glm::vec3(2.0, 0.015, -2.5), glm::vec3(0.5f));
 	a->anim.mv = (move*)(new move());
 	a->set_texture("img/mramor.jpg");
 	a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), -0.5f);
 	a->set_rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
 	a->set_color(c_g.get_color_by_name("gold"));
 	v.push_back(a);

 	a = new cube(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.2f, 1.2f, 1.2f));
	a->anim.mv = (move*)(new curcle_move(glm::vec3(1.0, 0.0, 1.0), 1.0f));
	a->set_texture("img/laminate.jpg");
 	a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
 	a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_color(c_g.get_color_by_name("bronze"));
 	v.push_back(a);

 	a = new model("mesh/sls_amg.obj", glm::vec3(0.0, -1.0+0.7, 0.0), glm::vec3(0.4f));
 	a->anim.mv = (move*)(new curcle_move(glm::vec3(1.0, 0.0, 1.0), 1.0f));
 	a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
 	a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 270.0f);
 	a->set_color(c_g.get_color_by_name("obsidian"));
 	v.push_back(a);


 	a = new square(glm::vec3(0.0f, -1.0f, -5.0f), glm::vec3(10.0f));
	a->anim.mv = (move*)(new move());
	a->set_texture("img/2.jpg");
 	a->anim.rot = new rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_color(c_g.get_color_by_name("silver"));
 	v.push_back(a);

 	a = new square(glm::vec3(-5.0f, -1.0f, 0.0f), glm::vec3(10.0f));
	a->anim.mv = (move*)(new move());
	a->set_texture("img/2.jpg");
 	a->anim.rot = new rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_rotation(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
 	a->set_color(c_g.get_color_by_name("silver"));
 	v.push_back(a);

 	a = new square(glm::vec3(5.0f, -1.0f, 0.0f), glm::vec3(10.0f));
	a->anim.mv = (move*)(new move());
	a->set_texture("img/2.jpg");
 	a->anim.rot = new rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_rotation(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
 	a->set_color(c_g.get_color_by_name("silver"));
 	v.push_back(a);

 	return v;
}

