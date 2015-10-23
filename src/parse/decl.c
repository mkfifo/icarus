#include <stdio.h> /* puts, printf */
#include <string.h> /* strncmp */
#include <stdlib.h> /* free */

#include "parse.h"

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

struct ic_decl * ic_parse_decl_builtin(struct ic_token_list *token_list){
#ifdef DEBUG_PARSE
    puts("ic_parse_decl_builtin called");
#endif

    puts("ic_parse_decl_builtin: UNIMPLEMENTED");

    /* FIXME */
    return 0;
}

struct ic_decl * ic_parse_decl_type(struct ic_token_list *token_list){
    /* our resulting ic_decl */
    struct ic_decl *decl = 0;

    if( ! token_list ){
        puts("ic_parse_decl_type: token_list was null");
        return 0;
    }

    decl = ic_parse_decl_type_header(token_list);
    if( ! decl ){
        puts("ic_parse_decl_type: call to ic_parse_decl_type_header failed");
        return 0;
    }

    decl = ic_parse_decl_type_body(token_list, decl);
    if( ! decl ){
        puts("ic_parse_decl_type: call to ic_parse_decl_type_body failed");
        return 0;
    }

    return decl;
}

struct ic_decl * ic_parse_decl_type_header(struct ic_token_list *token_list){
    /* our resulting ic_decl */
    struct ic_decl *decl = 0;
    /* our tdecl within the decl */
    struct ic_decl_type *tdecl = 0;
    /* current token we are looking at */
    struct ic_token *token = 0;

    if( ! token_list ){
        puts("ic_parse_decl_type_header: token_list was null");
        return 0;
    }

    /* check we really are at a `type` token */
    token = ic_token_list_expect_important(token_list, IC_TYPE);
    if( ! token ){
        puts("ic_parse_decl_type_header: call to ic_token_list_next failed for func");
        return 0;
    }

    /* allocate and init our decl */
    decl = ic_decl_new(ic_decl_tag_type);
    if( ! decl ){
        puts("ic_parse_decl_type_header: call to ic_decl_new failed");
        return 0;
    }

    /* fetch our tdecl from within decl */
    tdecl = ic_decl_get_tdecl(decl);
    if( ! tdecl ){
        puts("ic_parse_decl_type_header: call to ic_decl_get_tdecl failed");
        free(decl);
        return 0;
    }

    /* get our type name */
    token = ic_token_list_next_important(token_list);
    if( ! token ){
        puts("ic_parse_decl_type_header: call to ic_token_list_next failed for func name");
        return 0;
    }

    /* initialise our tdecl */
    if( ! ic_decl_type_init(tdecl, ic_token_get_string(token), ic_token_get_string_length(token)) ){
        puts("ic_parse_decl_type_header: call to ic_decl_type_init failed");
        free(decl);
        return 0;
    }

    return decl;
}

struct ic_decl * ic_parse_decl_type_body(struct ic_token_list *token_list, struct ic_decl *decl){
    /* parsed field */
    struct ic_field *field = 0;
    /* our tdecl within the decl */
    struct ic_decl_type *tdecl = 0;
    /* used to mark success of while loop */
    int success = 0;
    /* current token we are looking at */
    struct ic_token *token = 0;

#ifdef DEBUG_PARSE
    puts("ic_parse_decl_type called");
#endif

    if( ! token_list ){
        puts("ic_parse_decl_type_body: token_list was null");
        return 0;
    }

    /* fetch our tdecl from within decl */
    tdecl = ic_decl_get_tdecl(decl);
    if( ! tdecl ){
        puts("ic_parse_decl_type_body: call to ic_decl_get_tdecl failed");
        free(decl);
        return 0;
    }

    /* iterate through all tokens */
    /* keep iterating through all the tokens until we find an unexpected 'end' */
    while( (token = ic_token_list_peek_important(token_list)) ){
        /* check if this is the end */
        if( token->id == IC_END ){
            success = 1;
            break;
        }

        /* otherwise this is a field
         * parse it
         */
        field = ic_parse_field(token_list);
        if( ! field ){
            puts("ic_parse_decl_type_body: call to ic_parse_field failed");

            /* free decl and all contents */
            if( ! ic_decl_destroy(decl, 1) ){
                puts("ic_parse_decl_type_body: in error tidyup call to ic_decl_destroy failed");
            }
            return 0;
        }

        /* and store it */
        if( ! ic_decl_type_add_field(tdecl, field) ){
            puts("ic_parse_decl_type_body: call to ic_decl_type_add_field failed");

            /* free field and all contents */
            if( ! ic_field_destroy(field, 1) ){
                puts("ic_parse_decl_type_body: in error tidyup call to ic_field_destroy failed");
            }

            /* free decl and all contents */
            if( ! ic_decl_destroy(decl, 1) ){
                puts("ic_parse_decl_type_body: in error tidyup call to ic_decl_destroy failed");
            }
            return 0;
        }

        /* increment of i is handled by ic_parse_field */
    }

