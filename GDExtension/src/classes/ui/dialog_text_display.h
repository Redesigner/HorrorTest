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
    class RichTextLabel *_textElement;

    String _pendingText;
    TypedArray<String> _pendingTokens;
    float _currentTick = 0.0f;
    int _currentTokenIndex = 0;
    bool _textAnimating = false;

    DECLARE_PROPERTY(float, characterRate, 10.0f);
};
}