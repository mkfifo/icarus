#include <stdio.h>
#include <stdlib.h>

#include "type.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

/* allocate and intialise a new type
 * this will set type.type to unknown
 *
 * returns new type on success
 * returns 0 on failure
 */
struct ic_type * ic_type_new(void){
    struct ic_type *type = 0;

    type = calloc(1, sizeof(struct ic_type));
    if( ! type ){
        puts("ic_type_new: call to calloc failed");
        return 0;
    }

    if( ic_type_init(type) ){
        puts("ic_type_new: call to ic_type_init failed");
        return 0;
    }

    return type;
}

/* intialise a type
 * this will set type.type to unknown
 *
 * returns 0 on success
 * returns 1 on failure
 */
unsigned int ic_type_init(struct ic_type *type){
    if( ! type ){
        puts("ic_type_init: type was null");
        return 1;
    }

    /* default to unknown types
     * other type_type(s) are set via methods
     */
    type->type = ic_type_unknown;

    return 0;
}

/* destroy type
 *
 * this will only free type if `free_type` is truthy
 *
 * returns 0 on sucess
 * returns 1 on error
 */
unsigned int ic_type_destroy(struct ic_type *type, unsigned int free_type){
    if( ! type ){
        puts("ic_type_destroy: type was null");
        return 1;
    }

    /* cleanup depends on type_type */
    switch( type->type ){
        case ic_type_unknown:
            /* nothing to do */
            break;

        case ic_type_symbol:
            /* clean up symbol, do not free as member */
            if( ic_symbol_destroy( &(type->u.sym), 0 ) ){
                puts("ic_type_destroy: call to ic_symbol_destroy failed");
                return 1;
            }
            break;

        case ic_type_tdecl:
            /* nothing to do, tdecl is not onwed by us */
            break;

        default:
            puts("ic_type_destroy: type->type was impossible type_type");
            return 1;
            break;
    }

    /* if asked nicely */
    if( free_type ){
        free(type);
    }

    return 0;
}

/* set the sym on this type from the provided string
 * this will change type.type to sym
 *
 * Note that this is ONLY allowed if a tdecl hasn't already been set
 *
 * if type.type is tdecl then calling this function is an error
 * as that would be going 'backwards'
 *
 * returns 0 on success
 * returns 1 on error
 */
unsigned int ic_type_set_symbol(struct ic_type *type, char *type_str, unsigned int type_len){
    if( ! type ) {
        puts("ic_type_set_symbol: type was null");
        return 1;
    }

    if( ! type_str ) {
        puts("ic_type_set_symbol: type_str was null");
        return 1;
    }

    /* may have to do cleanup or raise errors based on current
     * type->type
     */
    switch( type->type ){
        case ic_type_unknown:
            /* nothing to do */
            break;

        case ic_type_symbol:
            /* error, already a symbol */
            puts("ic_type_set_symbol: type was already a symbol");
            return 1;
            break;

        case ic_type_tdecl:
            /* error, already a tdecl */
            puts("ic_type_set_symbol: type was already a tdecl");
            return 1;
            break;

        default:
            puts("ic_type_set_symbol: type->type was impossible type_type");
            return 1;
            break;
    }

    /* set to tdecl */
    type->type = ic_type_symbol;

    /* set our symbol from the provider char * and len */
    if( ic_symbol_init(&(type->u.sym), type_str, type_len) ){
        puts("ic_type_set_symbol: call to ic_symbol_init failed");
        return 1;
    }

    return 0;
}

/* set the *tdecl on this type
 * this will change type.type to tdecl
 *
 * this is only allowed it the type is NOT already set to tdecl
 * if type is already a symbol then the symbol will first be destroyed
 *
 * returns 0 on success
 * returns 1 on error
 */
unsigned int ic_type_set_tdecl(struct ic_type *type, struct ic_type_decl *tdecl){
    if( ! type ) {
        puts("ic_type_set_tdecl: type was null");
        return 1;
    }

    if( ! tdecl ) {
        puts("ic_type_set_tdecl: tdecl was null");
        return 1;
    }

    /* may have to do cleanup or raise errors based on current
     * type->type
     */
    switch( type->type ){
        case ic_type_unknown:
            /* nothing to do */
            break;

        case ic_type_symbol:
            /* clean up symbol, do not free as member */
            if( ic_symbol_destroy( &(type->u.sym), 0 ) ){
                puts("ic_type_set_tdecl: call to ic_symbol_destroy failed");
                return 1;
            }
            break;

        case ic_type_tdecl:
            /* error, already a tdecl */
            puts("ic_type_set_tdecl: type was already a tdecl");
            return 1;
            break;

        default:
            puts("ic_type_set_tdecl: type->type was impossible type_type");
            return 1;
            break;
    }

    /* set to tdecl */
    type->type = ic_type_tdecl;

    /* store our actual tdecl */
    type->u.tdecl = tdecl;

    return 0;
}

