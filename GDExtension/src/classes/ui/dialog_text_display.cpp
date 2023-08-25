#include "dialog_text_display.h"

#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

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
    set_process_mode(PROCESS_MODE_WHEN_PAUSED);

    // Hide this element by default when we're not loaded, and clear any text that's in here
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    _textElement->set_text(String());
    set_visible(false);
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
            set_visible(false);
			return;
		}
        set_visible(true);
		_textAnimating = true;
}

bool DialogTextDisplay::advance_dialog()
{
    if (_textAnimating)
    {
        _textAnimating = false;
        _textElement->set_text(_pendingText);
        return false;
    }
    set_visible(false);
    return true;
}

bool DialogTextDisplay::is_dialog_playing() const
{
    return _textAnimating;
}