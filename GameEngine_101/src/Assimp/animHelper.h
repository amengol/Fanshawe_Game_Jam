#pragma once
#include <glm\mat4x4.hpp>
#include <assimp/matrix4x4.h>

// Used for character animations
enum eCharacterAnim
{
    IDLE,
    WALKING,
    LEFT_STRAFE,
    RIGHT_STRAFE,
    WALKING_BACKWARDS,
    RUN,
    RUN_FORWARD,
    LEFT_STRAFE_RUN,
    RIGHT_STRAFE_RUN,
    TURN_LEFT_90,
    TURN_RIGHT_90,
    TURN_LEFT_180,
    TURN_RIGHT_180,
    JUMP,
    JUMP_FORWARD_WALKING,
    JUMP_FORWARD,
    TRICK,
    VIOLENT_TRICK,
    RUNAWAY,
    LEFT_CROSS_PUNCH,
    RIGHT_CROSS_PUNCH,
    RIGHT_KICKING,
    PRAYING,
    DYING_RISING
};

// Returns a GLM 4x4 matrix from an AiMatrix4x4
glm::mat4 AIMatrixToGLMMatrix(const aiMatrix4x4 &mat);