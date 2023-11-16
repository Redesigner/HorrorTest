#include "health_viewmodel.h"

#include "../health_ui.h"
#include "../../pawns/nightmare_character.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

HealthViewModel::HealthViewModel()
{
    model = nullptr;
    view = nullptr;
}

HealthViewModel::~HealthViewModel()
{
}

void HealthViewModel::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("on_health_changed"), &HealthViewModel::on_health_changed);
}

void HealthViewModel::bind_model(NightmareCharacter *character)
{
    model = character;
    model->connect("health_changed", Callable(this, "on_health_changed"));
    if (view)
    {
        view->set_health(model->get_current_health());
    }
}

void HealthViewModel::bind_view(HealthUi *health_ui)
{
    view = health_ui;
    if (model)
    {
        view->set_health(model->get_current_health());
    }
}

void HealthViewModel::on_health_changed(float health)
{
    if (!view)
    {
        WARN_PRINT("[HealthViewModel] health changed, but view was nullptr");
        return;
    }
    view->set_health(health);
}
