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
    glm::vec3 textureA;
    glm::vec3 textureB;
    glm::vec3 textureC;
    glm::vec3 normalA;  // normals dont need to be unit vectors!
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

        const glm::vec3 ab = m_b - m_a;
        const glm::vec3 ac = m_c - m_a;

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

    void applyTransformation(const glm::mat4& m)
    {
        m_a = m * glm::vec4(m_a, 1.0f);
        m_b = m * glm::vec4(m_b, 1.0f);
        m_c = m * glm::vec4(m_c, 1.0f);
        normalA = m * glm::vec4(normalA, 1.0f);
        normalB = m * glm::vec4(normalB, 1.0f);
        normalC = m * glm::vec4(normalC, 1.0f);
    }

    // https://math.stackexchange.com/questions/128991/how-to-calculate-the-area-of-a-3d-triangle
    float area() const
    {
        const glm::vec3 ab = m_b - m_a;
        const glm::vec3 ac = m_c - m_a;

        const float lengthsMultiplied = glm::length(ab) * glm::length(ac);

        const float costheta = glm::dot(ab, ac) / lengthsMultiplied;

        return 0.5f * sqrt(1 - costheta * costheta) * lengthsMultiplied;
    }

    // https://stackoverflow.com/questions/1406029/how-to-calculate-the-volume-of-a-3d-mesh-object-the-surface-of-which-is-made-up
    float calculateSignedVolume() const
    {
        const float v321 = m_c.x * m_b.y * m_a.z;
        const float v231 = m_b.x * m_c.y * m_a.z;
        const float v312 = m_c.x * m_a.y * m_b.z;
        const float v132 = m_a.x * m_c.y * m_b.z;
        const float v213 = m_b.x * m_a.y * m_c.z;
        const float v123 = m_a.x * m_b.y * m_c.z;
        return (1.0f / 6.0f) * (-v321 + v231 + v312 - v132 - v213 + v123);
    }

    // Source: "sünis könyv" from Szirmay
    bool intersectsWith(const Ray& ray) const
    {
        const glm::vec3 ab = m_b - m_a;
        const glm::vec3 ac = m_c - m_a;

        const glm::vec3 triangleNormal = glm::cross(ab, ac);

        // Check if the plane the triangle takes up is in the way of the ray
        const float dist = glm::dot(triangleNormal, m_a - ray.m_origin) / glm::dot(triangleNormal, ray.m_direction);
        if (dist < 0)
        {
            return false;
        }

        // Calculate the point of intersection between the plane and the ray
        const glm::vec3 p = ray.m_origin + ray.m_direction * dist;

        // Check if the point is inside the triangle
        return glm::dot(glm::cross(m_b - m_a, p - m_a), triangleNormal) >= 0 && glm::dot(glm::cross(m_c - m_b, p - m_b), triangleNormal) >= 0
               && glm::dot(glm::cross(m_a - m_c, p - m_c), triangleNormal) >= 0;
    }
};

}  // namespace shaper