using Godot;
using System;

public partial class NPC : Node3D
{
	[Export]
	private String _dialogText;

	public void TriggerInteraction(Player source)
	{
		source.SetDialog(_dialogText);
	}
}
