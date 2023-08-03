using Godot;
using System;

public class CustomMath
{
	public static float AngleBetween(float angle1, float angle2)
	{
		if (angle1 > 360.0f || angle1 < 0.0f)
		{
			angle1 = Mathf.Wrap(angle1, 0.0f, 360.0f);
		}
		if (angle2 > 360.0f || angle2 < 0.0f)
		{
			angle2 = Mathf.Wrap(angle2, 0.0f, 360.0f);
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

		// GD.Print("Angle between '", angle1, "' and '", angle2, "' is '", deltaAngle, "'");
		return deltaAngle;
	}

	public static Vector3 ClampVectorToLength(Vector3 vector, float length)
	{
		float lengthSquared = length * length;
		float vectorLengthSquared = vector.LengthSquared();

		if (vectorLengthSquared > lengthSquared )
		{
			return vector.Normalized() * length;
		}
		return vector;
	}

	public static Vector3 ConstantInterpToV(Vector3 vectorFrom, Vector3 vectorTo, float rate, float delta)
	{
		Vector3 deltaVector = vectorTo - vectorFrom;
		deltaVector.Normalized();
		return vectorFrom + deltaVector * rate * delta;
	}

	public static bool IsNearlyZero(Vector3 vector, float epsilon = 0.001f)
	{
		return vector.LengthSquared() < epsilon;
	}
}
