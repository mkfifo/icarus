#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "data/token.h"
#include "data/token_list.h"
#include "table.h"
#include "data/lex_data.h"
#include "lexer.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"

static unsigned int ic_lex_comment(struct ic_lex_data *lex_data);
static unsigned int ic_lex_identifier(struct ic_lex_data *lex_data);
static unsigned int ic_lex_literal_integer(struct ic_lex_data *lex_data);
static unsigned int ic_lex_literal_string(struct ic_lex_data *lex_data);

/* takes a character array of the source program
 *
 * takes the filename for current source unit, this
 * is attached to each token
 *
 * returns a token_list on success
 * returns 0 on failure
 */
struct ic_token_list * ic_lex(char *filename, char *source){
    /* final return value, only used at end of function */
    struct ic_token_list *final_value = 0;

    struct ic_lex_data * lex_data = 0;

    /* str from table */
    char * table_str = 0;
    /* len from table */
    unsigned int table_len = 0;
    /* id from table */
    enum ic_token_id table_id = 0;

    /* temporary token we construct */
    struct ic_token *token = 0;

    /* offset into table */
    unsigned int t_i = 0;
    /* table length */
    unsigned int t_len = 0;

    if( ! source ){
        puts("ic_lex: source was null");
        return 0;
    }

    lex_data = ic_lex_data_new(filename, source);
    if( ! lex_data ){
        puts("ic_lex: call to ic_lex_data_new failed");
        return 0;
    }

    t_len = IC_LEX_TABLE_LEN();

    /* go through source */
    for( lex_data->s_i = 0; lex_data->s_i < lex_data->s_len; ){
        token = 0;

        /* check if this exists within the table */
        for( t_i = 0; t_i < t_len; ++t_i ){
            table_str = table[t_i].str;
            table_len = table[t_i].len;
            table_id  = table[t_i].id;

            if( lex_data->s_i + table_len > lex_data->s_len ){
                /* no way it could fit */
                continue;
            }

            if( strncmp(&(source[lex_data->s_i]), table_str, table_len) ){
                /* strings did not match */
                continue;
            }

            /* we have a match !  */

            /* build a new token */
            token = ic_token_new(table_id, lex_data->start_of_line, lex_data->offset_into_line, lex_data->filename, lex_data->line_num);
            if( ! token ){
                puts("ic_lex: call to ic_token_new failed");
                return 0;
            }

            /* append token */
            if( ! ic_token_list_append(lex_data->token_list, token) ){
                puts("ic_lex: call to ic_token_list_append failed");
                return 0;
            }

            /* book keeping */
            /* update i
             * update offset into line
             */
            lex_data->s_i += table_len;
            lex_data->offset_into_line += table_len;

            /* if our match was a \n then we need to:
             *  increment the line counter
             *  reset offset into line
             *  capture current start of line
             */
            if( table_id == IC_NEWLINE ){
                lex_data->line_num += 1;
                lex_data->offset_into_line = 0;
                lex_data->start_of_line = &(lex_data->source[lex_data->s_i]);
            }
        }

        if( token ){
            /* inner loop matched, stop processing this time round */
            continue;
        }

        /* otherwise this is something with a payload
         * one of:
         *  comment
         *  string literal
         *  integer literal
         *  identifier
         */
        switch( source[lex_data->s_i] ){
            case '#':
                /* attempt lexing as comment */
                if( ! ic_lex_comment(lex_data) ){
                    puts("ic_lex: call to ic_lex_comment failed");
                    return 0;
                }
                break;

            case '"':
            case '\'':
                /* attempt lexing as string literal */
                if( ! ic_lex_literal_string(lex_data) ){
                    puts("ic_lex: call to ic_lex_literal_string failed");
                    return 0;
                }
                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                /* attempt lexing as integer literal */
                if( ! ic_lex_literal_integer(lex_data) ){
                    puts("ic_lex: call to ic_lex_literal_integer failed");
                    return 0;
                }
                break;

            default:
                /* attempt lexing as identifier */
                if( ! ic_lex_identifier(lex_data) ){
                    puts("ic_lex: call to ic_lex_identifier failed");
                    return 0;
                }
                break;
        }

        if( token ){
            /* inner loop matched, stop processing this time round */
            continue;
        }

        printf("ic_lex: lexing failed at character '%c' offset '%d' \n",
            source[lex_data->s_i],
            lex_data->s_i
        );
        return 0;
    }

