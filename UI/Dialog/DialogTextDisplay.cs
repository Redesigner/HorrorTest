using Godot;
using System;

public partial class DialogTextDisplay : Control
{
	private RichTextLabel _textElement;

	private String _pendingText;
	private float _currentTick = 0.0f;
	private int _currentCharIndex = 0;
	private bool _textAnimating = false;

    /// How many characters are displayed per second
    [Export]
	private float _characterRate = 10.0f;

	public override void _Ready()
	{
		base._Ready();

		_textElement = GetNode<RichTextLabel>("DialogPanel/RichTextLabel");
	}

    public override void _Process(double delta)
    {
        base._Process(delta);

		if (!_textAnimating)
		{
			return;
		}

		_currentTick += (float) delta * _characterRate;
		if (_currentTick >= 1.0f)
		{
			_textElement.Text += _pendingText[_currentCharIndex];
			_currentTick--;
			if (_currentCharIndex >= _pendingText.Length - 1)
			{
				_textAnimating = false;
				return;
			}
			_currentCharIndex++;
		}
    }

    // Indirect our text setting
    public void SetDialog(String dialog)
	{
		_textElement.Text = String.Empty;
		_pendingText = dialog;
		_currentTick = 0.0f;
		_currentCharIndex = 0;
		if (dialog.Length == 0)
		{
			return;
		}
		_textAnimating = true;
	}
}
