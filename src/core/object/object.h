

#ifndef OBJECT_H
#define OBJECT_H


#include <cstdio>


#include "core/std/any.h"
#include "core/std/set.h"
#include "core/std/variant.h"
#include "core/object/method_bind.h"



class Variant;
class Array;


class Object {
public:
    virtual ~Object() = default;

    virtual gdstd::string get_type_name() const = 0;

    virtual bool _set(const std::string &property, const Variant &value);
    virtual void _init();

    void connect(const std::string &signal, Callable callable, int flags = 0);
    void emit_signal(const std::string &signal, const std::vector<Variant> &args = {});
    void disconnect(const std::string &signal, Callable callable);


    std::vector<std::string> get_property_list() const;
    bool property_exists(const std::string &property) const;
    Variant get(const std::string &property) const;
    void call(const std::string &method_name, const std::vector<Variant> &args = {});

    void set(const std::string &property, const Variant &value);
    Variant get(const std::string &property);
    std::string get_class() const;

    std::string tr_n(const std::string &message, const std::string &plural_message, int n, const std::string &context = "");
    std::string tr(const std::string &message, const std::string &context = "");
    
    bool is_blocking_signals() const;
    bool is_class(const std::string &class_name) const;


protected:
   gdstd::unordered_map<std::string, std::vector<Callable>> signals;
   gdstd::unordered_map<gdstd::string, Variant> properties;

};

using ObjectFactory = std::function<Object *()>;
using Method = std::function<void(Object *, const std::vector<Variant>&)>;

class TypeRegistry {
public:
    static TypeRegistry &instance();

    void register_type(const std::string &name, ObjectFactory factory);
    Object *create(const std::string &name);

    void register_method(const std::string &class_name, const std::string &method_name, Method method);
    void call_method(Object *obj, const std::string &method_name, const std::vector<Variant> &args);

private:
    std::unordered_map<std::string, ObjectFactory> factories;
    std::unordered_map<std::string, std::unordered_map<std::string, Method>> methods;
};



#define GDCLASS(T, B) \
public: \
    using BaseClass = B; \
    static std::string get_class_name() { return #T; } \
    static Object *create_instance() { return new T(); } \
    static void register_class() { \
        TypeRegistry::instance().register_type(#T, T::create_instance); \
    } \
    virtual std::string get_type_name() const override { return #T; } \
    static void register_methods() { \
        T::_register_methods(); \
    } \
private: \
    static void _register_methods()





#endif // 
