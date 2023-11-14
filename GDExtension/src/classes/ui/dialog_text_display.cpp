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
    text_element = Object::cast_to<RichTextLabel>(get_node_or_null("DialogPanel/RichTextLabel"));
    set_process_mode(PROCESS_MODE_WHEN_PAUSED);

    // Hide this element by default when we're not loaded, and clear any text that's in here
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    text_element->set_text(String());
    set_visible(false);
}

void DialogTextDisplay::_process(double delta)
{
    if (!text_animating)
    {
        return;
    }

    if (skip_text_debounce)
    {
        skip_text_debounce = false;
    }

    current_tick += delta * _characterRate;

    if (current_tick >= 1.0f)
    {
        String currentText = text_element->get_text();
        currentText += static_cast<String>(pending_tokens[current_token_index]);
        text_element->set_text(currentText);
        current_tick--;
        if (current_token_index >= pending_tokens.size() - 1)
        {
            text_animating = false;
            return;
        }
        current_token_index++;
    }
}

void DialogTextDisplay::set_dialog(String dialog)
{
    pending_tokens = process_string(dialog);
    text_element->set_text("");
    pending_text = dialog;
    current_tick = 0.0f;
    current_token_index = 0;
    skip_text_debounce = true;
    if (dialog.is_empty())
    {
        set_visible(false);
        return;
    }
    set_visible(true);
    text_animating = true;
}

bool DialogTextDisplay::advance_dialog()
{
    if (skip_text_debounce)
    {
        return false;
    }
    if (text_animating)
    {
        text_animating = false;
        text_element->set_text(pending_text);
        UtilityFunctions::print("[DialogTextDisplay] player skipped dialog animation.");
        return false;
    }
    // set_visible(false);
    return true;
}

bool DialogTextDisplay::is_dialog_playing() const
{
    return text_animating;
}

bool DialogTextDisplay::accept()
{
    return advance_dialog();
}

void DialogTextDisplay::show()
{
    set_visible(true);
}

void DialogTextDisplay::hide()
{
    set_visible(false);
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
