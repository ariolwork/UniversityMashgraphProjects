//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "LiteMath.h"

//External dependencies
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <random>

static GLsizei WIDTH = 1000, HEIGHT = 800; //размеры окна

using namespace LiteMath;

float3 g_camPos(0, 3, 15);
float  cam_rot[2] = {-0.5f,0.0f};
int    mx = 0, my = 0;
bool com_event[10] = {false, false, false, false, false, false, false, false, false, false};//QWEASD RF Shift animation
int show_type = 0;
int fog = 1;
int shadow_level = 3;
int animation = 0;

void drop_cam(){
    g_camPos = float3(0, 2, 7);
    cam_rot[0] =0; cam_rot[1] = 3.14159265359;
}

void windowResize(GLFWwindow* window, int width, int height)
{
  WIDTH  = width;
  HEIGHT = height;
}

static void mouseMove(GLFWwindow* window, double xpos, double ypos)
{
  xpos *= 0.05f;
  ypos *= 0.05f;

  int x1 = int(xpos);
  int y1 = int(ypos);

  cam_rot[0] -= 0.05f*(y1 - my);	//Изменение угола поворота
  cam_rot[1] -= 0.05f*(x1 - mx);

  mx = int(xpos);
  my = int(ypos);
}

void exec_cam_events(){
	float a = ((com_event[8])?(2):(1));
	if(com_event[0]) cam_rot[1]-=0.05f*a;
	if(com_event[1]) g_camPos = make_float3(g_camPos.x, g_camPos.y, g_camPos.z-0.2f*a);
	if(com_event[2]) cam_rot[1]+=0.05f*a;
	if(com_event[3]) g_camPos = make_float3(g_camPos.x-0.2f*a, g_camPos.y, g_camPos.z);
	if(com_event[4]) g_camPos = make_float3(g_camPos.x, g_camPos.y, g_camPos.z+0.2f*a);
	if(com_event[5]) g_camPos = make_float3(g_camPos.x+0.2f*a, g_camPos.y, g_camPos.z);
	if(com_event[6]) g_camPos = make_float3(g_camPos.x, g_camPos.y+0.2f*a, g_camPos.z);
	if(com_event[7]) g_camPos = make_float3(g_camPos.x, g_camPos.y-0.2f*a, g_camPos.z);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_A){
		if(action == GLFW_PRESS) com_event[3] = true;
		else if (action == GLFW_RELEASE) com_event[3] = false;
	}else if (key == GLFW_KEY_D){
  		if(action == GLFW_PRESS) com_event[5] = true;
		else if (action == GLFW_RELEASE) com_event[5] = false;
	}else if (key == GLFW_KEY_W){
  		if(action == GLFW_PRESS) com_event[1] = true;
		else if (action == GLFW_RELEASE) com_event[1] = false;
	}else if (key == GLFW_KEY_S){
  		if(action == GLFW_PRESS) com_event[4] = true;
		else if (action == GLFW_RELEASE) com_event[4] = false;
	}else if (key == GLFW_KEY_Q){
  		if(action == GLFW_PRESS) com_event[0] = true;
		else if (action == GLFW_RELEASE) com_event[0] = false;
	}else if (key == GLFW_KEY_E){
  		if(action == GLFW_PRESS) com_event[2] = true;
		else if (action == GLFW_RELEASE) com_event[2] = false;
	}else if (key == GLFW_KEY_R){
  		if(action == GLFW_PRESS) com_event[6] = true;
		else if (action == GLFW_RELEASE) com_event[6] = false;
	}else if (key == GLFW_KEY_F){
  		if(action == GLFW_PRESS) com_event[7] = true;
		else if (action == GLFW_RELEASE) com_event[7] = false;
	}else if (key == GLFW_KEY_LEFT_SHIFT){
  		if(action == GLFW_PRESS) com_event[8] = true;
		else if (action == GLFW_RELEASE) com_event[8] = false;
	}else if (key == GLFW_KEY_0){
        show_type = 0;
        fog = 1;
        g_camPos = float3(0, 3, 15);
        cam_rot[0] = -0.5f; cam_rot[1]=0.0f;
	}else if (key == GLFW_KEY_1){
  		show_type = 1;
  		fog = 1;
        g_camPos = float3(0, 3, 15);
        cam_rot[0] = -0.5f; cam_rot[1]=0.0f;
	}else if (key == GLFW_KEY_2){
	    if (action == GLFW_RELEASE){
	    if(show_type<24 && show_type>19){
	        show_type+=1;
	        if(show_type>23){
	            show_type = 20;
	        }
	    } else {
  		    show_type = 20;
  		}
  		fog = 0;
  		}
  		drop_cam();
	}else if (key == GLFW_KEY_3){
	    if (action == GLFW_RELEASE){
  		    com_event[9] = !com_event[9];
        }
	}else if (key == GLFW_KEY_4){
  		if (action == GLFW_RELEASE){
	        fog++;
	        if(fog == 5)
	            fog = 0;
	    }
	}else if (key == GLFW_KEY_5){
  		if (action == GLFW_RELEASE) {
	        shadow_level++;
	        if(shadow_level == 7){
	            shadow_level = 0;
	        }   
	    }
	}else if (key == GLFW_KEY_ESCAPE){
	    glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int initGL()
{
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

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

	//запрашиваем контекст opengl версии 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 
    GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL ray marching sample", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
    glfwSetCursorPosCallback (window, mouseMove);
    glfwSetWindowSizeCallback(window, windowResize);
    glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	//создание шейдерной программы из двух файлов с исходниками шейдеров
	//используется класс-обертка ShaderProgram
	std::unordered_map<GLenum, std::string> shaders;
	shaders[GL_VERTEX_SHADER]   = "vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "fragment.glsl";
	ShaderProgram program(shaders); GL_CHECK_ERRORS;
    
  glfwSwapInterval(1); // force 60 frames per second
  
  //Создаем и загружаем геометрию поверхности
  //
  GLuint g_vertexBufferObject;
  GLuint g_vertexArrayObject;
  {
 
    float quadPos[] =
    {
      -1.0f,  1.0f,	// v0 - top left corner
      -1.0f, -1.0f,	// v1 - bottom left corner
      1.0f,  1.0f,	// v2 - top right corner
      1.0f, -1.0f	  // v3 - bottom right corner
    };
    
    g_vertexBufferObject = 0;
    GLuint vertexLocation = 0; // simple layout, assume have only positions at location = 0

    glGenBuffers(1, &g_vertexBufferObject); GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObject); GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), (GLfloat*)quadPos, GL_STATIC_DRAW);GL_CHECK_ERRORS;

    glGenVertexArrays(1, &g_vertexArrayObject); GL_CHECK_ERRORS;
    glBindVertexArray(g_vertexArrayObject); GL_CHECK_ERRORS;

    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObject); GL_CHECK_ERRORS;
    glEnableVertexAttribArray(vertexLocation); GL_CHECK_ERRORS;
    glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0); GL_CHECK_ERRORS;

    glBindVertexArray(0);
  }

    glfwSwapInterval(1);
	//цикл обработки сообщений и отрисовки сцены каждый кадр
	float step = 1.0f/1200;
	int number = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		exec_cam_events();
		if (com_event[9]){
		    cam_rot[1] -= 0.05f;
		}
		//очищаем экран каждый кадр
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);               GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;

        program.StartUseShader();                           GL_CHECK_ERRORS;

        float4x4 camRotMatrix   = mul(rotate_Y_4x4(-cam_rot[1]), rotate_X_4x4(+cam_rot[0]));
        float4x4 camTransMatrix = translate4x4(g_camPos);
        float4x4 rayMatrix      = mul(camRotMatrix, camTransMatrix);
        program.SetUniform("g_rayMatrix", rayMatrix);
        program.SetUniform("g_bgColor", float4(0,0,0,1));
        program.SetUniform("g_screenWidth" , WIDTH);
        program.SetUniform("g_screenHeight", HEIGHT);
        program.SetUniform("show_type", show_type);
        program.SetUniform("fog", fog);
        program.SetUniform("shadow_level", shadow_level);
        // очистка и заполнение экрана цветом
        //
        glViewport  (0, 0, WIDTH, HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // draw call
        //
        glBindVertexArray(g_vertexArrayObject); GL_CHECK_ERRORS;
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  GL_CHECK_ERRORS;  // The last parameter of glDrawArrays is equal to VS invocations
    
        program.StopUseShader();
    
   		glfwSwapBuffers(window); 
	}

	//очищаем vboи vao перед закрытием программы
  //
	glDeleteVertexArrays(1, &g_vertexArrayObject);
    glDeleteBuffers(1, &g_vertexBufferObject);

	glfwTerminate();
	return 0;
}
