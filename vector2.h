#ifndef VECTOR2_H
#define VECTOR2_H

struct Vector2{
    float x, y;

    //defult constructor
    Vector2() : x(0.0f), y(0.0f) {}
    //constructors:
    Vector2(float _x, float _y){
        x = _x;
        y = _y;
    }

    //functions; 
        // addition:
        Vector2 operator+(const Vector2& other) const {
            return Vector2(x + other.x, y + other.y);
        }
        //multiplication:
        Vector2 operator*(float scalar) const {
            return Vector2(x * scalar, y * scalar);
        }   



    

};

#endif