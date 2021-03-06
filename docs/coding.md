Implementation coding standards guide
=====================================

Error handling:
---------------

On error all functions should print their function name and then a description of the error,
all such printouts should be traceable to a single line number, so all error strings should be globally unique.

Every function should return 0 in the case of error.

Example conforming function:

    unsigned int ic_foo_bar(struct ic_foo *foo){
        if( ! foo ){
            puts("ic_foo: called with null foo");
            return 0;
        }

        if( ... ){
            puts("ic_foo: an error description goes here");
            return 0;
        }

        return 1;
    }

allowed special cases:

* ic_ast_append will return the index of the appended item, returns -1 on error
* ic_body_append will return the index of the appended item, returns -1 on error
* ic_pvector_append will return the index of the appended item, returns -1 on error
* ic_string_length will return the length, returns -1 on error
* ic_symbol_length will return the length, returns -1 on error

known exceptions:

    data/string.c:143: * returns -1 on error
    data/string.h:81: * returns -1 on error
    data/symbol.c:106: * returns -1 on error
    data/symbol.h:53: * returns -1 on error
    src/data/pvector.c:158: * returns -1 on failure
    src/data/pvector.h:78: * returns -1 on failure
    src/parse/data/ast.c:125: * returns -1 on failure
    src/parse/data/ast.h:49: * returns -1 on failure
    src/parse/data/body.c:121: * returns -1 on failure
    src/parse/data/body.h:50: * returns -1 on failure


Error checking
==============

most functions in icarus return 0 on error, this should be checked as such

    if( ! ic_foo_bar(...) ){
        puts("ic_baz: call to ic_foo_bar failed");
        return 0;
    }


some functions return a value to be assigned, the preferred form for this is

    struct ic_foo *foo = 0;

    foo = ic_foo_new();
    if( ! foo ){
        puts("ic_baz: call to ic_foo_new failed");
        return 0;
    }


there are a few functions which use 0 as a valid response and so need `-1` to indicate error, the form
for checking this is

    if( -1 == ic_foo_append(...) ){
        ...
    }

this is so that a grep for 'if( ic_foo' will not flag this line, which allows us to more easily find functions
not conforming to the first error checking rule above.


Function documentation
======================

all functions should document their return values

    /* does nothing
     * returns 1 on success
     * returns 0 on failure
     */
    unsigned int foo(void);

note that the wording 'on failure' is preferred over 'on error'

