using Godot;
using System;

public partial class NPC : Node3D
{
	public void TriggerInteraction(Player source)
	{
		GD.Print("Interacted with NPC");
		source.SetDialog("Hello!");
	}
}
