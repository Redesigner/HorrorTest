using Godot;
using System;

public partial class ActiveUI : Control
{
	private DialogTextDisplay _dialogTextDisplay;

	private String _currentText;

	public override void _Ready()
	{
		base._Ready();

		_dialogTextDisplay = (DialogTextDisplay) GetNode("DialogTextDisplay");
	}

	public void UpdateText(String text)
	{
		_dialogTextDisplay.SetDialog(text);
	}
}
