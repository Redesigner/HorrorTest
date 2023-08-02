using Godot;
using System;

public partial class TestLevel : Node3D
{
    Node3D _player;

    public override void _Ready()
    {
        _player = (Node3D)GetNode("Player");
    }
}
