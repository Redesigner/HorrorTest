using Godot;
using Godot.Collections;
using System;

public partial class Player : CharacterBody3D
{
	private Node3D _cameraArm;
	private Node3D _inputVectorDisplay;
	private MeshInstance3D _playerMesh;
	private Area3D _interactVolume;

	private RichTextLabel _debugText;

	[Export]
	private float _lookSpeedVertical = 1.0f;
	[Export]
	private float _lookSpeedHorizontal = 1.0f;

	/// Max walk speed in units/s
	[Export]
	private float _maxWalkSpeed = 4.0f;

	/// Acceleration rate in units/s2
	[Export]
	private float _groundAcceleration = 8.0f;

	/// Ground friction acceleration in units/s2
	[Export]
	private float _groundFriction = 16.0f;

	/// Turning speed in degrees/s
	[Export]
	private float _turnSpeed = 90.0f;

	private Vector2 _inputVector;

	[Signal]
	public delegate void DialogChangedEventHandler(String dialog);

	public override void _Ready()
	{
		_cameraArm = (Node3D) GetNode("CameraArm");
		_inputVectorDisplay = (Node3D) GetNode("InputVectorDisplay");
		_playerMesh = (MeshInstance3D) GetNode("PlayerMesh");
		_interactVolume = (Area3D)GetNode("PlayerMesh/InteractVolume");

		_debugText = (RichTextLabel)GetNode("DebugText");

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

		if (@event.IsActionPressed("interact"))
		{
			Interact();
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

		// Update on physics tick to prevent stuttering
		UpdateInput();

		if (InputPressed())
		{
			Vector2 desiredVelocity = _maxWalkSpeed * _inputVector;
			Vector2 currentGroundVelocity = new Vector2(Velocity.X, Velocity.Z);
			Vector2 groundVelocity = CustomMath.ConstantInterpToV(currentGroundVelocity, desiredVelocity, _groundAcceleration, (float)delta);
			Velocity = new Vector3(groundVelocity.X, Velocity.Y, groundVelocity.Y);
		}
		else if (IsOnFloor())
		{
            Vector2 desiredVelocity = Vector2.Zero;
            Vector2 currentGroundVelocity = new Vector2(Velocity.X, Velocity.Z);
			Vector2 groundVelocity = CustomMath.ConstantInterpToV(currentGroundVelocity, desiredVelocity, _groundAcceleration, (float)delta);
            Velocity = new Vector3(groundVelocity.X, Velocity.Y, groundVelocity.Y);
            if (CustomMath.IsNearlyZero(Velocity))
			{
				Velocity = Vector3.Zero;
			}
		}
		// MoveAndCollide(Velocity * (float)delta);
		Velocity += new Vector3(0.0f, -9.8f * (float)delta, 0.0f);
		MoveAndSlide();
		_debugText.Text = "Velocity: " + Velocity.Length() + " m/s";
		if (InputPressed())
		{
			_debugText.Text += "\nInput currently pressed.";
		}
		if (IsOnFloor())
		{
			_debugText.Text += "\nCurrently on the floor.";
		}


		LookAtWalkDirection(delta);
		if (Position.Y < -100.0f)
		{
			Position = new Vector3(0.0f, 5.0f, 0.0f);
			Velocity = Vector3.Zero;
		}
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
		Vector2 inputVector = Input.GetVector("walk_left", "walk_right", "walk_forward", "walk_back");
		// inputVector.X = Input.GetAxis("walk_left", "walk_right");
		// inputVector.Y = Input.GetAxis("walk_forward", "walk_back");
		return inputVector;
	}

	private void UpdateInput()
	{
		Vector2 inputVector = GetInputVector();
		Vector3 cameraRotation = _cameraArm.Rotation;
		float cameraYaw = -cameraRotation.Y;
		float cos = Mathf.Cos(cameraYaw);
		float sin = Mathf.Sin(cameraYaw);
		_inputVector = new Vector2(cos * inputVector.X - sin * inputVector.Y, sin * inputVector.X + cos * inputVector.Y);

		_inputVectorDisplay.Position = new Vector3(_inputVector.X, _inputVectorDisplay.Position.Y, _inputVector.Y);

		Vector2 cameraInputVector = Input.GetVector("look_left", "look_right", "look_up", "look_down");
		RotateCamera(cameraInputVector);
	}

	private bool InputPressed()
	{
		return !(_inputVector.X == 0.0f && _inputVector.Y == 0.0f);
	}

	private void LookAtWalkDirection(double delta)
	{
		if (!InputPressed())
		{
			return;
		}
		Vector2 normalizedDirection = _inputVector.Normalized();
		float desiredYaw = Mathf.RadToDeg(Mathf.Atan2(normalizedDirection.Y, normalizedDirection.X));
		float deltaYaw = CustomMath.AngleBetween(-_playerMesh.RotationDegrees.Y - 90.0f, desiredYaw);
		float yawOffset = (float)Mathf.Sign(deltaYaw) * _turnSpeed * (float)delta;

		// Clamp our rotation this frame so that its magnitude is never larger than the difference between our desired rotation and current rotation
		if (MathF.Abs(yawOffset) > MathF.Abs(deltaYaw))
		{
			yawOffset = deltaYaw;
		}


		Vector3 postPhysicsRotationDegrees = _playerMesh.RotationDegrees;
		postPhysicsRotationDegrees.Y -= yawOffset;
		_playerMesh.RotationDegrees = postPhysicsRotationDegrees;
	}

	private void Interact()
	{
		Array<Area3D> hitVolumes = _interactVolume.GetOverlappingAreas();
		foreach (Area3D volume in hitVolumes)
		{
			if (volume.Owner is not NPC)
			{
				continue;
			}

			NPC hitNpc = (NPC)volume.Owner;
			hitNpc.TriggerInteraction(this);
		}
	}

	public void SetDialog(String dialog)
	{
		EmitSignal(SignalName.DialogChanged, dialog);
	}
}
