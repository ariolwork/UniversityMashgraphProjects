#include <string>

#include "color.h"
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>


Color::Color(){
    ambient = glm::vec3(0.05375f, 0.05f, 0.06625f);
    diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f);
    specular = glm::vec3(0.332741f, 0.328634f, 0.346435f);
    shininess = 0.3f;
};


my_color_getter::my_color_getter(){
	std::srand(133);
	colors["emerald"] = Color(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f);
	color_names.push_back("emerald");
	colors["jade"] = Color(glm::vec3(0.135f, 0.2225f, 0.1575f), glm::vec3(0.54f, 0.89, 0.63f), glm::vec3(0.316228f, 0.316228f, 0.316228f), 0.1f);
	color_names.push_back("jade");
	colors["obsidian"] = Color();
	color_names.push_back("obsidian");
	colors["pearl"] = Color(glm::vec3(0.25f, 0.20725f, 0.20725f), glm::vec3(1.0f, 0.829f, 0.829f), glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f);
	color_names.push_back("pearl");
	colors["ruby"] = Color(glm::vec3(0.1745f, 0.01175f, 0.01175f), glm::vec3(0.61424f, 0.04136f, 0.04136f), glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f);
	color_names.push_back("ruby");
	colors["turquoise"] = Color(glm::vec3(0.1f, 0.18725f, 0.1745f), glm::vec3(0.396f, 0.74151f, 0.69102f), glm::vec3(0.297254f, 0.30829f, 0.306678f), 0.1f);
	color_names.push_back("turquoise");
	colors["brass"] = Color(glm::vec3(0.329412f, 0.223529f, 0.027451f), glm::vec3(0.780392f, 0.568627f, 0.113725f), glm::vec3(0.992157f, 0.941176f, 0.807843f), 0.21794872f);
	color_names.push_back("brass");
	colors["bronze"] = Color(glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f), glm::vec3(0.393548f, 0.271906f, 0.166721f), 0.2f);
	color_names.push_back("bronze");
	colors["chrome"] = Color(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6f);
	color_names.push_back("chrome");
	colors["copper"] = Color(glm::vec3(0.19125f, 0.0735f, 0.0225f), glm::vec3(0.7038f, 0.27048f, 0.0828f), glm::vec3(0.256777f, 0.137622f, 0.086014f), 0.1f);
	color_names.push_back("copper");
	colors["gold"] = Color(glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.628281f, 0.555802f, 0.366065f), 0.4f);
	color_names.push_back("gold");
	colors["silver"] = Color(glm::vec3(0.19225f, 0.19225f, 0.19225f), glm::vec3(0.50754f, 0.50754f, 0.50754f), glm::vec3(0.508273f, 0.508273f, 0.508273f), 0.4f);
	color_names.push_back("silver");

	//plastic_colors
	colors["black"] = Color(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.5f, 0.5f, 0.5f), 0.25f);
	color_names.push_back("black");
	colors["green"] = Color(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.35f, 0.1f), glm::vec3(0.45f, 0.55f, 0.45f), 0.25f);
	color_names.push_back("green");
	colors["red"] = Color(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.7f, 0.6f, 0.6f), 0.25f);
	color_names.push_back("red");
	colors["white"] = Color(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.55f, 0.55, 0.55f), glm::vec3(0.7f, 0.7f, 0.7f), 0.25f);
	color_names.push_back("white");
	colors["yellow"] = Color(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.6f, 0.6f, 0.6f), 0.25f);
	color_names.push_back("yellow");
	colors["blue"] = Color(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.0f, 0.35f), glm::vec3(0.45f, 0.55f, 0.45f), 0.25f);
	color_names.push_back("blue");
};


Color my_color_getter::get_color_by_name(std::string name){
	return colors[name];
}
Color my_color_getter::get_random_color(){
	return get_color_by_name(color_names[std::rand()%color_names.size()]);
}
Color my_color_getter::get_color(){
	return colors["obsidian"];
}