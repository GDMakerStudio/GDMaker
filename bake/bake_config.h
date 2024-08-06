#ifndef BAKE_CONFIG_H
#define BAKE_CONFIG_H

// Define the version of the bake configuration
#define BAKE_CONFIG_VERSION_MAJOR 1
#define BAKE_CONFIG_VERSION_MINOR 0
#define BAKE_CONFIG_VERSION_PATCH 0

// Combine version numbers into a single macro
#define BAKE_CONFIG_VERSION (BAKE_CONFIG_VERSION_MAJOR * 10000 + BAKE_CONFIG_VERSION_MINOR * 100 + BAKE_CONFIG_VERSION_PATCH)

// Macros for baking configuration settings
#ifndef BAKE_CONFIG_MAX_TEMPERATURE
#define BAKE_CONFIG_MAX_TEMPERATURE 250  // Maximum baking temperature in degrees Celsius
#endif

#ifndef BAKE_CONFIG_MIN_TEMPERATURE
#define BAKE_CONFIG_MIN_TEMPERATURE 50   // Minimum baking temperature in degrees Celsius
#endif

#ifndef BAKE_CONFIG_DEFAULT_TEMPERATURE
#define BAKE_CONFIG_DEFAULT_TEMPERATURE 180 // Default baking temperature in degrees Celsius
#endif

#ifndef BAKE_CONFIG_MAX_TIME
#define BAKE_CONFIG_MAX_TIME 120         // Maximum baking time in minutes
#endif

#ifndef BAKE_CONFIG_MIN_TIME
#define BAKE_CONFIG_MIN_TIME 5           // Minimum baking time in minutes
#endif

#ifndef BAKE_CONFIG_DEFAULT_TIME
#define BAKE_CONFIG_DEFAULT_TIME 30      // Default baking time in minutes
#endif

#ifndef BAKE_CONFIG_PREHEAT_REQUIRED
#define BAKE_CONFIG_PREHEAT_REQUIRED 1   // 1 if preheating is required, 0 otherwise
#endif

#ifndef BAKE_CONFIG_DEFAULT_PREHEAT_TEMPERATURE
#define BAKE_CONFIG_DEFAULT_PREHEAT_TEMPERATURE 200 // Default preheat temperature in degrees Celsius
#endif

#ifndef BAKE_CONFIG_MAX_RECIPES
#define BAKE_CONFIG_MAX_RECIPES 100      // Maximum number of recipes
#endif

#ifndef BAKE_CONFIG_DEFAULT_RECIPE
#define BAKE_CONFIG_DEFAULT_RECIPE "DefaultRecipe" // Default recipe name
#endif

#ifndef BAKE_CONFIG_MAX_INGREDIENTS
#define BAKE_CONFIG_MAX_INGREDIENTS 50   // Maximum number of ingredients per recipe
#endif

#ifndef BAKE_CONFIG_DEFAULT_INGREDIENT
#define BAKE_CONFIG_DEFAULT_INGREDIENT "Flour" // Default ingredient
#endif

#ifndef BAKE_CONFIG_MAX_OVEN_CAPACITY
#define BAKE_CONFIG_MAX_OVEN_CAPACITY 4  // Maximum oven capacity in terms of number of baking trays
#endif

#ifndef BAKE_CONFIG_MIN_OVEN_CAPACITY
#define BAKE_CONFIG_MIN_OVEN_CAPACITY 1  // Minimum oven capacity in terms of number of baking trays
#endif

#ifndef BAKE_CONFIG_DEFAULT_OVEN_CAPACITY
#define BAKE_CONFIG_DEFAULT_OVEN_CAPACITY 2 // Default oven capacity in terms of number of baking trays
#endif

#ifndef BAKE_CONFIG_OVEN_BRAND
#define BAKE_CONFIG_OVEN_BRAND "GenericOven" // Default oven brand
#endif

#ifndef BAKE_CONFIG_TIMER_ALERT_SOUND
#define BAKE_CONFIG_TIMER_ALERT_SOUND "alert.wav" // Default timer alert sound file
#endif

#ifndef BAKE_CONFIG_LOG_FILE
#define BAKE_CONFIG_LOG_FILE "bake_log.txt" // Default log file for recording baking sessions
#endif

#ifndef BAKE_CONFIG_ENABLE_LOGGING
#define BAKE_CONFIG_ENABLE_LOGGING 1     // 1 if logging is enabled, 0 otherwise
#endif

#define RAYLIB_IMPLEMENTATION
#include <raylib.h>


#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#define RPNG_IMPLEMENTATION
#include <rpng.h>


#define CURL_IMPLEMENTATION
#include <curl/curl.h>

#endif // BAKE_CONFIG_H
