#ifndef NIGHTMARE_NIGHTMARE_UI_H
#define NIGHTMARE_NIGHTMARE_UI_H

#include <godot_cpp/classes/control.hpp>

#include "../../class_macros.h"

namespace godot
{
class NightmareUi : public Control
{
    GDCLASS(NightmareUi, Control);

protected:
    static void _bind_methods();

public:
    NightmareUi();
    ~NightmareUi();

    virtual void _ready() override;

    void set_dialog(String dialog);

private:
    class DialogTextDisplay *_dialogTextDisplay;
};
}

#endif