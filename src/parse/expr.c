#include <stdio.h> /* puts */

#include "../parse.h"

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* ignore unused functions */
#pragma GCC diagnostic ignored "-Wunused-function"

/* ignore unused variables */
#pragma GCC diagnostic ignored "-Wunused-variable"

/* ignore unused variables that are set */
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

/* current supported expression types:
 *  func call
 *  identifier
 *  constant
 *  operator application
 */


/* consume token
 * returns ic_expr* on success
 * returns 0 on failure
 */
static struct ic_expr * ic_parse_expr_fcall(struct ic_tokens *tokens, unsigned int *i){
    struct ic_expr * expr = 0;

    if( ! tokens ){
        puts("ic_parse_expr_fcall: tokens was null");
        return 0;
    }
    if( ! i ){
        puts("ic_parse_expr_fcall: i was null");
        return 0;
    }

    puts("ic_parse_expr_fcall: unimplemented");
    return 0;
}

/* consume token
 * returns ic_expr* on success
 * returns 0 on failure
 */
static struct ic_expr * ic_parse_expr_identifier(struct ic_tokens *tokens, unsigned int *i){
    struct ic_expr * expr = 0;

    if( ! tokens ){
        puts("ic_parse_expr_identifier: tokens was null");
        return 0;
    }
    if( ! i ){
        puts("ic_parse_expr_identifier: i was null");
        return 0;
    }

    puts("ic_parse_expr_identifier: unimplemented");
    return 0;
}

/* consume token and make a string
 * returns ic_expr* on success
 * returns 0 on failure
 */
static struct ic_expr * ic_parse_expr_constant_string(struct ic_tokens *tokens, unsigned int *i){
    /* our eventual return value */
    struct ic_expr * expr = 0;
    /* the i value that marks the beginning of our string
     * this is the offset *after* the opening " is read
     */
    unsigned int start = 0;
    /* used to record distance of tokens */
    unsigned int dist = 0;
    /* sum of all the distances of tokens in this string */
    unsigned int dist_sum = 0;

    if( ! tokens ){
        puts("ic_parse_expr_constant_string: tokens was null");
        return 0;
    }
    if( ! i ){
        puts("ic_parse_expr_constant_string: i was null");
        return 0;
    }

    /* check for opening quote */
    if( ic_parse_check_token("\"", 1, tokens->tokens, i) ){
        puts("ic_parse_expr_constant_string: failed to find opening quote (\")");
        return 0;
    }

    /* record our starting value */
    start = *i;

    /* consume string
     * we just iterate over it until we see the closing "
     * recording the distances to sum them up
     */
    while( ! ic_parse_check_token("\"", 1, tokens->tokens, i) ){
        dist = ic_parse_token_length(tokens->tokens, *i);
        if( ! dist ){
            puts("ic_parse_expr_constant_string: ran out of tokens while looking for closing quote (\")");
            return 0;
        }

        dist_sum += dist;
    }

    /* FIXME build string
     * we have the start of the string (start)
     * and the total length (dist_sum)
     */

    /* FIXME return value */

    puts("ic_parse_expr_constant_string: unimplemented");
    return 0;
}

/* consume token and make an int
 * returns ic_expr* on success
 * returns 0 on failure
 */
static struct ic_expr * ic_parse_expr_constant_integer(struct ic_tokens *tokens, unsigned int *i){
    struct ic_expr * expr = 0;

    if( ! tokens ){
        puts("ic_parse_expr_constant_integer: tokens was null");
        return 0;
    }
    if( ! i ){
        puts("ic_parse_expr_constant_integer: i was null");
        return 0;
    }

    puts("ic_parse_expr_constant_integer: unimplemented");
    return 0;
}


/* consume token
 * returns ic_expr* on success
 * returns 0 on failure
 */
static struct ic_expr * ic_parse_expr_operator(struct ic_tokens *tokens, unsigned int *i){
    puts("ic_parse_expr_operator: unimplemented");
    return 0;
}

struct ic_expr * ic_parse_expr(struct ic_tokens *tokens, unsigned int *i){
    /* pointer used to peek at start of next token */
    char *next = 0;

    if( ! tokens ){
        puts("ic_parse_expr: tokens was null");
        return 0;
    }
    if( ! i ){
        puts("ic_parse_expr: i was null");
        return 0;
    }

    /* rules
     *
     * we see a " token -> string value
     * we see a number as the first char -> int value
     * we see a symbol, need to inspect next token
     *      ( -> func call
     *      {+ - * ...} -> operator
     *      else -> identifier
     */
    if( ic_parse_stringish(tokens, i) ){
        return ic_parse_expr_constant_string(tokens, i);
    }

    if( ic_parse_numberish(tokens, i) ){
        return ic_parse_expr_constant_integer(tokens, i);
    }

    next = ic_parse_peek_next(tokens, i);
    if( ! next ){
        /* this in theory could mean out final token is an identifier
         * however this is illegal in practice as all expr/stmt must
         * appears inside a body ending with `end`
         * so this is indeed an error
         */
        puts("ic_parse_expr: call to ic_parse_peek_next failed");
        return 0;
    }

    /* basic support for operators
     * this will only support:
     *  identifier operator ...
     * as any numbers or strings are caught above
     *
     * FIXME reconsider this
     */
    if( ic_parse_operatorish(next) ){
        return ic_parse_expr_operator(tokens, i);
    }

    /* if we see an open bracket this is a function call */
    if( *next == '(' ){
        return ic_parse_expr_fcall(tokens, i);
    }

    /* otherwise assume this is just an identifier */
    return ic_parse_expr_identifier(tokens, i);
}

