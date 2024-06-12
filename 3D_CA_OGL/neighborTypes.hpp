#ifndef NEIGHBORTYPES_H
#define NEIGHBORTYPES_H

#include <glm/glm.hpp>
#include <vector>

std::vector<glm::vec3> VON_NEUMANN = {
    glm::vec3(1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, -1, 0),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, -1),
};

std::vector<glm::vec3> MOORE = {
    glm::vec3(-1, 0, 0),
    glm::vec3(-1, 1, 0),
    glm::vec3(-1, -1, 0),
    glm::vec3(-1, 0, 1),
    glm::vec3(-1, 0, -1),
    glm::vec3(-1, 1, 1),
    glm::vec3(-1, 1, -1),
    glm::vec3(-1, -1, 1),
    glm::vec3(-1, -1, -1),
    glm::vec3(0, 1, 0),
    glm::vec3(0, -1, 0),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, -1),
    glm::vec3(0, 1, 1),
    glm::vec3(0, -1, 1),
    glm::vec3(0, 1, -1),
    glm::vec3(0, -1, -1),
    glm::vec3(1, 0, 0),
    glm::vec3(1, 1, 0),
    glm::vec3(1, -1, 0),
    glm::vec3(1, 0, 1),
    glm::vec3(1, 0, -1),
    glm::vec3(1, 1, 1),
    glm::vec3(1, 1, -1),
    glm::vec3(1, -1, 1),
    glm::vec3(1, -1, -1)
};

#endif