#ifndef NIGHTMARE_CUSTOM_MATH_H
#define NIGHTMARE_CUSTOM_MATH_H

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace godot
{
class CustomMath
{
    CustomMath() = delete;

public:
    static float angle_between(float angle1, float angle2);

    static Vector3 clamp_vector_to_length(Vector3 vector, float length);

    static Vector3 constant_interp_to_vector(Vector3 vectorFrom, Vector3 vectorTo, float rate, double delta);

    static Vector2 constant_interp_to_vector(Vector2 vectorFrom, Vector2 vectorTo, float rate, double delta);

    static bool is_nearly_zero(Vector3 vector, float epsilon = 0.001f);
};
}
#endif