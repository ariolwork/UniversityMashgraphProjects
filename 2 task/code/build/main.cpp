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
std::vector<object*> screen_saver2(my_color_getter& c_g);
std::vector<object*> screen_saver3(my_color_getter& c_g);
std::vector<object*> screen_saver4(my_color_getter& c_g);

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
	screen_savers.push_back(screen_saver2(c_g));
	screen_savers.push_back(screen_saver3(c_g));
	screen_savers.push_back(screen_saver4(c_g));


	
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


    	program.SetUniform("lightPos", glm::vec3(2.0f, 1.0, 3.0));
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
	}else if (key == GLFW_KEY_4){
  		if(action == GLFW_PRESS){
  			screen_saver_number = 3;
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
	object* a = new square(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.2f, 1.2f, 1.2f));
	a->set_texture("img/2.jpg");
	a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	a->anim.mv = (move*)(new move());
 	a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
 	a->set_color(c_g.get_color_by_name("obsidian"));
 	v.push_back((object*)a);

 	a = new square(glm::vec3(0.0f, 0.01f, 0.0f), glm::vec3(0.84f, 0.84f, 0.84f));
 	a->set_texture("img/2.jpg");
	a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	a->anim.mv = (move*)(new move());
 	a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), 1.2f);
 	a->anim.rot->set_rot_time_move(45);
 	a->set_color(c_g.get_color_by_name("jade"));
 	v.push_back((object*)a);

 	int count_in_line = 10;
 	float height_map[count_in_line][count_in_line];
 	for(int i=0;i<count_in_line;i++){
 		for(int j=0;j<count_in_line;j++){
 			height_map[i][j] = ((float)(std::rand()%100))/100.0f;
 		}
 	}
 	int smooze_level = 1;
 	for (auto i=0;i<count_in_line;i++)
        for (auto j=0;j<count_in_line;j++)
            {   height_map[i][j]=0;
                int kol_vo=0;
                for (auto k=i-1;k<=i+1;k++)
                    {
                      if ((k>-1)&(k<count_in_line))
                        { 
                            for (auto l=j-1;l<=j+1;l++)
                            { if ((l>-1)&(l<count_in_line))
                                { 
                                    if ((k!=i)|(l!=j))
                                    { 
                                height_map[i][j]=height_map[i][j]+height_map[k][l];
                                kol_vo++;
                                    }
                                }
                            }
                        }
                    }
                height_map[i][j]=height_map[i][j]/kol_vo;
            }

 	glm::vec3 _size = glm::vec3(1.0f/(count_in_line+1));
 	float y = 1.0f/(count_in_line+1)/2;
 	for (int i=0;i<count_in_line;i++){
 		for (int j=0;j<count_in_line;j++){
 			float height = ((float)(std::rand()%100))/100.0f;
 			float x = 1.0*i/count_in_line - 1.0f/2;
 			float z = 1.0*j/count_in_line - 1.0f/2;
 			a = new cube(glm::vec3(x, y, z), _size);
 			//a->set_texture("img/1.jpg");
 			a->anim.mv = (move*)(new equalizer(height_map[i][j]));
 			a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
 			a->set_color(c_g.get_random_color());
 			v.push_back((object*)a);
 		}
 	}

 	int count_of_hex = 40;
 	float radius = 1.2f;
 	for(int i=0;i<count_of_hex;i++){
 		a = new hexagon(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		a->anim.mv = (move*)(new square_move(glm::vec3(1.0, 0.0, 1.0), radius));
		a->anim.mv->set_time_move(radius*i*4/count_of_hex);
		a->anim.mv->set_speed(0.4);
 		a->anim.rot = new rotation(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
 		a->set_color(c_g.get_color_by_name("gold"));
 		v.push_back((object*)a);
 	}
 	int count_triangle = 13;
 	for(int i=0;i<count_triangle;i++){
 		a = new triangle(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2f));
 		a->set_texture("img/4.jpg");
		a->anim.mv = (move*)(new curcle_move(glm::vec3(1.0f, 1.0f, 0.0f), 1.5));
		a->anim.mv->set_time_move(360.0f*i/count_triangle);
		a->anim.mv->set_speed(-1.0);
 		a->anim.rot = new rotation(glm::vec3(0.0f, 0.0f, 1.0f), 2.0f);
 		a->set_color(c_g.get_color_by_name("gold"));
 		v.push_back((object*)a);
 	}
 	for(int i=0;i<count_triangle;i++){
 		a = new triangle(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2));
 		a->set_texture("img/4.jpg");
		a->anim.mv = (move*)(new curcle_move(glm::vec3(1.0f, 1.0f, 0.0f), 1.3));
		a->anim.mv->set_time_move(360.0f*i/count_triangle);
 		a->anim.rot = new rotation(glm::vec3(0.0f, 0.0f, 1.0f), -2.0f);
 		a->set_color(c_g.get_color_by_name("chrome"));
 		v.push_back((object*)a);
 	}

 	return v;
}

