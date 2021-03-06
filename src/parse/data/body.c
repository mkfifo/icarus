#include <stdio.h>  /* puts, printf */
#include <stdlib.h> /* calloc, free */

#include "../../data/pvector.h"
#include "body.h"
#include "stmt.h"

/* allocate and initialise a new ic_body
 *
 * returns pointer to new ic_body on success
 * returns 0 on failure
 */
struct ic_body *ic_body_new(void) {
    struct ic_body *body = 0;

    /* alloc */
    body = calloc(1, sizeof(struct ic_body));
    if (!body) {
        puts("ic_body_new: calloc failed");
        return 0;
    }

    /* init */
    if (!ic_body_init(body)) {
        puts("ic_body_new: call to ic_body_init failed");
        free(body);
        return 0;
    }

    return body;
}

/* initialise an existing ic_body
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_body_init(struct ic_body *body) {
    if (!body) {
        puts("ic_body_init: body was null");
        return 0;
    };

    /* simply dispatch to pvector init */
    if (!ic_pvector_init(&(body->contents), 0)) {
        puts("ic_body_init: call to ic_pvector_init failed");
        return 0;
    }

    body->scope = 0;

    /* success */
    return 1;
}

/* destroy body
 *
 * will only free body if `free_body` is truthy
 *
 * this will NOT free the scope
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_body_destroy(struct ic_body *body, unsigned int free_body) {
    int i = 0;
    int len = 0;
    struct ic_stmt *stmt = 0;

    if (!body) {
        puts("ic_body_destroy: body was null");
        return 0;
    };

    len = ic_body_length(body);
    if (len == -1) {
        puts("ic_body_destroy: call to ic_body_length failed");
        return 0;
    }

    /* we cannot dispatch to pvector as it has no idea what it contains
     * instead we must iterate through it manually calling the appropriate
     * constructor - in this case statement
     */
    for (i = 0; i < len; ++i) {
        stmt = ic_body_get(body, i);
        if (!stmt) {
            puts("ic_body_destroy: call to ic_body_get failed");
            return 0;
        }

        /* dispatch to stmt destroy
         * set free_stmt
         */
        if (!ic_stmt_destroy(stmt, 1)) {
            puts("ic_body_destroy: call to ic_stmt_destroy failed");
            return 0;
        }
    }

    /* FIXME currently leaking scope */
    body->scope = 0;

    /* free if asked nicely */
    if (free_body) {
        free(body);
    }

    return 1;
}

/* perform a deep copy of a body
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_body *ic_body_deep_copy(struct ic_body *body) {
    struct ic_body *new_body = 0;

    if (!body) {
        puts("ic_body_deep_copy: body was null");
        return 0;
    }

    new_body = calloc(1, sizeof(struct ic_body));
    if (!new_body) {
        puts("ic_body_deep_copy: call to calloc failed");
        return 0;
    }

    if (!ic_body_deep_copy_embedded(body, new_body)) {
        puts("ic_body_deep_copy: call to ic_body_deep_copy_embedded failed");
        return 0;
    }

    return new_body;
}

/* perform a deep copy of a body embedded within an object
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_body_deep_copy_embedded(struct ic_body *from, struct ic_body *to) {
    unsigned int i = 0;
    unsigned int len = 0;
    struct ic_stmt *stmt = 0;
    struct ic_stmt *new_stmt = 0;

    if (!from) {
        puts("ic_body_deep_copy_embedded: from was null");
        return 0;
    }

    if (!to) {
        puts("ic_body_deep_copy_embedded: to was null");
        return 0;
    }

    to->scope = 0;
    if (from->scope) {
        puts("ic_body_deep_copy_embedded: from->scope was set, implementation error");
        return 0;
    }

    len = ic_body_length(from);
    for (i=0; i<len; ++i) {
        stmt = ic_body_get(from, i);
        if (!stmt) {
            puts("ic_body_deep_copy_embedded: call to ic_body_get failed");
            return 0;
        }

        new_stmt = ic_stmt_deep_copy(stmt);
        if (!new_stmt) {
            puts("ic_body_deep_copy_embedded: call to ic_stmt_deep_copy failed");
            return 0;
        }

        if (-1 == ic_body_append(to, new_stmt)) {
            puts("ic_body_deep_copy_embedded: call to ic_body_append failed");
            return 0;
        }
    }

    return 1;
}

/* returns item at offset i on success
 * returns 0 on failure
 */
struct ic_stmt *ic_body_get(struct ic_body *body, unsigned int i) {
    if (!body) {
        puts("ic_body_get: body was null");
        return 0;
    };

    /* let pvector do all the work */
    return ic_pvector_get(&(body->contents), i);
}

/* append data to body
 *
 * returns index of item on success
 * returns -1 on failure
 */
int ic_body_append(struct ic_body *body, struct ic_stmt *data) {
    if (!body) {
        puts("ic_body_append: body was null");
        return -1;
    };

    /* let pvector do all the work */
    return ic_pvector_append(&(body->contents), data);
}

/* returns length on success
 * returns 0 on failure
 */
unsigned int ic_body_length(struct ic_body *body) {
    if (!body) {
        puts("ic_body_length: body was null");
        return 0;
    };

    /* let pvector do all the work */
    return ic_pvector_length(&(body->contents));
}

/* print this body */
void ic_body_print(FILE *fd, struct ic_body *body, unsigned int *indent_level) {
    struct ic_stmt *stmt = 0;
    unsigned int i = 0;
    unsigned int len = 0;

    if (!body) {
        puts("ic_body_print: body was null");
        return;
    }
    if (!indent_level) {
        puts("ic_body_print: indent_level was null");
        return;
    }

    /* incr indent level before body */
    ++*indent_level;

    len = ic_pvector_length(&(body->contents));
    for (i = 0; i < len; ++i) {
        stmt = ic_pvector_get(&(body->contents), i);

        if (!stmt) {
            puts("ic_body_print: ic_pvector_get call failed");
            continue;
        }

        ic_stmt_print(fd, stmt, indent_level);
    }

    /* decr indent level after body */
    --*indent_level;
}
