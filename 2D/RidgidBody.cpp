#include "RigidBody.h"

RigidBody::RigidBody(float m, Vector2 pos){
    mass = m;
    position = pos;

    velocity = Vector2(0,0);
    acceleration = Vector2(0,0);

    // If mass is 0, we treat it as an unmovable object (inverseMass = 0)
    if (mass != 0) inverseMass = 1.0f / mass;
    else inverseMass = 0;

}

void RigidBody::applyForce(Vector2 force){
    // a = a + F/m  
    acceleration = acceleration + (force * inverseMass);

}

void RigidBody::update(float deltaTime){
    //update function
    velocity = velocity + (acceleration * deltaTime);
    position = position + (velocity * deltaTime);

    acceleration = Vector2(0,0);




}
