#ifndef NIGHTMARE_DIALOG_TEXT_DISPLAY_H
#define NIGHTMARE_DIALOG_TEXT_DISPLAY_H

#include <godot_cpp/classes/control.hpp>

#include "../../class_macros.h"

namespace godot
{
class DialogTextDisplay : public Control
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

private:
    class RichTextLabel *_textElement;

    String _pendingText;
    float _currentTick = 0.0f;
    int _currentCharIndex = 0;
    bool _textAnimating = false;

    DECLARE_PROPERTY(float, characterRate, 10.0f);
};
}

#endif