    /* if it is then consume */
    token = ic_token_list_expect_important(token_list, IC_END);
    if( ! token ){
        puts("ic_parse_decl_type_body: call to ic_token_list_expect for end failed");
        return 0;
    }

    /* if success is 1 then this means we hit `end` as
     * we expected and our type decl is finished
     */
    if( success ){
        /* victory ! */
        return decl;
    }

    /* if ret is not 1 then
     * this means we ran out of tokens
     * this is an error case as `end` should cause the
     * successful return
     */
    puts("ic_parse_decl_type_body: something went wrong in finding end");

    /* free decl and all contents */
    if( ! ic_decl_destroy(decl, 1) ){
        puts("ic_parse_decl_type_body: in error tidyup call to ic_decl_destroy failed");
    }
    return 0;
}

struct ic_decl * ic_parse_decl_enum(struct ic_token_list *token_list){
#ifdef DEBUG_PARSE
    puts("ic_parse_decl_enum called");
#endif

    puts("ic_parse_decl_enum: UNIMPLEMENTED");

    /* FIXME */
    return 0;
}

struct ic_decl * ic_parse_decl_union(struct ic_token_list *token_list){
#ifdef DEBUG_PARSE
    puts("ic_parse_decl_union called");
#endif

    puts("ic_parse_decl_union: UNIMPLEMENTED");

    /* FIXME */
    return 0;
}

struct ic_decl * ic_parse_decl_func(struct ic_token_list *token_list){
    struct ic_decl *decl = 0;

    if( ! token_list ){
        puts("ic_parse_decl_func: token_list was null");
        return 0;
    }

    decl = ic_parse_decl_func_header(token_list);
    if( ! decl ){
        puts("ic_parse_decl_func: call to ic_parse_decl_func_header failed");
        return 0;
    }

    decl = ic_parse_decl_func_body(token_list, decl);
    if( ! decl ){
        puts("ic_parse_decl_func: call to ic_parse_decl_func_body failed");
        return 0;
    }

    return decl;
}

struct ic_decl * ic_parse_decl_func_header(struct ic_token_list *token_list){
    /* our argument */
    struct ic_field *arg = 0;
    /* our resulting ic_decl */
    struct ic_decl *decl = 0;
    /* our tdecl within the decl */
    struct ic_decl_func *fdecl = 0;
    /* current token */
    struct ic_token *token = 0;

    if( ! token_list ){
        puts("ic_parse_decl_func_header: token_list was null");
        return 0;
    }

    /* check we really are at a `function` token */
    token = ic_token_list_expect_important(token_list, IC_FUNC);
    if( ! token ){
        puts("ic_parse_decl_type: call to ic_token_list_next failed for func");
        return 0;
    }

    /* allocate and init our decl */
    decl = ic_decl_new(ic_decl_tag_func);
    if( ! decl ){
        puts("ic_parse_decl_func_header: call to ic_decl_new failed");
        return 0;
    }

    /* fetch our fdecl from within decl */
    fdecl = ic_decl_get_fdecl(decl);
    if( ! fdecl ){
        puts("ic_parse_decl_func_header: call to ic_decl_get_fdecl failed");
        free(decl);
        return 0;
    }

    /* get our function name token */
    token = ic_token_list_next_important(token_list);
    if( ! token ){
        puts("ic_parse_decl_type: call to ic_token_list_next failed for func name");
        return 0;
    }

    if( token->id != IC_IDENTIFIER ){
        printf("ic_parse_decl_func_header: expected IC_IDENTIFIER token, got: ");
        ic_token_id_print_debug(token->id);
        puts("");
        return 0;
    }

    /* initialise our fdecl */
    if( ! ic_decl_func_init(fdecl, ic_token_get_string(token), ic_token_get_string_length(token)) ){
        puts("ic_parse_decl_func_header: call to ic_decl_func_init failed");
        free(decl);
        return 0;
    }

    /* parse arguments */
    /* opening bracket */
    token = ic_token_list_expect_important(token_list, IC_LRBRACKET);
    if( ! token ){
        puts("ic_parse_decl_type: call to ic_token_list_next failed for '('");
        return 0;
    }

