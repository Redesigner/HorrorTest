#pragma once

#define BIND_PROPERTY_HINT(variant, p_name, class, hint_type, hint)\
    ClassDB::bind_method(D_METHOD("get_"#p_name), &class::get_ ##p_name);\
    ClassDB::bind_method(D_METHOD("set_"#p_name, "p_"#p_name), &class::set_ ##p_name);\
    ClassDB::add_property( #class, PropertyInfo((variant), #p_name, hint_type, hint), "set_"#p_name, "get_"#p_name);\

#define BIND_PROPERTY(variant, p_name, class)\
    ClassDB::bind_method(D_METHOD("get_"#p_name), &class::get_ ##p_name);\
    ClassDB::bind_method(D_METHOD("set_"#p_name, "p_"#p_name), &class::set_ ##p_name);\
    ClassDB::add_property( #class, PropertyInfo((variant), #p_name), "set_"#p_name, "get_"#p_name);\

#define DECLARE_PROPERTY(type, p_name, default_value)\
private:\
    type _##p_name = default_value;\
public:\
    void set_##p_name(type value)\
    {\
        _##p_name = value;\
    }\
    type get_##p_name() const\
    {\
        return _##p_name;\
    }\
private:

#define DECLARE_PROPERTY_NODEFAULT(type, p_name)\
private:\
    type _##p_name;\
public:\
    void set_##p_name(type value)\
    {\
        _##p_name = value;\
    }\
    type get_##p_name() const\
    {\
        return _##p_name;\
    }\
private:

#define IN_EDITOR() Engine::get_singleton()->is_editor_hint()