#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot
{
class ViewModel : public RefCounted
{
    GDCLASS(ViewModel, RefCounted);

public:
    ViewModel();
    ~ViewModel();

protected:
    static void _bind_methods();

};
}