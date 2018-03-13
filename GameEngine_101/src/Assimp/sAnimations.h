#pragma once;
#include <string>;

// The types of animations
enum CharacterAnimationTypes
{
    IDLE,
    WALKING,
    WALKING_BACKWARDS,
    RUNNING,
    JUMP,
    JUMP_FORWARD,
    LEFT_STRAFE,
    LEFT_STRAFE_WALKING,
    LEFT_TURN,
    LEFT_TURN_90,
    RIGHT_STRAFE,
    RIGHT_STRAFE_WALKING,
    RIGHT_TURN,
    RIGHT_TURN_90
};

// Stores the name of all supported animations
struct sAnimations
{
    std::string idle;
    std::string walking;
    std::string walking_backwards;
    std::string running;
    std::string jump;
    std::string jump_forward;
    std::string left_strafe;
    std::string left_strafe_walking;
    std::string left_turn;
    std::string left_turn_90;
    std::string right_strafe;
    std::string right_strafe_walking;
    std::string right_turn;
    std::string right_turn_90;
};