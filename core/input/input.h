#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "core/error/error_macros.h"
#include "bake/bake_config.h"

typedef enum {
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
} KeyCode;

typedef enum {
    MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
} MouseButton;

typedef struct {
    GLFWwindow* window; // NOTE: USE THIS FOR INPUT WINDOW CONTROLLER
    bool keys[GLFW_KEY_LAST + 1];
    bool keys_last[GLFW_KEY_LAST + 1];
    bool mouse_buttons[GLFW_MOUSE_BUTTON_LAST + 1];
    bool mouse_buttons_last[GLFW_MOUSE_BUTTON_LAST + 1];
} Input;

void input_update(Input *input);
bool input_is_key_pressed(Input *input, KeyCode key);
bool input_is_key_just_pressed(Input *input, KeyCode key);
bool input_is_key_just_released(Input *input, KeyCode key);
bool input_is_mouse_button_pressed(Input *input, MouseButton button);
bool input_is_mouse_button_just_pressed(Input *input, MouseButton button);
bool input_is_mouse_button_just_released(Input *input, MouseButton button);
void input_get_mouse_position(Input *input, double *x, double *y);

#endif // INPUT_H