    /* iterate until closing bracket
     * parsing arguments
     */
    while( (token = ic_token_list_peek_important(token_list)) ){
        if( token->id == IC_RRBRACKET ){
            break;
        }

        /* parse argument */
        arg = ic_parse_field(token_list);
        if( ! arg ){
            puts("ic_parse_decl_func_header: call to ic_parse_field failed");
            free(decl);
            return 0;
        }

        /* save it */
        if( ! ic_decl_func_add_arg(fdecl, arg) ){
            puts("ic_parse_decl_func_header: call to if_decl_func_add_arg failed");
            free(decl);
            return 0;
        }
    }

    token = ic_token_list_expect_important(token_list, IC_RRBRACKET);
    if( ! token ){
        puts("ic_parse_decl_type: call to ic_token_list_next failed for ')'");
        return 0;
    }

    /* parse return type
     * check if we have a return type arrow ->
     *
     * if we do, consume it, and then parse return type
     *
     * if we do not, then there is no return type and we set this function as Void
     * then begin parsing the body
     */
    token = ic_token_list_peek_important(token_list);
    if( ! token ){
        puts("ic_parse_decl_type: call to ic_token_list_next failed when looking for return type or body");
        return 0;
    }

    if( token->id == IC_ARROW ){
        /* consume arrow */
        token = ic_token_list_next_important(token_list);
        if( ! token ){
            puts("ic_parse_decl_type: call to ic_token_list_next_important failed when trying to consume arrow");
            return 0;
        }

        /* we found a type arrow and have skipped over it
         * parse return type
         */
        token = ic_token_list_expect_important(token_list, IC_IDENTIFIER);
        if( ! token ){
            puts("ic_parse_decl_type: call to ic_token_list_next failed when looking for return type after arrow");
            return 0;
        }

        /* add to our fdecl */
        if( ! ic_decl_func_set_return( fdecl, ic_token_get_string(token), ic_token_get_string_length(token)) ){
            puts("ic_parse_decl_func_header: call to ic_decl_func_set_return failed");
            free(decl);
            return 0;
        }

    } else {
        /* add Void type to our fdecl */
        if( ! ic_decl_func_set_return(fdecl, "Void", 4) ){
            puts("ic_parse_decl_func_header: call to ic_decl_func_set_return failed for 'Void'");
            free(decl);
            return 0;
        }
    }

    return decl;
}

struct ic_decl * ic_parse_decl_func_body(struct ic_token_list *token_list, struct ic_decl *decl){
    /* our tdecl within the decl */
    struct ic_decl_func *fdecl = 0;
    /* used to mark success of while loop */
    int success  = 0;
    /* pointer used for each stmt within body */
    struct ic_stmt *stmt = 0;
    /* current token */
    struct ic_token *token = 0;

    /* fn name(args...) -> return_type
     *      body...
     * end
     *
     * # default to void return type
     * fn name(args...)
     *      body...
     * end
     */

#ifdef DEBUG_PARSE
    puts("ic_parse_decl_func called");
#endif

    /* fetch our fdecl from within decl */
    fdecl = ic_decl_get_fdecl(decl);
    if( ! fdecl ){
        puts("ic_parse_decl_func_body: call to ic_decl_get_fdecl failed");
        free(decl);
        return 0;
    }

    /* parse body */

    /* iterate through all tokens
     * until `end`
     */
    while( (token = ic_token_list_peek_important(token_list)) ){
        if( token->id == IC_END ){
            success = 1;
            break;
        }

        /* leave stmt parsing up to the experts */
        stmt = ic_parse_stmt(token_list);
        if( ! stmt ){
            puts("ic_parse_decl_func_body: call to ic_parse_stmt failed");
            free(decl);
            return 0;
        }

        /* save to our body */
        if( ! ic_decl_func_add_stmt(fdecl, stmt) ){
            puts("ic_parse_decl_func_body: call to ic_decl_func_add_stmt failed");
            free(decl);
            return 0;
        }
    }

    /* consume end token */
    token = ic_token_list_expect_important(token_list, IC_END);
    if( ! token ){
        puts("ic_parse_decl_func_body: call to ic_token_list_expect for end failed");
        return 0;
    }

    /* if success is 1 then we found an `end` token */
    if( success ){
        /* victory ! */
        return decl;
    }

    /* this means we ran out of tokens
     * or that some other error occured
     *
     * this is an error case as `end` should cause the
     * successful return
     */
    puts("ic_parse_decl_func_body: error occurred in ic_parse_this_is_not_the_end");
    free(decl);
    return 0;
}


