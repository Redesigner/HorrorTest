#pragma once

namespace godot
{
class StackableUiElement
{
public:
    virtual void scroll_left();
    virtual void scroll_right();

    virtual void scroll_up();
    virtual void scroll_down();

    /// @brief Advance or confirm the element
    /// @return true if the element should be removed from the stack
    virtual bool accept();

    virtual void show() = 0;
    virtual void hide() = 0;
};
}