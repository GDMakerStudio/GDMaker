#include "register_core_types.h"


#include "core/config/engine.h"
#include "core/config/project_settings.h"
#include "core/cache/cache_core.h"
#include "core/cache/cache_crypto.h"
#include "core/object/object.h"
#include "core/object/struct_db.h"


#include "core/variant/variant.h"

struct StructDB db = {0};




void register_core_types() {
    // this file is a management to save classes with using structdb
    // also you can use GDREGISTER_CLASS macro whene you need it 
    // also you can check if class / struct is saved using StructDB
    // use class_exists()
   
    GDREGISTER_CLASS(&db , ProjectSettings);
    GDREGISTER_CLASS(&db , CacheCore);
    GDREGISTER_CLASS(&db , CacheCrypto);
    GDREGISTER_CLASS(&db , Object);
    GDREGISTER_CLASS(&db , Variant);
    GDREGISTER_CLASS(&db , String);
    GDREGISTER_CLASS(&db , Array);
    GDREGISTER_CLASS(&db , File);
    GDREGISTER_CLASS(&db , Directory);

    bind_method(&db , "ProjectSettings", "project_settings_new", &project_settings_new);
    
}
