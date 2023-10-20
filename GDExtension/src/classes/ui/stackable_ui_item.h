#pragma once

namespace godot
{
class StackableUiItem
{
public:
    virtual void scroll_left() = 0;
    virtual void scroll_right() = 0;

    virtual void scroll_up() = 0;
    virtual void scroll_down() = 0;

    virtual void accept() = 0;
};
}