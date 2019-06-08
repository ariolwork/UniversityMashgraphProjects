#ifndef COLOR_H
#define COLOR_H

#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <glm/gtc/type_ptr.hpp>


struct Color
{
	glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    Color();
    Color(glm::vec3 a, glm::vec3 d, glm::vec3 s, float sh):ambient(a),diffuse(d),specular(s),shininess(sh){};
	
};


class my_color_getter{
    std::map<std::string, Color> colors;
    std::vector<std::string> color_names;
public:
    my_color_getter();

    Color get_random_color();
    Color get_color_by_name(std::string name);
    Color get_color();
};








#endif