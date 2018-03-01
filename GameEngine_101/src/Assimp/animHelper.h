#pragma once
#include <glm\mat4x4.hpp>
#include <assimp/matrix4x4.h>

// Returns a GLM 4x4 matrix from an AiMatrix4x4
glm::mat4 AIMatrixToGLMMatrix(const aiMatrix4x4 &mat);