#ifndef RigidBody_H
#define RigidBody_H

#include "vector2.h"
 
class RigidBody{

    public:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration; 

    float mass;
    float inverseMass;
    
    //constructor 
    RigidBody(float m, Vector2 pos);

    // functions:
    //update function runs every frame
    void update(float deltaTime);

    //apply force 
    void applyForce(Vector2 force);

};


#endif
