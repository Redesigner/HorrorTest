#pragma once

#include <godot_cpp/classes/control.hpp>
#include "stackable_ui_element.h"

#include "../../class_macros.h"

namespace godot
{
class DialogTextDisplay : public Control, public StackableUiElement
{
    GDCLASS(DialogTextDisplay, Control);

protected:
    static void _bind_methods();

public:
    DialogTextDisplay();
    ~DialogTextDisplay();

    virtual void _ready() override;

    virtual void _process(double delta) override;

    void set_dialog(String dialog);

    // return true if the dialog is finished animating
    bool advance_dialog();

    bool is_dialog_playing() const;


    // StackableUiElement
    virtual bool accept() override;

    virtual void show() override;
    virtual void hide() override;

private:
    static TypedArray<String> process_string(String string);

private:
    class RichTextLabel *text_element;

    String pending_text;
    TypedArray<String> pending_tokens;
    float current_tick = 0.0f;
    int current_token_index = 0;
    bool text_animating = false;
    bool skip_text_debounce = false;

    DECLARE_PROPERTY(float, characterRate, 10.0f);
};
}