#include "property_info.h"


const std::string& PropertyInfo::get_name() const {
    return name;
}

const std::string& PropertyInfo::get_type() const {
    return type;
}

PropertyInfo::Variant PropertyInfo::get_default_value() const {
    return default_value;
}

void PropertyInfo::set_default_value(const Variant &value) {
    default_value = value;
}
