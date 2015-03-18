#include <stdio.h> /* puts, printf */
#include <string.h> /* strncmp */

#include "../parse.h"

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

struct ic_decl * ic_parse_type_decl(struct ic_tokens *tokens, unsigned int *i){
    unsigned int dist = 0;
    /* parsed field */
    struct ic_field *field = 0;
    /* our resulting ic_decl */
    struct ic_decl *decl = 0;
    /* our tdecl within the decl */
    struct ic_type_decl *tdecl = 0;

#ifdef DEBUG_PARSE
    puts("ic_parse_type_decl called");
#endif

    if( ! tokens ){
        puts("ic_parse_type_decl: tokens was null");
        return 0;
    }

    /* check we really are at a `type` token */
    dist = ic_parse_token_length(tokens->tokens, *i);
    if( dist != 4 || strncmp("type", &(tokens->tokens[*i]), 4) ){
        printf("ic_parse_type_decl: expected 'type', encountered '%.*s'\n",
                dist,
                &(tokens->tokens[*i]));
        return 0;
    }

    /* step over `type` keyword */
    ic_parse_token_advance(i, dist);

    /* allocate and init our decl */
    decl = ic_decl_new(ic_decl_type_decl);
    if( ! decl ){
        puts("ic_parse_type_decl: call to ic_decl_new failed");
        return 0;
    }

    /* fetch our tdecl from within decl */
    tdecl = ic_decl_get_tdecl(decl);
    if( ! tdecl ){
        puts("ic_parse_type_decl: call to ic_decl_get_tdecl failed");
        return 0;
    }

    /* get our type name dist */
    dist = ic_parse_token_length(tokens->tokens, *i);
    /* initialise our tdecl */
    if( ic_type_decl_init(tdecl, &(tokens->tokens[*i]), dist) ){
        puts("ic_parse_type_decl: call to ic_type_decl_init failed");
        return 0;
    }

#ifdef DEBUG_PARSE
    printf("ic_parse_type_decl: our name is '%.*s'\n",
            dist,
            &(tokens->tokens[*i]));
#endif

    /* step over name */
    ic_parse_token_advance(i, dist);

    /* iterate through all tokens */
    for( ; tokens->tokens[*i] != '\0' && *i < tokens->len ; ){
        dist = ic_parse_token_length(tokens->tokens, *i);

#ifdef DEBUG_PARSE
        printf("ic_parse_token_type_decl: inspecting token '%.*s'\n",
                dist,
                &(tokens->tokens[*i]) );
#endif

        /* we keep stepping through loop until we find an
         * `end` token
         * note that this means `end` is a reserved word
         */
        if( dist == 3 &&
            ! strncmp( &(tokens->tokens[*i]), "end", 3) ){
            printf("ic_parse_token_type_decl: found end of string token '%.*s'\n",
                    dist,
                    &(tokens->tokens[*i]) );

            /* step over `end` token */
            ic_parse_token_advance(i, dist);

            /* return our result */
            return decl;
        }

        /* otherwise this is a field
         * parse it
         */
        field = ic_parse_field(tokens, i);
        if( ! field ){
            puts("ic_parse_type_decl: call to ic_parse_field failed");
            return 0;
        }

        /* and store it */
        if( ic_type_decl_add_field(tdecl, field) ){
            puts("ic_parse_type_decl: call to ic_type_decl_add_field failed");
            return 0;
        }

        /* increment of i is handled by ic_parse_field */
    }

    /* this means we ran out of tokens
     * this is an error case as `end` should cause the
     * successful return
     */
    return 0;
}

struct ic_decl * ic_parse_enum_decl(struct ic_tokens *tokens, unsigned int *i){
#ifdef DEBUG_PARSE
    puts("ic_parse_enum_decl called");
#endif

    puts("ic_parse_enum_decl: UNIMPLEMENTED");

    /* FIXME */
    return 0;
}

struct ic_decl * ic_parse_union_decl(struct ic_tokens *tokens, unsigned int *i){
#ifdef DEBUG_PARSE
    puts("ic_parse_union_decl called");
#endif

    puts("ic_parse_union_decl: UNIMPLEMENTED");

    /* FIXME */
    return 0;
}

