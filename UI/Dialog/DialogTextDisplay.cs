using Godot;
using System;

public partial class DialogTextDisplay : Control
{
	RichTextLabel _textElement;

	public override void _Ready()
	{
		base._Ready();

		_textElement = GetNode<RichTextLabel>("DialogPanel/RichTextLabel");
	}

	// Indirect our text setting
	public void SetDialog(String dialog)
	{
		_textElement.Text = dialog;
	}
}
