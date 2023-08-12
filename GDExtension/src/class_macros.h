#ifndef NIGHTMARE_CLASS_MACROS_H
#define NIGHTMARE_CLASS_MACROS_H

#define BIND_PROPERTY(p_name, class, variant)\
    ClassDB::bind_method(D_METHOD("get_"#p_name), &class::get_p_name);\
    ClassDB::bind_method(D_METHOD("set_"#p_name, "p_"#p_name), &class::set_ ##p_name);\
    ClassDB::add_property(#class, PropertyInfo((variant), #p_name), "set_"#p_name, "get_"#p_name);\

#define DECLARE_PROPERTY(p_name, type, default_value)\
private:\
    ##type _##p_name = ##default_value;\
public:\
    void set_##p_name(##type value)\
    {\
        _##p_name = value;\
    }\
    ##type get_##p_name() const\
    {\
        return _##p_name;\
    }

#endif