    /* lexing was a success */
    final_value = lex_data->token_list;

    /* clean up lex_data */
    if( ! ic_lex_data_destroy(lex_data, 1) ){
        puts("ic_lex: call to ic_lex_data failed");
        return 0;
    }

    /* return generated token list */
    return final_value;
}

static unsigned int ic_lex_comment(struct ic_lex_data *lex_data){
    /* FIXME consume rest of line into token */
    /* FIXME add payload */
    /* FIXME maintain lex_data while we do so */
    puts("ic_lex_comment: unimplemented");
    return 0;
}

static unsigned int ic_lex_identifier(struct ic_lex_data *lex_data){
    /* FIXME consume */
    /* FIXME add payload */
    /* FIXME maintain lex_data while we do so */
    puts("ic_lex_comment: unimplemented");
    return 0;
}

static unsigned int ic_lex_literal_integer(struct ic_lex_data *lex_data){
    /* temporary token we construct */
    struct ic_token *token = 0;

    /* first char in integer found */
    char * integer_start = 0;
    /* length of integer found */
    unsigned int integer_len = 0;
    /* value from strtol */
    long int integer_value = 0;

    /* endptr used by strtol */
    char *endptr = 0;

    if( ! lex_data ){
        puts("ic_lex_literal_integer: lex_data was null");
        return 0;
    }

    integer_start = &(lex_data->source[lex_data->s_i]);
    for( integer_len = 0; ; ++integer_len ){
        /* safety net for overrunning buffer */
        if( lex_data->s_i + integer_len > lex_data->s_len ){
            break;
        }

        switch( lex_data->source[lex_data->s_i + integer_len] ){
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;

            case 'x':
            case 'b':
                puts("ic_lex_literal_integer: hex and binary are not yet supported");
                return 0;
                break;

            default:
                /* final character, stop
                 * NB: this could be \0
                 */
                goto SUCCESS;
                break;
        };
    }

SUCCESS:

    /* build a new token */
    token = ic_token_new(IC_LITERAL_INTEGER, lex_data->start_of_line, lex_data->offset_into_line, lex_data->filename, lex_data->line_num);
    if( ! token ){
        puts("ic_lex_literal_integer: call to ic_token_new failed");
        return 0;
    }

    /* append token */
    if( ! ic_token_list_append(lex_data->token_list, token) ){
        puts("ic_lex_literal_integer: call to ic_token_list_append failed");
        return 0;
    }

    /* book keeping */
    /* update i
     * update offset into line
     */
    lex_data->s_i += integer_len;
    lex_data->offset_into_line += integer_len;

    /* capture payload */
    integer_value = strtol(integer_start, &endptr, 10);
    /* check value is not out of range */
    if( integer_value == LONG_MAX ){
        puts("ic_lex_literal_integer: call to strtol overflowed");
        return 0;
    }
    if( integer_value == LONG_MIN ){
        puts("ic_lex_literal_integer: call to strtol underflowed");
        return 0;
    }

    /* check we gobbled up as many characters as we expected */
    if( integer_len != (endptr - integer_start) ){
        puts("ic_lex_literal_integer: strtol parsed length was not as expected");
        return 0;
    }

    /* add payload */
    token->u.integer = integer_value;

    return 1;
}

static unsigned int ic_lex_literal_string(struct ic_lex_data *lex_data){
    /* FIXME consume */
    /* FIXME add payload */
    /* FIXME maintain lex_data while we do so */
    puts("ic_lex_literal_string: unimplemented");
    return 0;
}

