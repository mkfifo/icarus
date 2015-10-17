#ifndef IC_TYPE_H
#define IC_TYPE_H

#include "../../parse/data/decl.h"
#include "type_builtin.h"

/* an instance of a type
 * ic_type has a single instance for each type within a kludge (FIXME consider type scoping)
 *
 * an ic_type can either be:
 *  - a builtin - FIXME no current support for builtins
 *  - a user defined type - ic_decl_type
 */

enum ic_type_tag {
    ic_type_builtin,
    ic_type_user
};

struct ic_type {
    enum ic_type_tag tag;
    union {
        struct ic_type_builtin *builtin;
        struct ic_decl_type *decl;
    } u;
};

/* alloc and init a new type representing a tdecl
 *
 * returns new type on success
 * returns 0 on failure
 */
struct ic_type * ic_type_new_tdecl(struct ic_decl_type *decl);

/* init an existing type representing a tdecl
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_type_init_tdecl(struct ic_type *type, struct ic_decl_type *decl);
/* alloc and init a new type representing a builtin
 *
 * returns new type on success
 * returns 0 on failure
 */
struct ic_type * ic_type_new_builtin(struct ic_type_builtin *builtin);

/* init an existing type representing a builtin
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_type_init_builtin(struct ic_type *type, struct ic_type_builtin *builtin);

/* destroy a type
 *
 * will only free the type if `free_type` is truthy
 *
 * will NOT free any decl or builtin objects
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_type_destroy(struct ic_type *type, unsigned int free_type);

/* get builtin from type
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_type_builtin * ic_type_get_builtin(struct ic_type *type);

/* get decl from type
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type * ic_type_get_decl(struct ic_type *type);

/* return a symbol representing the name of this type
 *
 * returns ic_symbol * on sucess
 * returns 0 on failure
 */
struct ic_symbol * ic_type_name(struct ic_type *type);

/* is this type void
 *
 * returns 1 if provided type is Void
 * returns 0 if provided type is NOT void
 */
unsigned int ic_type_isvoid(struct ic_type *type);

/* are these 2 types the equal
 *
 * returns 1 if they are equal
 * returns 0 of they are NOT equal
 */
unsigned int ic_type_equal(struct ic_type *a, struct ic_type *b);

#endif
