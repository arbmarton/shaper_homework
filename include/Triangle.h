#pragma once

#include "Ray.h"

#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)

namespace shaper {


struct Triangle
{
    glm::vec3 m_a;
    glm::vec3 m_b;
    glm::vec3 m_c;
    glm::vec3 m_textureA;
    glm::vec3 m_textureB;
    glm::vec3 m_textureC;
    glm::vec3 m_normalA;  // normals dont need to be unit vectors!
    glm::vec3 m_normalB;
    glm::vec3 m_normalC;

    // This gets the surface normal, by creating a vector orthogonal to the surface,
    // and determining if its the correct one by comparing it to the average vertex normal.
    glm::vec3 getSurfaceNormal() const;

    // Apply a generic transformation matrix to the positions and normals
    void applyTransformation(const glm::mat4& m);

    float area() const;

    // https://stackoverflow.com/questions/1406029/how-to-calculate-the-volume-of-a-3d-mesh-object-the-surface-of-which-is-made-up
    float calculateSignedVolume() const;

    bool intersectsWith(const Ray& ray) const;
};

}  // namespace shaper