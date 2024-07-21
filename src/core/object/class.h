#ifndef CLASS_H
#define CLASS_H

// INCLUDE C STANDER LIBARIES

#include <cstring>
#include <cstdbool>
#include <cstdint>
#include <cstddef>

#include "core/std/list.h"
#include "core/std/map.h"
#include "core/std/unordered_map.h"
#include "core/std/unordered_set.h"

#include "core/string/string.h"
#include "core/variant/array.h"

class Object;
class PropertyInfo;

// CLASSDOC
// THE CLASSDOC IS A USED FOR DEFINE A SINGALE CLASS
// THE CLASSDOC IS A USED FOR AUTOMATIC CREATE CLASS 
// EXAMPLES
// GDCLASSDOC(MyClass) {
// 	void* (my_function)();
// };


#define GDCLASSDOC(p_class) \
typedef struct p_class p_class; \
class p_class


struct ClassInfo {
    const gdstd::string name;
    const gdstd::string base_name;
    gdstd::vector<gdstd::string> properties;
    void (*create_func)();
    size_t propert_count;
    const char **property;
    bool is_enabled; // New field to indicate if the class is enabled
    
    gdstd::vector<PropertyInfo> properties;
    gdstd::vector<gdstd::string> methods;
    gdstd::vector<gdstd::string> signals;

    bool has_property(const gdstd::string &property_name) const;
    const PropertyInfo* get_property(const gdstd::string &property_name) const;
    void add_property(const PropertyInfo &property);
    void remove_property(const gdstd::string &property_name);

};


class MethodDefinition {
public:
    using Variant = gdstd::variant<gdstd::string, int, float, bool>;

    struct Parameter {
        gdstd::string name;
        gdstd::string type;
        Variant default_value;
    };

    MethodDefinition(const gdstd::string &name, const gdstd::string &return_type, const std::vector<Parameter> &parameters)
        : name(name), return_type(return_type), parameters(parameters) {}

    const gdstd::string& get_name() const;
    const gdstd::string& get_return_type() const;
    const gdstd::vector<Parameter>& get_parameters() const;

private:
    gdstd::string name;
    gdstd::string return_type;
    gdstd::vector<Parameter> parameters;
};



GDCLASSDOC(Class) {
public:
    enum APIType {
        CORE,
        MODULE,
        PLUGIN,
        CUSTOM
    };
public:
    static Class& (get_signleton)(void);
    const char (get_class)(void);
    gdstd::vector<gdstd::string> (get_class_list)(void);
    int (find_class)(const gdstd::string& class_name);
    bool (class_exists)(const gdstd::string& class_name);
    bool (can_instantiate)(const gdstd::string& class_name);
    const char (class_get_property_list)(const gdstd::string& class_name, bool no_inheritance, size_t *property_count);
    const char (class_get_property)(void *object, const char *property);
    bool register_class(const gdstd::string& class_name, const char *pase_class_name, void *(*create_func)());
    void* (instantiate)(const gdstd::string& class_name);
    bool (is_class_enabled)(const gdstd::string& class_name);
    
    template<typename Ret, typename... Args> 
    static void bind_method(const gdstd::string& name, Ret(Object::*method)(Args...));
    

public:
    template <typename T>
    void register_class(const gdstd::string &parent_class_name) {
        if (classes.find(T::get_class_static()) != classes.end()) {
            throw std::runtime_error("Class already registered: " + T::get_class_static());
        }
        ClassInfo info = { T::get_class_static(), parent_class_name, []() -> void* { return new T(); } };
        classes[T::get_class_static()] = info;
    }

    template <typename T>
    void unregister_class() {
        auto it = classes.find(T::get_class_static());
        if (it != classes.end()) {
            classes.erase(it);
        } else {
            throw std::runtime_error("Class not found: " + T::get_class_static());
        }
    }
private:
	 Class() = default;
     ~Class();
   	 gdstd::unordered_map<gdstd::string, ClassInfo> classes;
};

protected:
	friend class Object;
	friend class Resource;
	friend void register_core_types();
};


#define GDREGISTER_CLASS(p_class) Class::get_signleton().register_class<p_class>(#p_class)
#define GDUNREGISTER_CLASS(p_class) Class::get_signleton().unregister_class<p_class>()


#endif // CLASS_H
