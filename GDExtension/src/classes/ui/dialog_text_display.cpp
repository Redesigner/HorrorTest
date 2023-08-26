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
        currentText += _pendingTokens[_currentTokenIndex];
        _textElement->set_text(currentText);
        _currentTick--;
        if (_currentTokenIndex >= _pendingTokens.size() - 1)
        {
            _textAnimating = false;
            return;
        }
        _currentTokenIndex++;
    }
}

void DialogTextDisplay::set_dialog(String dialog)
{
    _pendingTokens = process_string(dialog);
    _textElement->set_text("");
    _pendingText = dialog;
    _currentTick = 0.0f;
    _currentTokenIndex = 0;
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

TypedArray<String> DialogTextDisplay::process_string(String string)
{
    // I'm not optimistic about the performance here, but it's making do with godot's built in types
    TypedArray<String> tokens = TypedArray<String>::make();
    String currentToken = "";
    bool isSpecialToken = false;
    for (int i = 0; i < string.length(); i++)
    {
        char32_t currentCharacter= string[i];

        if (currentCharacter == '[')
        {
            // start a new special token
            isSpecialToken = true;
            currentToken += "[";
            continue;
        }

        if (isSpecialToken && currentCharacter == ']')
        {
            currentToken += ']';
            isSpecialToken = false;
            continue;
        }

        if (isSpecialToken)
        {
            currentToken += currentCharacter;
            continue;
        }

        currentToken += currentCharacter;
        tokens.append(currentToken);
        currentToken = "";
    }
    if (currentToken.length() > 1)
    {
        tokens.append(currentToken);
    }

    return tokens;
}
