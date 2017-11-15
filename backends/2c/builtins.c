#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void panic(char *ch) {
    puts("Panic!");
    if (ch) {
        puts(ch);
    }
    exit(1);
}

void *ic_alloc(size_t size) {
    void *v = 0;

    /* malloc/calloc allow a size of 0 but state
     * "if size is 0, then malloc() returns either NULL, or a unique pointer
     * value that can later be successfully passed to free()."
     *
     * we want to guarantee the second behavior
     * so we default to a minimum size of 1
     */
    if (size == 0) {
        size = 1;
    }

    v = calloc(1, size);
    if (!v) {
        puts("ic_alloc: call to calloc failed");
        exit(1);
    }

    return v;
}

/* pre-declarations
 * types should be opaque
 */
typedef char Unit;
typedef uint8_t Bool;
typedef int32_t Sint;
typedef uint32_t Uint;
typedef struct String *String;

Unit ic_unit_new();
Bool ic_bool_new(uint8_t boolean);
Sint ic_sint_new(int32_t integer);
Uint ic_uint_new(uint32_t integer);
String ic_string_new(char *str, unsigned int len);

Unit ic_unit_new(void) {
    return 0;
}

Unit Unit_a_b(void) {
  return ic_unit_new();
}

/* builtin fn println() */
Unit println_a_b(void) {
    puts("");
    return 0;
}

/* builtin fn print(a::Unit) */
Unit print_a_Unit_b(Unit a) {
    fputs("Unit()", stdout);
    return 0;
}

/* builtin fn println(a::Unit) */
Unit println_a_Unit_b(Unit a) {
    print_a_Unit_b(a);
    println_a_b();
    return 0;
}

/* builtin fn equal(a::Unit, b::Unit) -> Bool */
Bool equal_a_Unit_Unit_b(Unit a, Unit b) {
    return ic_bool_new(1);
}

/* builtin fn to_str(a::Unit) -> String */
String to_str_a_Unit_b(Unit a) {
    return ic_string_new("()", 4);
}

/* builtin fn print(a::Bool) */
Unit print_a_Bool_b(Bool b) {
    if (b) {
        fputs("True", stdout);
    } else {
        fputs("False", stdout);
    }
    return 0;
}
/* builtin fn println(a::Bool) */
Unit println_a_Bool_b(Bool b) {
    print_a_Bool_b(b);
    println_a_b();
    return 0;
}
Bool ic_bool_new(uint8_t boolean) {
    Bool b = boolean;
    return b;
}
/* builtin fn equal(a::Bool, b::Bool) -> Bool */
Bool equal_a_Bool_Bool_b(Bool a, Bool b) {
    if (a == 0 && b == 0) {
        /* return truthy boolean */
        return ic_bool_new(1);
    }

    if (a && b) {
        /* return truthy boolean */
        return ic_bool_new(1);
    }

    /* return falsey boolean */
    return ic_bool_new(0);
}
/* builtin fn not(a::Bool) -> Bool */
Bool not_a_Bool_b(Bool a) {
    if (a) {
        /* return falsey boolean */
        return ic_bool_new(0);
    } else {
        /* return truthy boolean */
        return ic_bool_new(1);
    }
}
/* builtin fn and(a::Bool, b::Bool) -> Bool */
Bool and_a_Bool_Bool_b(Bool a, Bool b) {
    if (a && b) {
        /* return truthy boolean */
        return ic_bool_new(1);
    }
    /* return falsey boolean */
    return ic_bool_new(0);
}
/* builtin fn or(a::Bool, b::Bool) -> Bool */
Bool or_a_Bool_Bool_b(Bool a, Bool b) {
    if (a) {
        /* return truthy boolean */
        return ic_bool_new(1);
    }

    if (b) {
        /* return truthy boolean */
        return ic_bool_new(1);
    }

    /* return falsey boolean */
    return ic_bool_new(0);
}
/* builtin fn to_str(a::Bool) -> String */
String to_str_a_Bool_b(Bool a) {
    /* FIXME TODO should re-use constants */
    if (a) {
        return ic_string_new("True", 5);
    } else {
        return ic_string_new("False", 6);
    }
}
/* builtin fn assert(a::Bool) -> Unit */
Unit assert_a_Bool_b(Bool a) {
    if (!a) {
        panic("assertion failed");
    }
    return 0;
}

