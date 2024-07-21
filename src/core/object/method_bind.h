#ifndef METHODBIND_H
#define METHODBIND_H

#include "core/std/string.h"
#include "core/std/vector.h"
#include "core/std/property_info.h"


class Object; // Forward declaration

class MethodBind {
public:
    MethodBind(const gdstd::string& name, const gdstd::vector<PropertyInfo>& arguments, PropertyType return_type);
    virtual ~MethodBind();

    const gdstd::string& get_name() const;
    const gdstd::vector<PropertyInfo>& get_argument_info() const;
    PropertyType get_return_type() const;

    virtual void call(Object* obj, const gdstd::vector<void*>& args) = 0;

protected:
    gdstd::string method_name;
    gdstd::vector<PropertyInfo> argument_info;
    PropertyType return_type;
};

template<typename Ret, typename... Args>
class MethodBindImpl : public MethodBind {
public:
    using MethodType = std::function<Ret(Object*, Args...)>;

    MethodBindImpl(const std::string& name, MethodType method, const std::vector<PropertyInfo>& arguments);

    virtual void call(Object* obj, const std::vector<void*>& args) override;

private:
    MethodType method;
};

#endif // METHODBIND_H