std::vector<object*> screen_saver2(my_color_getter& c_g){
	std::vector<object*> v;
	object* a;

	int count_in_layer = 10;
	//furst_layer
	float fig_size = 1.0;
	float size = fig_size/(count_in_layer+count_in_layer*0.15);
	float split = (size - fig_size/(count_in_layer))/count_in_layer;

	for(int z1 = 0;z1<count_in_layer;z1++){
		for(int i=0;i<count_in_layer;i++){
			for(int j = 0;j<count_in_layer;j++){
				float x = fig_size*i/count_in_layer  +split*i  -fig_size/2;
				float y = fig_size*j/count_in_layer  +split*j  -fig_size/2;
				float z = fig_size*z1/count_in_layer +split*z1 -fig_size/2;
				glm::vec3 b;
				b.x = glm::cos(x/fig_size);
				b.y = glm::cos(y/fig_size);
				b.z = glm::cos(z/fig_size);
				a = new cube(glm::vec3(x, y, z), glm::vec3(size));
				//a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
				auto t = new boom_animation(glm::vec3(x,y,z));
				t->set_back_plate(3.0);
				a->anim.mv = (move*)(t);
 				a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
 				a->set_color(c_g.get_random_color());
 				v.push_back((object*)a);
 			}
 		}
 	}
 	int count_of_hex = 40;
 	float radius = 3.0;
 	for(int i=0;i<count_of_hex;i++){
 		a = new hexagon(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f));
 		a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
		a->anim.mv = (move*)(new square_move(glm::vec3(1.0, 1.0, 0.0), radius));
		a->anim.mv->set_time_move(radius*i*4/count_of_hex);
		a->anim.mv->set_speed(0.4);
 		a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
 		a->set_color(c_g.get_color_by_name("gold"));
 		v.push_back((object*)a);
 	}

 	a = new square(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(12.0f, 12.0f, 12.0f));
 	a->set_texture("img/5.jpg");
	a->anim.mv = (move*)(new move());
 	a->anim.rot = new rotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.05f);
 	a->set_color(c_g.get_color_by_name("silver"));
 	v.push_back((object*)a);
 	return v;
}

std::vector<object*> screen_saver3(my_color_getter& c_g){
  /*std::vector<object*> v;
	object* a = new model("models/stickman/stickman.OBJ");
	a->set_color(c_g.get_color_by_name("gold"));
	v.push_back((object*)a);*/
	std::vector<object*> v;
	int c_s = 2.0;
	object* a;
	int count = 50;
	for(int i=0; i<count;i++){
		a = new cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		a->anim.mv = (move*)(new dna_animation(glm::vec3(1.0f, 1.0f, 0.0f), 0.5));
 		a->anim.rot = new rotation(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
 		a->anim.mv->set_time_move(360*i*c_s/count);
 		((dna_animation *)(a->anim.mv))->count_of_curcles = c_s;
 		((dna_animation *)(a->anim.mv))->index = (float)i/count;
 		if(i%2 == 1)
 			a->set_color(c_g.get_color_by_name("silver"));
 		else
 			a->set_color(c_g.get_color_by_name("gold"));
 		
 		v.push_back(a);
 	}
 	for(int i=0; i<count;i++){
		a = new cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		a->anim.mv = (move*)(new dna_animation(glm::vec3(1.0f, 1.0f, 0.0f), 0.5));
 		a->anim.rot = new rotation(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
 		a->anim.mv->set_time_move(360*i*c_s/count+180);
 		((dna_animation *)(a->anim.mv))->count_of_curcles = c_s;
 		((dna_animation *)(a->anim.mv))->index = (float)i/count;
 		if(i%2 == 1)
 			a->set_color(c_g.get_color_by_name("silver"));
 		else
 			a->set_color(c_g.get_color_by_name("gold"));
 		v.push_back(a);
 	}
 	a = new cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0, 0.05f, 0.05f));
	a->anim.mv = (move*)(new move());
 	a->anim.rot = new rotation(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
 	a->set_color(c_g.get_color_by_name("silver"));
 	v.push_back(a);

	//box
 	a = new square(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(15.0f, 15.0f, 15.0f));
	a->anim.mv = (move*)(new move());
	a->set_texture("img/6.jpg");
 	a->anim.rot = new rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_color(c_g.get_color_by_name("obsidian"));
 	v.push_back(a);

 	a = new square(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(15.0f, 15.0f, 15.0f));
	a->anim.mv = (move*)(new move());
	a->set_texture("img/6.jpg");
 	a->anim.rot = new rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_color(c_g.get_color_by_name("obsidian"));
 	v.push_back(a);

 	a = new square(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(15.0f, 15.0f, 15.0f));
	a->anim.mv = (move*)(new move());
	a->set_texture("img/6.jpg");
 	a->anim.rot = new rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_rotation(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
 	a->set_color(c_g.get_color_by_name("obsidian"));
 	v.push_back(a);

 	a = new square(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(15.0f, 15.0f, 15.0f));
	a->anim.mv = (move*)(new move());
	a->set_texture("img/6.jpg");
 	a->anim.rot = new rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_rotation(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
 	a->set_color(c_g.get_color_by_name("obsidian"));
 	v.push_back(a);

 	a = new square(glm::vec3(0.0f, -7.5f, 0.0f), glm::vec3(15.0f, 15.0f, 15.0f));
	a->anim.mv = (move*)(new move());
	a->set_texture("img/5.jpg");
 	a->anim.rot = new rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
 	a->set_color(c_g.get_color_by_name("obsidian"));
 	v.push_back(a);
 	return v;
}


std::vector<object*> screen_saver4(my_color_getter& c_g){
	std::vector<object*> v;
	object* a;
	a = new model("model/rose.obj", glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.7f));
 	a->anim.mv = (move*)(new curcle_move(glm::vec3(1.0, 0.0, 1.0), 1.0f));
 	a->anim.rot = new rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.3f);
 	a->set_rotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
 	a->set_color(c_g.get_color_by_name("gold"));
 	v.push_back(a);
 	return v;

}