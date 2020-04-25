#pragma once

#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)


struct Triangle
{
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
    glm::vec3 textureA;
    glm::vec3 textureB;
    glm::vec3 textureC;
    glm::vec3 normalA;
    glm::vec3 normalB;
    glm::vec3 normalC;
};