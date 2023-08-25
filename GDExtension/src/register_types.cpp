#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include "classes/pawn.h"
#include "classes/nightmare_character.h"
#include "classes/npc.h"
#include "classes/enemy.h"
#include "classes/camera_arm.h"

#include "classes/ui/dialog_text_display.h"
#include "classes/ui/nightmare_ui.h"

#include "classes/inventory/inventory.h"

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    // BASE CLASSES
    ClassDB::register_class<Pawn>();
    ClassDB::register_class<NightmareCharacter>();
    ClassDB::register_class<NPC>();
    ClassDB::register_class<Enemy>();
    ClassDB::register_class<CameraArm>();

    // UI CLASSES
    ClassDB::register_class<DialogTextDisplay>();
    ClassDB::register_class<NightmareUi>();

    // INVENTORY CLASSES
    ClassDB::register_class<Inventory>();
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_example_module);
    init_obj.register_terminator(uninitialize_example_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}