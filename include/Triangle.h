#pragma once

#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)

namespace shaper
{


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

    // This gets the surface normal, by creating a vector orthogonal to the surface,
    // and determining if its the correct one by comparing it to the average vertex normal.
    glm::vec3 getSurfaceNormal() const
    {
        const glm::vec3 averageNormal{ (normalA.x + normalB.x + normalC.x) / 3, (normalA.y + normalB.y + normalC.y) / 3,
                                       (normalA.z + normalB.z + normalC.z) / 3 };

        // If the normals are nullvectors just return here
        if (averageNormal == glm::vec3{ 0, 0, 0 })
        {
            return glm::vec3{ 0, 0, 0 };
        }

        const glm::vec3 ab = b - a;
        const glm::vec3 ac = c - a;

        const glm::vec3 normal = glm::cross(ab, ac);
        if (glm::dot(normal, averageNormal) > 0)  // If the vectors look in the same direction its okay, else flip our calculated normal
        {
            return glm::normalize(normal);
        }
        else
        {
            return glm::normalize(-1.0f * normal);
        }
    }
};

}