#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

// Please group the classes by directory, to make it easier to read
// The order here should be the same as the order below
#include "classes/pawns/pawn.h"
#include "classes/pawns/nightmare_character.h"
#include "classes/pawns/enemy.h"
#include "classes/camera_arm.h"

#include "classes/ui/dialog_text_display.h"
#include "classes/ui/nightmare_ui.h"

#include "classes/ui/inventory/inventory_ui_menu.h"
#include "classes/ui/inventory/inventory_ui_item_display.h"
#include "classes/ui/inventory/inventory_ui_item_preview_list.h"
#include "classes/ui/inventory/inventory_ui_item_preview.h"

#include "classes/inventory/inventory.h"
#include "classes/inventory/inventory_item_resource.h"

#include "classes/interactable/interactable.h"
#include "classes/interactable/npc.h"
#include "classes/interactable/pickup.h"
#include "classes/interactable/lock.h"

using namespace godot;

// still using the example module name...
void initialize_example_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    // BASE CLASSES
    ClassDB::register_class<Pawn>();
    ClassDB::register_class<NightmareCharacter>();
    ClassDB::register_class<Enemy>();
    ClassDB::register_class<CameraArm>();

    // UI CLASSES
    ClassDB::register_class<DialogTextDisplay>();
    ClassDB::register_class<NightmareUi>();
    // Inventory ui
    ClassDB::register_class<InventoryUiMenu>();
    ClassDB::register_class<InventoryUiItemDisplay>();
    ClassDB::register_class<InventoryUiItemPreviewList>();
    ClassDB::register_class<InventoryUiItemPreview>();

    // INVENTORY CLASSES
    ClassDB::register_class<Inventory>();
    ClassDB::register_class<InventoryItemResource>();

    // INTERACTABLE CLASSES
    ClassDB::register_class<Interactable>(); // this class should be registered as an abstract, but I can't figure out how to do that correctly at the moment
    ClassDB::register_class<NPC>();
    ClassDB::register_class<Pickup>();
    ClassDB::register_class<Lock>();
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