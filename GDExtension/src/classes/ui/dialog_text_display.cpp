#include "dialog_text_display.h"

#include <godot_cpp/classes/rich_text_label.hpp>

using namespace godot;

void DialogTextDisplay::_bind_methods()
{
    BIND_PROPERTY(Variant::FLOAT, characterRate, DialogTextDisplay);
}

DialogTextDisplay::DialogTextDisplay()
{
}

DialogTextDisplay::~DialogTextDisplay()
{
}

void DialogTextDisplay::_ready()
{
    _textElement = Object::cast_to<RichTextLabel>(get_node_or_null("DialogPanel/RichTextLabel"));
}

void DialogTextDisplay::_process(double delta)
{
    if (!_textAnimating)
    {
        return;
    }

    _currentTick += delta * _characterRate;

    if (_currentTick >= 1.0f)
    {
        String currentText = _textElement->get_text();
        currentText += _pendingText[_currentCharIndex];
        _textElement->set_text(currentText);
        _currentTick--;
        if (_currentCharIndex >= _pendingText.length() - 1)
        {
            _textAnimating = false;
            return;
        }
        _currentCharIndex++;
    }
}

void DialogTextDisplay::set_dialog(String dialog)
{
        _textElement->set_text("");
		_pendingText = dialog;
		_currentTick = 0.0f;
		_currentCharIndex = 0;
		if (dialog.is_empty())
		{
			return;
		}
		_textAnimating = true;
}