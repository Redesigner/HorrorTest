#include "custom_math.h"

#include <godot_cpp/core/math.hpp>

using namespace godot;

float CustomMath::angle_between(float angle1, float angle2)
{
    if (angle1 > 360.0f || angle1 < 0.0f)
    {
        angle1 = Math::wrapf(angle1, 0.0f, 360.0f);
    }
    if (angle2 > 360.0f || angle2 < 0.0f)
    {
        angle2 = Math::wrapf(angle2, 0.0f, 360.0f);
    }

    float deltaAngle = angle2 - angle1;
    if (deltaAngle > 180.0f)
    {
        deltaAngle -= 360.0f;
    }
    else if (deltaAngle < -180.0f)
    {
        deltaAngle += 360.0f;
    }

    return deltaAngle;
}

Vector3 ClampVectorToLength(Vector3 vector, float length)
{
    float lengthSquared = length * length;
    float vectorLengthSquared = vector.length_squared();

    if (vectorLengthSquared > lengthSquared )
    {
        return vector.normalized() * length;
    }
    return vector;
}

Vector3 ConstantInterpToV(Vector3 vectorFrom, Vector3 vectorTo, float rate, float delta)
{
    float movement = rate * delta;
    Vector3 deltaVector = vectorTo - vectorFrom;
    if (deltaVector.length_squared() < movement * movement)
    {
        return vectorTo;
    }
    return vectorFrom + deltaVector.normalized() * movement;
}
Vector2 ConstantInterpToV(Vector2 vectorFrom, Vector2 vectorTo, float rate, float delta)
{
    float movement = rate * delta;
    Vector2 deltaVector = vectorTo - vectorFrom;
    if (deltaVector.length_squared() < movement * movement)
    {
        return vectorTo;
    }
    return vectorFrom + deltaVector.normalized() * movement;
}

bool IsNearlyZero(Vector3 vector, float epsilon = 0.001f)
{
    return vector.length_squared() < epsilon;
}