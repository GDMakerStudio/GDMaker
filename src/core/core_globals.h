#ifndef CORE_GLOBALS_H
#define CORE_GLOBALS_H


#include "core/object/class.h"
#include "core/object/method_bind.h"
#include "core/object/ref_counted.h"
#include "core/object/object.h"
#include "core/object/callable.h"
#include "core/input/input.h"
#include "core/input/input_map.h"
#include "core/plugins/plugin_map.h"
#include "core/string/string.h"
#include "core/varaint/array.h"

#include <pybind11/pybind11.h> 


void register_core_globals_types() {}
void uregister_core_globals_types() {}



#endif //
