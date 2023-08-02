using Godot;
using System;

public partial class Player : Node3D
{
    private Node3D _cameraArm;
    private Node3D _inputVectorDisplay;
    private MeshInstance3D _playerMesh;

    [Export]
    private float _lookSpeedVertical = 1.0f;
    [Export]
    private float _lookSpeedHorizontal = 1.0f;

    [Export]
    private float _walkSpeed = 1.0f;

    [Export]
    private float _turnSpeed = 90.0f;

    private Vector2 _inputVector;

    public override void _Ready()
    {
        _cameraArm = (Node3D) GetNode("CameraArm");
        _inputVectorDisplay = (Node3D) GetNode("InputVectorDisplay");
        _playerMesh = (MeshInstance3D) GetNode("PlayerMesh");

        Input.MouseMode = Input.MouseModeEnum.Captured;
    }

    public override void _Input(InputEvent @event)
    {
        if (@event is InputEventMouseMotion mouseEvent)
        {
            RotateCamera(mouseEvent.Relative);
            return;
        }

        if (@event.IsActionPressed("ui_cancel"))
        {
            GetTree().Quit();
            return;
        }
    }

    public override void _Process(double delta)
    {
        base._Process(delta);

    }

    public override void _PhysicsProcess(double delta)
    {
        base._PhysicsProcess(delta);

        Vector2 inputVector = GetInputVector();
        Vector3 cameraRotation = _cameraArm.Rotation;
        float cameraYaw = -cameraRotation.Y;
        float cos = Mathf.Cos(cameraYaw);
        float sin = Mathf.Sin(cameraYaw);
        _inputVector = new Vector2(cos * inputVector.X - sin * inputVector.Y, sin * inputVector.X + cos * inputVector.Y);

        if (_inputVector.X == 0.0f && _inputVector.Y == 0.0f)
        {
            return;
        }
        Position += new Vector3(_inputVector.X, 0.0f, _inputVector.Y) * (float)(_walkSpeed * delta);

        Vector2 normalizedInput = _inputVector.Normalized();
        float desiredYaw = Mathf.RadToDeg(Mathf.Atan2(normalizedInput.Y, normalizedInput.X));
        float deltaYaw = CustomMath.AngleBetween(-_playerMesh.RotationDegrees.Y - 90.0f, desiredYaw);
        float yawOffset =  (float)Mathf.Sign(deltaYaw) * _turnSpeed * (float)delta;

        // Clamp our rotation this frame so that its magnitude is never larger than the difference between our desired rotation and current rotation
        if (MathF.Abs(yawOffset) > MathF.Abs(deltaYaw))
        {
            yawOffset = deltaYaw;
        }


        Vector3 postPhysicsRotationDegrees = _playerMesh.RotationDegrees;
        postPhysicsRotationDegrees.Y -= yawOffset;
        _playerMesh.RotationDegrees = postPhysicsRotationDegrees;

        _inputVectorDisplay.Position = new Vector3(_inputVector.X, 0.0f, _inputVector.Y);
    }

    public void RotateCamera(Vector2 input)
    {
        Vector3 currentRotation = _cameraArm.RotationDegrees;
        currentRotation.Y += input.X * -_lookSpeedHorizontal;
        currentRotation.X += input.Y * -_lookSpeedVertical;
        currentRotation.X = Mathf.Clamp(currentRotation.X, -89.9f, 89.9f);
        _cameraArm.RotationDegrees = currentRotation;
    }

    public Vector2 GetInputVector()
    {
        Vector2 inputVector;
        inputVector.X = Input.GetAxis("walk_left", "walk_right");
        inputVector.Y = Input.GetAxis("walk_forward", "walk_back");
        float lengthSquared = inputVector.X * inputVector.X + inputVector.Y * inputVector.Y;

        if (lengthSquared > 1.0f)
        {
            inputVector = inputVector.Normalized();
        }
        return inputVector;
    }
}