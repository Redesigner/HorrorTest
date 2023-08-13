#include "nightmare_ui.h"

#include "dialog_text_display.h"

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
}

void NightmareUi::set_dialog(String dialog)
{
    _dialogTextDisplay->set_dialog(dialog);
}