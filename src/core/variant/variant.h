#ifndef VARIANT_H
#define VARIANT_H


class Variant {
public:
    enum Type {
        NIL,
        INT,
        FLOAT,
        STRING,
    };

    Variant();
    Variant(int v);
    Variant(float v);
    Variant(const std::string &v);

    Type get_type() const;

    int to_int() const;
    float to_float() const;
    std::string to_string() const;

private:
    Type type;
    union {
        int int_value;
        float float_value;
    };
    std::string string_value;
};



#endif // 





