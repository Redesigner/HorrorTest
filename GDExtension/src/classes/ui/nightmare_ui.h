#ifndef NIGHTMARE_NIGHTMARE_UI_H
#define NIGHTMARE_NIGHTMARE_UI_H

#include <godot_cpp/classes/control.hpp>

#include "../../class_macros.h"

#include <godot_cpp/classes/input_event.hpp>

#include <stack>

namespace godot
{
class FadeUi;
class StackableUiElement;
class InventoryUiMenu;
class NightmareUi : public Control
{
    GDCLASS(NightmareUi, Control);

public:
    NightmareUi();
    ~NightmareUi();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    virtual void _input(const Ref<InputEvent> &event) override;


    void push_element(StackableUiElement *element);

    void pop_element();

    void set_dialog(String dialog);

    void advance_dialog();

    bool is_dialog_playing() const;

    FadeUi * get_fade_effect() const;

    InventoryUiMenu *get_inventory_menu() const;

private:
    void toggle_inventory();


    std::stack<StackableUiElement *> ui_stack;

    class DialogTextDisplay *_dialog_text_display;
    InventoryUiMenu *_inventory_menu;
    FadeUi *_fade_ui;
};
}

#endif