#ifndef PROPERTY_INFO_H
#define PROPERTY_INFO_H

#include "core/std/string.h"
#include "core/std/vector.h"
#include "core/std/variant.h"


class String;

enum PropertyType {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_OBJECT,
    TYPE_VOID, // Added for completeness
};


/**
   	 PropertyInfo is a class that encapsulates information about a property in the ClassDB system.
	It holds details like the property's name, type, and default value. 
	This class allows for a more organized and structured way to handle properties of registered classes.
**/
class PropertyInfo {
public:
    using Variant = gdstd::variant<gdstd::string, int, float, bool>;

    PropertyInfo(const gdstd::string &name, const gdstd::string &type, const Variant &default_value)
        : name(name), type(type), default_value(default_value) {}

    const gdstd::string& get_name() const;
    const gdstd::string& get_type() const;
    Variant get_default_value() const;
    void set_default_value(const Variant &value);

private:
    gdstd::string name;
    gdstd::string type;
    Variant default_value;
};

#endif // PROPERTY_INFO_H
