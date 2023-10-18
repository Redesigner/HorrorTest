#pragma once

#include <godot_cpp/classes/color_rect.hpp>

namespace godot
{
class FadeUi : public ColorRect
{
    GDCLASS(FadeUi, ColorRect);

public:
    FadeUi();
    ~FadeUi();

protected:
    static void _bind_methods();

public:
    virtual void _process(double delta) override;

    void fade_in(float fade_time);

    /// @brief Fade the ui to black
    /// @param fade_time How long it takes the ui to fade out
    void fade_out(float fade_time);

private:
    void fade(float fade_time);

    void complete_fade();

    float current_fade_time = 0.0f;
    float max_fade_time = 0.0f;

    bool fading_in = false;
    bool fading = false;
};
}