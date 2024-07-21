

#include "class.h"


#include "core/object/method_bind.h"
#include "core/object/object.h"
#include "core/object/property_info.h"
#include "core/std/map.h"
#include "core/std/set.h"
#include "core/std/string.h"
#include "core/std/variant.h"
#include "core/std/unordered_set.h"



bool ClassInfo::has_property(const gdstd::string &property_name) const {
    return std::any_of(properties.begin(), properties.end(), [&property_name](const PropertyInfo &property) {
        return property.get_name() == property_name;
    });
}

const PropertyInfo* ClassInfo::get_property(const gdstd::string &property_name) const {
    auto it = std::find_if(properties.begin(), properties.end(), [&property_name](const PropertyInfo &property) {
        return property.get_name() == property_name;
    });
    if (it != properties.end()) {
        return &(*it);
    }
    return nullptr;
}

void ClassInfo::add_property(const PropertyInfo &property) {
    properties.push_back(property);
}

void ClassInfo::remove_property(const gdstd::string &property_name) {
    properties.erase(std::remove_if(properties.begin(), properties.end(), [&property_name](const PropertyInfo &property) {
        return property.get_name() == property_name;
    }), properties.end());
}


const gdstd::string& MethodDefinition::get_name() const {
    return name;
}

const gdstd::string& MethodDefinition::get_return_type() const {
    return return_type;
}

const gdstd::vector<MethodDefinition::Parameter>& MethodDefinition::get_parameters() const {
    return parameters;
}



Class& Class::get_signleton() {
    static ClassDB signleton;
    return signleton;
}

bool Class::class_exists(const gdstd::string &class_name) const {
    return classes.find(class_name) != classes.end();
}

bool Class::can_instantiate(const std::string &class_name) const {
    auto it = classes.find(class_name);
    return it != classes.end() && it->second.is_enabled;
}

gdstd::vector<gdstd::string> Class::get_class_list() const {
    gdstd::vector<gdstd::string> class_list;
    for (const auto &pair : classes) {
        class_list.push_back(pair.first);
    }
    return class_list;
}

const gdstd::string& Class::get_parent_class(const gdstd::string &class_name) const {
    auto it = classes.find(class_name);
    if (it != classes.end()) {
        return it->second.parent_name;
    }
    throw std::runtime_error("Class not found: " + class_name);
}

bool Class::is_parent_class(const gdstd::string &class_name, const gdstd::string &inherits) const {
    auto it = classes.find(class_name);
    while (it != classes.end()) {
        if (it->second.parent_name == inherits) {
            return true;
        }
        it = classes.find(it->second.parent_name);
    }
    return false;
}

void* Class::instantiate(const gdstd::string &class_name) const {
    auto it = classes.find(class_name);
    if (it != classes.end() && it->second.create_func) {
        return it->second.create_func();
    }
    throw std::runtime_error("Cannot instantiate class: " + class_name);
}

int Class:find_class(const gdstd::string& class_name) {
    auto it = classes.find(class_name);
    if (it != classes.end()) {
        return &it->second;
    }
    return nullptr;
}


bool Class::can_instantiate(const gdstd::string& class_name) const {
    auto it = classes.find(class_name);
    return it != classes.end() && it->second.create_func != nullptr;
}






