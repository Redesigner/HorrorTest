#include "nightmare_ui.h"

#include "dialog_text_display.h"
#include "stackable_ui_element.h"
#include "inventory/inventory_ui_menu.h"
#include "effects/fade_ui.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/window.hpp>

using namespace godot;

NightmareUi::NightmareUi()
{
    _dialog_text_display = nullptr;
    _inventory_menu = nullptr;
}

NightmareUi::~NightmareUi()
{
}

void NightmareUi::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_dialog"), &NightmareUi::set_dialog);
    ClassDB::bind_method(D_METHOD("update_inventory"), &NightmareUi::update_inventory);
}

void NightmareUi::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    _dialog_text_display = get_node<DialogTextDisplay>("DialogTextDisplay");
    _inventory_menu = get_node<InventoryUiMenu>("InventoryMenu");
    _fade_ui = get_node<FadeUi>("Fade");
    // the ui will run when we're paused
    set_process_mode(PROCESS_MODE_ALWAYS);
}

void NightmareUi::_input(const Ref<InputEvent> &event)
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    // control the quitting from here, so it runs even when paused?
    if (event->is_action_pressed("ui_cancel"))
    {
        get_tree()->quit();
        return;
    }

    if (event->is_action_pressed("interact"))
    {
        if (!ui_stack.empty())
        {
            if (ui_stack.top()->accept())
            {
                pop_element();
            }
        }
        return;
    }

    if (event->is_action_pressed("toggle_inventory"))
    {
        toggle_inventory();
        return;
    }

    if (event->is_action_pressed("toggle_fullscreen"))
    {
        Window *window = get_tree()->get_root();
        if (window->get_mode() == Window::MODE_FULLSCREEN)
        {
            window->set_mode(Window::MODE_WINDOWED);
        }
        else
        {
            window->set_mode(Window::MODE_FULLSCREEN);
        }
        return;
    }
}

void NightmareUi::push_element(StackableUiElement *element)
{
    // Since there is an element on the screen, pause
    get_tree()->set_pause(true);
    element->show();
    ui_stack.push(element);
}

void NightmareUi::pop_element()
{
    ui_stack.top()->hide();
    ui_stack.pop();
    if (ui_stack.empty())
    {
        get_tree()->set_pause(false);
    }
}

void NightmareUi::set_dialog(String dialog)
{
    push_element(_dialog_text_display);
    _dialog_text_display->set_dialog(dialog);
}

void NightmareUi::advance_dialog()
{
    if (!_dialog_text_display->is_visible())
    {
        return;
    }
    if (_dialog_text_display->advance_dialog())
    {
        get_tree()->set_pause(false);
    }
}

bool NightmareUi::is_dialog_playing() const
{
    return _dialog_text_display->is_dialog_playing();
}

void NightmareUi::set_inventory(Inventory *inventory)
{
    _inventory_menu->set_inventory(inventory);
}

void NightmareUi::update_inventory()
{
    _inventory_menu->update();
}

FadeUi *NightmareUi::get_fade_effect() const
{
    return _fade_ui;
}

void NightmareUi::toggle_inventory()
{
    if (!ui_stack.empty())
    {
        // don't open the inventory if something else is being displayed
        if (ui_stack.top() == _inventory_menu)
        {
            pop_element();
        }
        return;
    }
    push_element(_inventory_menu);
}
