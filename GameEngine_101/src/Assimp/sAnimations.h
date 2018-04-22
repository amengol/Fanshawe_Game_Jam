#pragma once;
#include <string>;

// Stores the name of all supported animations
struct sAnimations
{
    std::string idle;
    std::string walking;
    std::string walking_backwards;
    std::string running;
    std::string jump;
    std::string jump_forward_walking;
    std::string jump_forward_running;
    std::string left_strafe;
    std::string left_strafe_running;
    std::string left_turn;
    std::string left_turn_90;
    std::string right_strafe;
    std::string right_strafe_running;
    std::string right_turn;
    std::string right_turn_90;
    std::string trick;
    std::string violent_trick;
    std::string runaway;
    std::string left_cross_punch;
    std::string right_cross_punch;
    std::string right_kicking;
    std::string praying;
    std::string dying;
    std::string stunned;
    std::string slash_01;
    std::string slash_02;
};