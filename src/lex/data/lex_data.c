#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token_list.h"
#include "lex_data.h"

/* construct a new lex data
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_lex_data * ic_lex_data_new(char *filename, char *source){
    struct ic_lex_data *lex_data = 0;

    if( ! filename ){
        puts("ic_lex_data_new: filename was null");
        return 0;
    }

    if( ! source ){
        puts("ic_lex_data_new: source was null");
        return 0;
    }

    lex_data = calloc(1, sizeof(struct ic_lex_data));
    if( ! lex_data ){
        puts("ic_lex_data_new: call to calloc failed");
        return 0;
    }

    if( ! ic_lex_data_init(lex_data, filename, source) ){
        puts("ic_lex_data_new: call to ic_lex_data_init failed");
        return 0;
    }

    return lex_data;
}

/* init an existing lex data
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_lex_data_init(struct ic_lex_data *lex_data, char *filename, char *source){
    if( ! lex_data ){
        puts("ic_lex_data_init: lex_data was null");
        return 0;
    }

    if( ! filename ){
        puts("ic_lex_data_init: filename was null");
        return 0;
    }

    if( ! source ){
        puts("ic_lex_data_init: source was null");
        return 0;
    }

    lex_data->filename = filename;
    lex_data->source = source;

    lex_data->token_list = ic_token_list_new();
    if( ! lex_data->token_list ){
        puts("ic_lex_data_init: call to ic_token_list_new failed");
        return 0;
    }

    lex_data->s_len = strlen(source);

    return 1;
}

