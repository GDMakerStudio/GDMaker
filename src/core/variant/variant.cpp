#include "variant.h"


// Variant implementations
Variant::Variant() : type(NIL) {}
Variant::Variant(int v) : type(INT), int_value(v) {}
Variant::Variant(float v) : type(FLOAT), float_value(v) {}
Variant::Variant(const std::string &v) : type(STRING), string_value(v) {}

Variant::Type Variant::get_type() const { return type; }

int Variant::to_int() const {
    if (type == INT) return int_value;
    throw std::runtime_error("Variant is not an int");
}

float Variant::to_float() const {
    if (type == FLOAT) return float_value;
    throw std::runtime_error("Variant is not a float");
}

std::string Variant::to_string() const {
    if (type == STRING) return string_value;
    throw std::runtime_error("Variant is not a string");
}




