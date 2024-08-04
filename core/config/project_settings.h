#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include "core/object/object.h"
#include "core/string/string.h"
#include "core/variant/variant.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SETTINGS 128
#define MAX_KEY_LENGTH 64
#define MAX_VALUE_LENGTH 256

typedef struct {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
} Setting;

typedef struct ProjectSettings {
    Setting settings[MAX_SETTINGS];
    String file;
    int count;
} ProjectSettings;

 
ProjectSettings* project_settings_new();
int load_settings(ProjectSettings* ps, const char* filepath);
const char* get_setting(ProjectSettings* ps, const char* key);
int set_settings(ProjectSettings* ps, const char* key, const char* value);
int save_settings(ProjectSettings* ps, const char* filepath);


#endif 