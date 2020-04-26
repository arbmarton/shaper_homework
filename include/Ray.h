#pragma once

#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)

namespace shaper {

struct Ray
{
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

}  // namespace shaper