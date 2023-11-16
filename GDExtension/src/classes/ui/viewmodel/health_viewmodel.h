#pragma once

#include "viewmodel.h"

namespace godot
{
class HealthUi;
class NightmareCharacter;

class HealthViewModel : public ViewModel
{
    GDCLASS(HealthViewModel, ViewModel);

public:
    HealthViewModel();
    ~HealthViewModel();

protected:
    static void _bind_methods();

public:
    void bind_model(NightmareCharacter *character);
    void bind_view(HealthUi *health_ui);

    void on_health_changed(float health);

private:
    NightmareCharacter *model;
    HealthUi *view;
};
}