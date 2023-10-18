#ifndef NIGHTMARE_NIGHTMARE_UI_H
#define NIGHTMARE_NIGHTMARE_UI_H

#include <godot_cpp/classes/control.hpp>

#include "../../class_macros.h"

#include <godot_cpp/classes/input_event.hpp>

namespace godot
{
class FadeUi;
class NightmareUi : public Control
{
    GDCLASS(NightmareUi, Control);

protected:
    static void _bind_methods();

public:
    NightmareUi();
    ~NightmareUi();

    virtual void _ready() override;

    virtual void _input(const Ref<InputEvent> &event) override;

    void set_dialog(String dialog);

    void advance_dialog();

    bool is_dialog_playing() const;

    void set_inventory(class Inventory *inventory);

    void update_inventory();

    FadeUi * get_fade_effect() const;

private:
    void toggle_inventory();

    class DialogTextDisplay *_dialogTextDisplay;
    class InventoryUiMenu *_inventoryMenu;
    FadeUi* _fadeUi;
};
}

#endif