using Godot;
using Godot.Collections;
using System;

public partial class CameraArm : Node3D
{
    [Export]
    private float _armLength = 3.0f;

    [Export]
    private float _armCollisionBuffer = 0.1f;

    private Camera3D _camera;

    public override void _Ready()
    {
        base._Ready();
        _camera = (Camera3D)GetNode("Camera3D");
    }

    public override void _PhysicsProcess(double delta)
    {
        PhysicsDirectSpaceState3D spaceState = GetWorld3D().DirectSpaceState;
        Vector3 cameraDesiredPosition = GlobalTransform.Basis.Z * _armLength;
        PhysicsRayQueryParameters3D ray = PhysicsRayQueryParameters3D.Create(GlobalPosition, GlobalPosition + cameraDesiredPosition);
        Dictionary collisionResult = spaceState.IntersectRay(ray);

        if (collisionResult.Count > 0)
        {
            Vector3 collisionPosition = (Vector3)collisionResult["position"];
            Vector3 deltaPosition = GlobalPosition - collisionPosition;
            Vector3 cameraPosition = new Vector3(0.0f, 0.0f, deltaPosition.Length() - _armCollisionBuffer);
            _camera.Position = cameraPosition;
        }
        else
        {
            _camera.Position = new Vector3(0.0f, 0.0f, _armLength);
        }
    }
}
