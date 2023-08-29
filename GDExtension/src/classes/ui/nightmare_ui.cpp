#include "nightmare_ui.h"

#include "dialog_text_display.h"

#include "inventory/inventory_ui_menu.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/window.hpp>

using namespace godot;

void NightmareUi::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_dialog"), &NightmareUi::set_dialog);
    ClassDB::bind_method(D_METHOD("update_inventory"), &NightmareUi::update_inventory);
}

NightmareUi::NightmareUi()
{
    _dialogTextDisplay = nullptr;
    _inventoryMenu = nullptr;
}

NightmareUi::~NightmareUi()
{
}

void NightmareUi::_ready()
{
    _dialogTextDisplay = dynamic_cast<DialogTextDisplay *>(get_node_or_null("DialogTextDisplay"));
    _inventoryMenu = dynamic_cast<InventoryUiMenu *>(get_node_or_null("InventoryMenu"));
    // the ui will run when we're paused
    set_process_mode(PROCESS_MODE_ALWAYS);
}

void NightmareUi::_input(const Ref<InputEvent> &event)
{
    // control the quitting from here, so it runs even when paused?
    if (event->is_action_pressed("ui_cancel"))
    {
        get_tree()->quit();
        return;
    }

    if (event->is_action_pressed("interact"))
    {
        advance_dialog();
        return;
    }

    if (event->is_action_pressed("toggle_inventory"))
    {
        if (!_inventoryMenu->is_visible())
        {
            get_tree()->set_pause(true);
            _inventoryMenu->set_visible(true);
        }
        else
        {
            _inventoryMenu->set_visible(false);

            // unpause the game only if there is no dialog playing
            if (!is_dialog_playing())
            {
                get_tree()->set_pause(false);
            }
        }
    }

    if (event->is_action_pressed("toggle_fullscreen"))
    {
        Window* window = get_tree()->get_root();
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

void NightmareUi::set_dialog(String dialog)
{
    _dialogTextDisplay->set_dialog(dialog);
    get_tree()->set_pause(true);
}

void NightmareUi::advance_dialog()
{
    if (_dialogTextDisplay->advance_dialog())
    {
        get_tree()->set_pause(false);
    }
}

bool NightmareUi::is_dialog_playing() const
{
    return _dialogTextDisplay->is_dialog_playing();
}

void NightmareUi::set_inventory(Inventory *inventory)
{
    _inventoryMenu->set_inventory(inventory);
}

void NightmareUi::update_inventory()
{
    _inventoryMenu->update();
}
