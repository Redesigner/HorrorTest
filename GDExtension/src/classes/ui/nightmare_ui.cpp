#include "nightmare_ui.h"

#include "dialog_text_display.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/window.hpp>

using namespace godot;

void NightmareUi::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_dialog"), &NightmareUi::set_dialog);
}

NightmareUi::NightmareUi()
{
}

NightmareUi::~NightmareUi()
{
}

void NightmareUi::_ready()
{
    _dialogTextDisplay = dynamic_cast<DialogTextDisplay *>(get_node_or_null("DialogTextDisplay"));
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