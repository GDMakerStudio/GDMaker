#include "config_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ConfigFile* config_file_new() {
    ConfigFile *config = (ConfigFile*)malloc(sizeof(ConfigFile));
    config->sections = NULL;
    config->section_count = 0;
    return config;
}

bool config_file_load(ConfigFile *config, const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) return false;

    char line[256];
    ConfigSection *current_section = NULL;

    while (fgets(line, sizeof(line), file)) {
        char *trimmed_line = strtok(line, "\n");
        if (trimmed_line[0] == '[') {
            current_section = (ConfigSection*)malloc(sizeof(ConfigSection));
            current_section->section = strdup(trimmed_line + 1);
            current_section->section[strlen(current_section->section) - 1] = '\0';
            current_section->pairs = NULL;
            current_section->pair_count = 0;
            config->sections = (ConfigSection*)realloc(config->sections, sizeof(ConfigSection) * (config->section_count + 1));
            config->sections[config->section_count++] = *current_section;
        } else if (current_section && strchr(trimmed_line, '=')) {
            ConfigPair pair;
            pair.key = strdup(strtok(trimmed_line, "="));
            pair.value = strdup(strtok(NULL, "="));
            current_section->pairs = (ConfigPair*)realloc(current_section->pairs, sizeof(ConfigPair) * (current_section->pair_count + 1));
            current_section->pairs[current_section->pair_count++] = pair;
        }
    }

    fclose(file);
    return true;
}

bool config_file_save(ConfigFile *config, const char *file_path) {
    FILE *file = fopen(file_path, "w");
    if (!file) return false;

    for (int i = 0; i < config->section_count; i++) {
        fprintf(file, "[%s]\n", config->sections[i].section);
        for (int j = 0; j < config->sections[i].pair_count; j++) {
            fprintf(file, "%s=%s\n", config->sections[i].pairs[j].key, config->sections[i].pairs[j].value);
        }
    }

    fclose(file);
    return true;
}

const char* config_file_get_value(ConfigFile *config, const char *section, const char *key) {
    for (int i = 0; i < config->section_count; i++) {
        if (strcmp(config->sections[i].section, section) == 0) {
            for (int j = 0; j < config->sections[i].pair_count; j++) {
                if (strcmp(config->sections[i].pairs[j].key, key) == 0) {
                    return config->sections[i].pairs[j].value;
                }
            }
        }
    }
    return NULL;
}

void config_file_set_value(ConfigFile *config, const char *section, const char *key, const char *value) {
    for (int i = 0; i < config->section_count; i++) {
        if (strcmp(config->sections[i].section, section) == 0) {
            for (int j = 0; j < config->sections[i].pair_count; j++) {
                if (strcmp(config->sections[i].pairs[j].key, key) == 0) {
                    free(config->sections[i].pairs[j].value);
                    config->sections[i].pairs[j].value = strdup(value);
                    return;
                }
            }
            ConfigPair pair;
            pair.key = strdup(key);
            pair.value = strdup(value);
            config->sections[i].pairs = (ConfigPair*)realloc(config->sections[i].pairs, sizeof(ConfigPair) * (config->sections[i].pair_count + 1));
            config->sections[i].pairs[config->sections[i].pair_count++] = pair;
            return;
        }
    }
    ConfigSection new_section;
    new_section.section = strdup(section);
    new_section.pairs = (ConfigPair*)malloc(sizeof(ConfigPair));
    new_section.pair_count = 1;
    new_section.pairs[0].key = strdup(key);
    new_section.pairs[0].value = strdup(value);
    config->sections = (ConfigSection*)realloc(config->sections, sizeof(ConfigSection) * (config->section_count + 1));
    config->sections[config->section_count++] = new_section;
}

ConfigSection* config_file_get_section(ConfigFile *config, const char *section) {
    for (int i = 0; i < config->section_count; i++) {
        if (strcmp(config->sections[i].section, section) == 0) {
            return &config->sections[i];
        }
    }
    return NULL;
}

void config_file_remove_key(ConfigFile *config, const char *section, const char *key) {
    for (int i = 0; i < config->section_count; i++) {
        if (strcmp(config->sections[i].section, section) == 0) {
            for (int j = 0; j < config->sections[i].pair_count; j++) {
                if (strcmp(config->sections[i].pairs[j].key, key) == 0) {
                    free(config->sections[i].pairs[j].key);
                    free(config->sections[i].pairs[j].value);
                    for (int k = j; k < config->sections[i].pair_count - 1; k++) {
                        config->sections[i].pairs[k] = config->sections[i].pairs[k + 1];
                    }
                    config->sections[i].pair_count--;
                    config->sections[i].pairs = (ConfigPair*)realloc(config->sections[i].pairs, sizeof(ConfigPair) * config->sections[i].pair_count);
                    return;
                }
            }
        }
    }
}

void config_file_remove_section(ConfigFile *config, const char *section) {
    for (int i = 0; i < config->section_count; i++) {
        if (strcmp(config->sections[i].section, section) == 0) {
            free(config->sections[i].section);
            for (int j = 0; j < config->sections[i].pair_count; j++) {
                free(config->sections[i].pairs[j].key);
                free(config->sections[i].pairs[j].value);
            }
            free(config->sections[i].pairs);
            for (int k = i; k < config->section_count - 1; k++) {
                config->sections[k] = config->sections[k + 1];
            }
            config->section_count--;
            config->sections = (ConfigSection*)realloc(config->sections, sizeof(ConfigSection) * config->section_count);
            return;
        }
    }
}