Sint ic_sint_new(int32_t integer) {
    Sint i = integer;
    return i;
}
/* builtin fn print(a::Sint) */
Unit print_a_Sint_b(Sint i) {
    printf("%" PRId32, i);
    return 0;
}
/* builtin fn println(a::Sint) */
Unit println_a_Sint_b(Sint i) {
    print_a_Sint_b(i);
    println_a_b();
    return 0;
}
/* builtin fn equal(a::Sint, b::Sint) -> Bool */
Bool equal_a_Sint_Sint_b(Sint a, Sint b) {
    if (a == b) {
        /* return truthy boolean */
        return ic_bool_new(1);
    }
    /* return falsey boolean */
    return ic_bool_new(0);
}
/* builtin fn plus(a::Sint, b::Sint) -> Sint */
Sint plus_a_Sint_Sint_b(Sint a, Sint b) {
    Sint i = 0;
    i = ic_sint_new(a + b);
    return i;
}
/* builtin fn minus(a::Sint, b::Sint) -> Sint */
Sint minus_a_Sint_Sint_b(Sint a, Sint b) {
    Sint i = 0;
    i = ic_sint_new(a - b);
    return i;
}
/* builtin fn multiply(a::Sint, b::Sint) -> Sint */
Sint multiply_a_Sint_Sint_b(Sint a, Sint b) {
    Sint i = 0;
    i = ic_sint_new(a * b);
    return i;
}
/* builtin fn divide(a::Sint, b::Sint) -> Sint */
Sint divide_a_Sint_Sint_b(Sint a, Sint b) {
    Sint i = 0;
    i = ic_sint_new(a / b);
    return i;
}
/* builtin fn modulo(a::Sint, b::Sint) -> Sint */
Sint modulo_a_Sint_Sint_b(Sint a, Sint b) {
    Sint i = 0;
    i = ic_sint_new(a % b);
    return i;
}
/* builtin fn lessthan(a::Sint, b::Sint) -> Bool */
Bool lessthan_a_Sint_Sint_b(Sint a, Sint b) {
    Bool r = 0;
    r = ic_bool_new(a < b);
    return r;
}
/* builtin fn greaterthan(a::Sint, b::Sint) -> Bool */
Bool greaterthan_a_Sint_Sint_b(Sint a, Sint b) {
    Bool r = 0;
    r = ic_bool_new(a > b);
    return r;
}
/* builtin fn lessthan_equal(a::Sint, b::Sint) -> Bool */
Bool lessthan_equal_a_Sint_Sint_b(Sint a, Sint b) {
    Bool r = 0;
    r = ic_bool_new(a <= b);
    return r;
}
/* builtin fn greaterthan_equal(a::Sint, b::Sint) -> Bool */
Bool greaterthan_equal_a_Sint_Sint_b(Sint a, Sint b) {
    Bool r = 0;
    r = ic_bool_new(a >= b);
    return r;
}