struct ic_decl * ic_parse_func_decl(struct ic_tokens *tokens, unsigned int *i){
    unsigned int dist = 0;
    /* our argument */
    struct ic_field *arg = 0;
    /* our resulting ic_decl */
    struct ic_decl *decl = 0;
    /* our tdecl within the decl */
    struct ic_func_decl *fdecl = 0;

#ifdef DEBUG_PARSE
    puts("ic_parse_func_decl called");
#endif

    if( ! tokens ){
        puts("ic_parse_func_decl: tokens was null");
        return 0;
    }

    /* check we really are at a `function` token */
    dist = ic_parse_token_length(tokens->tokens, *i);
    if( dist != 8 || strncmp("function", &(tokens->tokens[*i]), 4) ){
        printf("ic_parse_func_decl: expected 'function', encountered '%.*s'\n",
                dist,
                &(tokens->tokens[*i]));
        return 0;
    }

    /* step over `function` keyword */
    ic_parse_token_advance(i, dist);

    /* allocate and init our decl */
    decl = ic_decl_new(ic_decl_func_decl);
    if( ! decl ){
        puts("ic_parse_func_decl: call to ic_decl_new failed");
        return 0;
    }

    /* fetch our fdecl from within decl */
    fdecl = ic_decl_get_fdecl(decl);
    if( ! fdecl ){
        puts("ic_parse_func_decl: call to ic_decl_get_fdecl failed");
        return 0;
    }

    /* get our function name dist */
    dist = ic_parse_token_length(tokens->tokens, *i);
    /* initialise our fdecl */
    if( ic_func_decl_init(fdecl, &(tokens->tokens[*i]), dist) ){
        puts("ic_parse_func_decl: call to ic_func_decl_init failed");
        return 0;
    }

#ifdef DEBUG_PARSE
    printf("ic_parse_func_decl: our name is '%.*s'\n",
            dist,
            &(tokens->tokens[*i]));
#endif

    /* step over name */
    ic_parse_token_advance(i, dist);


    /* parse arguments */
    /* opening bracket */
    dist = ic_parse_token_length(tokens->tokens, *i);
    if( dist != 1 ||
        strncmp("(", &(tokens->tokens[*i]), 1) ){
        printf("ic_parse_func_decl: expected '(', found '%.*s'\n",
                dist,
                &(tokens->tokens[*i]) );
        return 0;
    }
    /* step over opening bracket */
    ic_parse_token_advance(i, dist);

    /* iterate until closing bracket
     * parsing arguments
     */
    for( ; tokens->tokens[*i] != '\0' && *i < tokens->len ; ){
        /* get dist */
        dist = ic_parse_token_length(tokens->tokens, *i);

        /* check for closing brace */
        if( dist == 1 && ! strncmp(")", &(tokens->tokens[*i]), 1) ){
            /* if we have found our closing bracket then
             * argument parsing is complete
             */
            break;
        }

        /* parse argument */
        arg = ic_parse_field(tokens, i);
        if( ! arg ){
            puts("ic_parse_func_decl: call to ic_parse_field failed");
            return 0;
        }

        /* save it */
        if( ic_func_decl_add_arg(fdecl, arg) ){
            puts("ic_parse_func_decl: call to if_func_decl_add_arg failed");
            return 0;
        }
    }

    /* step over closing bracket */
    ic_parse_token_advance(i, dist);


    /* FIXME parse body */

    /* iterate through all tokens
     * until `end`
     */
    for( ; tokens->tokens[*i] != '\0' && *i < tokens->len ; ){
        dist = ic_parse_token_length(tokens->tokens, *i);

#ifdef DEBUG_PARSE
        printf("ic_parse_token_func_decl: inspecting token '%.*s'\n",
                dist,
                &(tokens->tokens[*i]) );
#endif

        /* we keep stepping through loop until we find an
         * `end` token
         * note that this means `end` is a reserved word
         */
        if( dist == 3 &&
            ! strncmp( &(tokens->tokens[*i]), "end", 3) ){
            printf("ic_parse_token_func_decl: found end of string token '%.*s'\n",
                    dist,
                    &(tokens->tokens[*i]) );

            /* step over `end` token */
            ic_parse_token_advance(i, dist);

            /* return our result */
            return decl;
        }

        /* FIXME
         * for now we just skip merrily over function body
         */
        ic_parse_token_advance(i, dist);
    }

    /* this means we ran out of tokens
     * this is an error case as `end` should cause the
     * successful return
     */
    puts("ic_parse_func_decl: ran out of tokens");
    return 0;
}

