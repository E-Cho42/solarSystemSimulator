#ifndef BALL_H
#define BALL_H

#include "vector2.h"
#include "RigidBody.h"


class Ball {
public:
    RigidBody rb;
    float radius;

    Ball();
    Ball(RigidBody _rb, float r);
    
};

#endif