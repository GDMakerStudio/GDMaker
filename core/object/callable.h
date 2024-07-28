#ifndef CALLABLE_H
#define CALLABLE_H
  
#include <cstdlib>
#include <cstdio>

#include "core/collections/hash_map.h"
#include "core/collections/array.h"


struct Object; // For forward declaration


/**
  The Callable struct is a C++ based implementation designed to manage and invoke callable methods dynamically
  */

#define MAX_METHODS 128

// Define a macro to bind a member function to the callable
#define callable_mp(I, N, M) Callable::get_signleton()->bind(&(I), N, (void (*)(void*))M)

// Define a structure to hold callable methods
typedef struct CallableInfo {
  const char* name;
  void (*method)(void*);
};
        

struct Callable {
  CallableInfo methods[MAX_METHODS];
  int count;
  static Callable signleton;
  
  // Whene you need to use the HashMap for store the methods to maps
  // Also you need to use the callable for connect function with some class
  // Example in python with using Button example 
  // This example is a show you how to use callable class 
  // call : Callable
  // class ExampleScene(Object):
  //       def __init__(self):
  //            pass
  //       def _on_ready():
  //           button.connect("on_pressed", self , call ,on_pressed)
  //       pass
  
  HashMap<Callable, MAX_METHODS> collections;
public:
  
  static void (*get_signleton)();
  void (*init)(struct Callable* callable);
  void (*bind)(struct Callable* callable, const char* name, void (*method)(void*));
  void (*unbind)(struct Callable* callable, const char* name);
  void (*call)(struct Callable* callable, const char* name, void* signleton);
  bool (*is_bound)(struct Callable* callable, const char* name);
  void (*list_methods)(struct Callable* callable);
  CallableInfo* (*get_method)(struct Callable* callable, const char* name);
protected:
          friend struct Object;

  Callable();
  ~Callable();
};


#endif // CALLABLE_H
