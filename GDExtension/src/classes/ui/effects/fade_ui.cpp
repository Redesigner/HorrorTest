#include "fade_ui.h"

#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

FadeUi::FadeUi()
{
    set_process_mode(PROCESS_MODE_ALWAYS);
}

FadeUi::~FadeUi()
{
}

void FadeUi::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("fade_out_complete"));
    ADD_SIGNAL(MethodInfo("fade_in_complete"));
}

void FadeUi::_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    if (!fading)
    {
        return;
    }
    current_fade_time += delta;
    Color current_color = get_color();
    const float alpha = Math::clamp(current_fade_time / max_fade_time, 0.f, 1.f);
    const float alpha_cubed = alpha * alpha * alpha;
    current_color.a = fading_in ? 1 - alpha_cubed : alpha_cubed;
    set_color(current_color);

    if (current_fade_time > max_fade_time)
    {
        current_fade_time = max_fade_time;
        complete_fade();
    }
}

void FadeUi::fade_in(float fade_time)
{
    UtilityFunctions::print("[FadeUi] fade in beginning.");
    fading_in = true;
    fade(fade_time);
}

void FadeUi::fade_out(float fade_time)
{
    UtilityFunctions::print("[FadeUi] fade out beginning.");
    set_visible(true);
    fading_in = false;
    fade(fade_time);
}

void FadeUi::fade(float fade_time)
{
    UtilityFunctions::print("[FadeUi] fade start.");
    max_fade_time = fade_time;
    fading = true;
}

void FadeUi::complete_fade()
{
    current_fade_time = 0.0f;
    fading = false;

    if (fading_in)
    {
        emit_signal("fade_in_complete");
        set_visible(false);
    }
    else
    {
        emit_signal("fade_out_complete");
    }
}