Uint ic_uint_new(uint32_t integer) {
    Uint i = integer;
    return i;
}
/* builtin fn print(a::Sint) */
Unit print_a_Uint_b(Uint i) {
    printf("%" PRId32, i);
    return 0;
}
/* builtin fn println(a::Sint) */
Unit println_a_Uint_b(Uint i) {
    print_a_Uint_b(i);
    println_a_b();
    return 0;
}
/* builtin fn equal(a::Sint, b::Sint) -> Bool */
Bool equal_a_Uint_Uint_b(Uint a, Uint b) {
    if (a == b) {
        /* return truthy boolean */
        return ic_bool_new(1);
    }
    /* return falsey boolean */
    return ic_bool_new(0);
}
/* builtin fn plus(a::Sint, b::Sint) -> Sint */
Uint plus_a_Uint_Uint_b(Uint a, Uint b) {
    Uint i = 0;
    i = ic_uint_new(a + b);
    return i;
}
/* builtin fn minus(a::Sint, b::Sint) -> Sint */
Uint minus_a_Uint_Uint_b(Uint a, Uint b) {
    Uint i = 0;
    i = ic_uint_new(a - b);
    return i;
}
/* builtin fn multiply(a::Sint, b::Sint) -> Sint */
Uint multiply_a_Uint_Uint_b(Uint a, Uint b) {
    Uint i = 0;
    i = ic_uint_new(a * b);
    return i;
}
/* builtin fn divide(a::Sint, b::Sint) -> Sint */
Uint divide_a_Uint_Uint_b(Uint a, Uint b) {
    Uint i = 0;
    i = ic_uint_new(a / b);
    return i;
}
/* builtin fn modulo(a::Sint, b::Sint) -> Sint */
Uint modulo_a_Uint_Uint_b(Uint a, Uint b) {
    Uint i = 0;
    i = ic_uint_new(a % b);
    return i;
}
/* builtin fn lessthan(a::Uint, b::Uint) -> Bool */
Bool lessthan_a_Uint_Uint_b(Uint a, Uint b) {
    Bool r = 0;
    r = ic_bool_new(a < b);
    return r;
}
/* builtin fn greaterthan(a::Uint, b::Uint) -> Bool */
Bool greaterthan_a_Uint_Uint_b(Uint a, Uint b) {
    Bool r = 0;
    r = ic_bool_new(a > b);
    return r;
}
/* builtin fn lessthan_equal(a::Uint, b::Uint) -> Bool */
Bool lessthan_equal_a_Uint_Uint_b(Uint a, Uint b) {
    Bool r = 0;
    r = ic_bool_new(a <= b);
    return r;
}
/* builtin fn greaterthan_equal(a::Uint, b::Uint) -> Bool */
Bool greaterthan_equal_a_Uint_Uint_b(Uint a, Uint b) {
    Bool r = 0;
    r = ic_bool_new(a >= b);
    return r;
}

/* builtin type String */
struct String {
    char *str;
    unsigned int len;
};

String ic_string_new(char *str, unsigned int len) {
    String s = 0;
    s = ic_alloc(sizeof(String));
    /* FIXME sharing str */
    s->str = str;
    s->len = len;
    return s;
}
/* builtin fn print(a::String) */
Unit print_a_String_b(String s) {
    printf("%s", s->str);
    return 0;
}
/* builtin fn println(a::String) */
Unit println_a_String_b(String s) {
    print_a_String_b(s);
    println_a_b();
    return 0;
}
/* builtin fn equal(a::String, b::String) -> Bool */
Bool equal_a_String_String_b(String a, String b) {
    if (strcmp(a->str, b->str)) {
        /* return falsey boolean */
        return ic_bool_new(0);
    }
    /* return truthy boolean */
    return ic_bool_new(1);
}
/* builtin fn concat(a::String, b::String) -> String */
String concat_a_String_String_b(String a, String b) {
    String s = 0;
    s = ic_alloc(sizeof(String));
    s->len = a->len + b->len;
    s->str = ic_alloc(s->len);
    strncpy(s->str, a->str, a->len);
    strncpy(&(s->str[a->len]), b->str, b->len);
    return s;
}
/* builtin fn length(a::String) -> Uint */
Uint length_a_String_b(String a) {
    return a->len;
}

/* builtin fn equal(a::Sint, b::Uint) -> Bool */
Bool equal_a_Sint_Uint_b(Sint a, Uint b) {
    Sint tmp = 0;
    if (b < 0) {
        return 0;
    }
    tmp = b;
    if (a == b) {
        return 1;
    }
    return 0;
}
/* builtin fn equal(a::Uint, b::Sint) -> Bool */
Bool equal_a_Uint_Sint_b(Uint a, Sint b) {
    Sint tmp = 0;
    if (a < 0) {
        return 0;
    }
    tmp = a;
    if (a == b) {
        return 1;
    }
    return 0;
}
