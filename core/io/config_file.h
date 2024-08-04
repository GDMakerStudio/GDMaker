#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <stdbool.h>

typedef struct {
    char *key;
    char *value;
} ConfigPair;

typedef struct {
    char *section;
    ConfigPair *pairs;
    int pair_count;
} ConfigSection;

typedef struct {
    ConfigSection *sections;
    int section_count;
} ConfigFile;

ConfigFile* config_file_new();
bool config_file_load(ConfigFile *config, const char *file_path);
bool config_file_save(ConfigFile *config, const char *file_path);
const char* config_file_get_value(ConfigFile *config, const char *section, const char *key);
void config_file_set_value(ConfigFile *config, const char *section, const char *key, const char *value);
ConfigSection* config_file_get_section(ConfigFile *config, const char *section);
void config_file_remove_key(ConfigFile *config, const char *section, const char *key);
void config_file_remove_section(ConfigFile *config, const char *section);

#endif // CONFIG_FILE_H
