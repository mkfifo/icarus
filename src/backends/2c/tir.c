#include "tir.h"
#include "expr.h"

#include "../../analyse/data/kludge.h"
#include "../../parse/data/stmt.h"
#include "../../transform/data/tbody.h"

unsigned int ic_b2c_compile_stmt_ret(struct ic_kludge *input_kludge, struct ic_transform_ir_ret *ret, FILE *out);
unsigned int ic_b2c_compile_stmt_let(struct ic_kludge *input_kludge, struct ic_transform_ir_let *let, FILE *out);
unsigned int ic_b2c_compile_stmt_assign(struct ic_kludge *input_kludge, struct ic_transform_ir_assign *assign, FILE *out);
unsigned int ic_b2c_compile_stmt_begin(struct ic_kludge *input_kludge, struct ic_transform_ir_begin *begin, FILE *out);
unsigned int ic_b2c_compile_stmt_if(struct ic_kludge *input_kludge, struct ic_transform_ir_if *tif, FILE *out);
unsigned int ic_b2c_compile_stmt_match(struct ic_kludge *input_kludge, struct ic_transform_ir_match *match, FILE *out);
unsigned int ic_b2c_compile_stmt_fcall(struct ic_kludge *input_kludge, struct ic_transform_ir_expr_fcall *fcal, FILE *out);

/* compile a given body to specified file
 *
 * generate a c program
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_b2c_compile_body(struct ic_kludge *input_kludge, struct ic_transform_body *tbody, FILE *out) {
    /* current tir_stmt in tir_body */
    struct ic_transform_ir_stmt *tstmt = 0;
    /* index of current tir_stmt in tir_body */
    unsigned int i = 0;
    /* length of tir_body */
    unsigned int len = 0;

    if (!input_kludge) {
        puts("ic_b2c_compile_body: input_kludge was null");
        return 0;
    }

    if (!tbody) {
        puts("ic_b2c_compile_body: tbody was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_body: out was null");
        return 0;
    }

    len = ic_transform_body_length(tbody);

    for (i = 0; i < len; ++i) {
        tstmt = ic_transform_body_get(tbody, i);
        if (!tstmt) {
            puts("ic_b2c_compile_body: call to ic_transform_body_get failed");
            return 0;
        }

        if (!ic_b2c_compile_stmt(input_kludge, tstmt, out)) {
            puts("ic_b2c_compile_body: call to ic_b2c_compile_stmt failed");
            return 0;
        }
    }

    return 1;
}

/* compile a given stmt to specified file
 *
 * generate a c program
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_b2c_compile_stmt(struct ic_kludge *input_kludge, struct ic_transform_ir_stmt *tstmt, FILE *out) {
    unsigned int indent_level = 1;

    if (!input_kludge) {
        puts("ic_b2c_compile_stmt: input_kludge was null");
        return 0;
    }

    if (!tstmt) {
        puts("ic_b2c_compile_stmt: tstmt was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt: out was null");
        return 0;
    }

    /* dispatch to appropriate sub handler based on tstmt type */
    switch (tstmt->tag) {
        case ic_transform_ir_stmt_type_fcall:
            if (!ic_b2c_compile_stmt_fcall(input_kludge, tstmt->u.fcall, out)) {
                puts("ic_b2c_compile_stmt: call to ic_b2c_compile_stmt_fcall failed");
                return 0;
            }
            return 1;
            break;

        case ic_transform_ir_stmt_type_let:
            return ic_b2c_compile_stmt_let(input_kludge, &(tstmt->u.let), out);
            break;

        case ic_transform_ir_stmt_type_ret:
            return ic_b2c_compile_stmt_ret(input_kludge, &(tstmt->u.ret), out);
            break;

        case ic_transform_ir_stmt_type_assign:
            return ic_b2c_compile_stmt_assign(input_kludge, &(tstmt->u.assign), out);
            break;

        case ic_transform_ir_stmt_type_begin:
            return ic_b2c_compile_stmt_begin(input_kludge, &(tstmt->u.begin), out);
            break;

        case ic_transform_ir_stmt_type_if:
            return ic_b2c_compile_stmt_if(input_kludge, &(tstmt->u.sif), out);
            break;

        case ic_transform_ir_stmt_type_match:
            return ic_b2c_compile_stmt_match(input_kludge, &(tstmt->u.match), out);
            break;

        default:
            puts("ic_b2c_compile_stmt: stmt had impossible tag:");
            ic_transform_ir_stmt_print(stdout, tstmt, &indent_level);

            return 0;
            break;
    }

    puts("ic_b2c_compile_stmt: impossible...");
    return 0;
}

unsigned int ic_b2c_compile_stmt_ret(struct ic_kludge *input_kludge, struct ic_transform_ir_ret *ret, FILE *out) {
    struct ic_symbol *ret_sym = 0;
    char *ret_str = 0;

    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_ret: input_kludge was null");
        return 0;
    }

    if (!ret) {
        puts("ic_b2c_compile_stmt_ret: ret was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_ret: out was null");
        return 0;
    }

    ret_sym = ret->var;
    if (!ret_sym) {
        puts("ic_b2c_compile_stmt_ret: ret_sym was null");
        return 0;
    }

    ret_str = ic_symbol_contents(ret_sym);
    if (!ret_str) {
        puts("ic_b2c_compile_stmt_ret: call to ic_symbol_contents failed");
        return 0;
    }

    fprintf(out, "  return %s;\n", ret_str);

    return 1;
}

unsigned int ic_b2c_compile_stmt_let(struct ic_kludge *input_kludge, struct ic_transform_ir_let *let, FILE *out) {
    struct ic_decl_type *let_type = 0;
    char *let_name = 0;
    struct ic_symbol *let_sym = 0;
    char *let_str = 0;

    struct ic_transform_ir_expr *expr = 0;

    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_let: input_kludge was null");
        return 0;
    }

    if (!let) {
        puts("ic_b2c_compile_stmt_let: let was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_let: out was null");
        return 0;
    }

    let_type = let->type;

    let_sym = ic_decl_type_mangled_name(let_type);
    if (!let_sym) {
        puts("ic_b2c_compile_stmt_let: call to ic_decl_type_mangled_name failed");
        return 0;
    }

    let_str = ic_symbol_contents(let_sym);
    if (!let_str) {
        puts("ic_b2c_compile_stmt_let: call to ic_symbol_contents failed for let type");
        return 0;
    }

    let_name = ic_symbol_contents(let->name);
    if (!let_name) {
        puts("ic_b2c_compile_stmt_let: call to ic_symbol_contents failed for let name");
        return 0;
    }

    /* print "type name = " */
    fprintf(out, "  %s %s = ", let_str, let_name);

    /* expression */
    expr = let->expr;

    if (!ic_b2c_compile_expr(input_kludge, expr, out)) {
        puts("ic_b2c_compile_stmt_let: call to ic_b2c_compile_expr failed");
        return 0;
    }

    /* closing semicolon and trailing \n */
    fputs(";\n", out);

    return 1;
}

unsigned int ic_b2c_compile_stmt_assign(struct ic_kludge *input_kludge, struct ic_transform_ir_assign *assign, FILE *out) {
    char *left_str = 0;
    struct ic_symbol *left_sym = 0;
    struct ic_transform_ir_expr *expr = 0;

    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_assign: input_kludge was null");
        return 0;
    }

    if (!assign) {
        puts("ic_b2c_compile_stmt_assign: assign was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_assign: out was null");
        return 0;
    }

    left_sym = assign->left;
    if (!left_sym) {
        puts("ic_b2c_compile_stmt_assign: assign->left was falsey");
        return 0;
    }

    left_str = ic_symbol_contents(left_sym);
    if (!left_str) {
        puts("ic_b2c_compile_stmt_assign: call to ic_symbol_contents failed for let type");
        return 0;
    }

    /* print "name = " */
    fprintf(out, "  %s = ", left_str);

    /* expression */
    expr = assign->right;

    if (!ic_b2c_compile_expr(input_kludge, expr, out)) {
        puts("ic_b2c_compile_stmt_assign: call to ic_b2c_compile_expr failed");
        return 0;
    }

    /* closing semicolon and trailing \n */
    fputs(";\n", out);

    return 1;
}

unsigned int ic_b2c_compile_stmt_begin(struct ic_kludge *input_kludge, struct ic_transform_ir_begin *begin, FILE *out) {
    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_begin: input_kludge was null");
        return 0;
    }

    if (!begin) {
        puts("ic_b2c_compile_stmt_begin: begin was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_begin: out was null");
        return 0;
    }

    fputs("  {\n", out);

    /* body */
    if (!ic_b2c_compile_body(input_kludge, begin->tbody, out)) {
        puts("ic_b2c_compile_stmt_begin: call to ic_b2c_compile_body failed for tbody");
        return 0;
    }

    fputs("  }\n", out);

    return 1;
}

unsigned int ic_b2c_compile_stmt_if(struct ic_kludge *input_kludge, struct ic_transform_ir_if *tif, FILE *out) {
    struct ic_symbol *cond = 0;
    char *cond_ch = 0;

    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_if: input_kludge was null");
        return 0;
    }

    if (!tif) {
        puts("ic_b2c_compile_stmt_if: tif was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_if: out was null");
        return 0;
    }

    /* transform guarantees us that the cond will always be a name of an
     * in-scope boolean literal which we can just test
     */

    cond = tif->cond;
    if (!cond) {
        puts("ic_b2c_compile_stmt_if: cond on tif was null");
        return 0;
    }

    cond_ch = ic_symbol_contents(cond);
    if (!cond_ch) {
        puts("ic_b2c_compile_stmt_if: call to ic_symbol_contents failed");
        return 0;
    }

    fprintf(out, "  if (%s) {\n", cond_ch);

    if (!tif->then_tbody) {
        puts("ic_b2c_compile_stmt_if: then_tbody on tif was null");
        return 0;
    }
    /* if body */
    if (!ic_b2c_compile_body(input_kludge, tif->then_tbody, out)) {
        puts("ic_b2c_compile_stmt_if: call to ic_b2c_compile_body failed for then_tbody");
        return 0;
    }

    /* optional else */
    if (tif->else_tbody) {
        fputs("  } else {\n", out);
        /* else body */
        if (!ic_b2c_compile_body(input_kludge, tif->else_tbody, out)) {
            puts("ic_b2c_compile_stmt_if: call to ic_b2c_compile_body failed for else_tbody");
            return 0;
        }
    }

    fputs("  }\n", out);

    return 1;
}

unsigned int ic_b2c_compile_stmt_match(struct ic_kludge *input_kludge, struct ic_transform_ir_match *match, FILE *out) {
    char *sym_char = 0;
    unsigned int len = 0;
    unsigned int i = 0;
    struct ic_transform_ir_match_case *tcase = 0;
    struct ic_symbol *type_mangled_name = 0;
    char *type_mangled_name_ch = 0;
    char *field_name_ch = 0;
    struct ic_symbol *field_mangled_type_sym = 0;
    char *field_mangled_type_ch = 0;

    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_match: input_kludge was null");
        return 0;
    }

    if (!match) {
        puts("ic_b2c_compile_stmt_match: match was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_match: out was null");
        return 0;
    }

    /* match f
     *  case a::Signed
     *      ...
     *  case b::String
     *      ...
     * end
     *
     * ->
     *
     * switch (f->_tag) {
     *   case Foo_tag_Signed_a:
     *      ...
     *   case Foo_tag_String_b:
     *      ...
     *   default:
     *    panic("...")
     * }
     */

    type_mangled_name = ic_decl_type_mangled_name(match->tdecl);
    if (!type_mangled_name) {
        puts("ic_b2c_compile_stmt_match: call to ic_decl_type_mangled_name failed");
        return 0;
    }

    type_mangled_name_ch = ic_symbol_contents(type_mangled_name);
    if (!type_mangled_name_ch) {
        puts("ic_b2c_compile_stmt_match: call to ic_symbol_contents failed");
        return 0;
    }

    sym_char = ic_symbol_contents(match->match_symbol);

    fprintf(out, "  switch (%s->_tag) {\n", sym_char);

    /* go through cases */
    len = ic_pvector_length(&(match->cases));
    for (i = 0; i < len; ++i) {
        tcase = ic_pvector_get(&(match->cases), i);
        if (!tcase) {
            puts("ic_b2c_compile_stmt_match: call to ic_pvector_get failed");
            return 0;
        }

        field_name_ch = ic_symbol_contents(&(tcase->field->name));

        field_mangled_type_sym = ic_type_ref_mangled_name(tcase->field->type);
        if (!field_mangled_type_sym) {
            puts("ic_b2c_compile_stmt_match: call to ic_type_ref_mangled_name failed");
            return 0;
        }

        field_mangled_type_ch = ic_symbol_contents(field_mangled_type_sym);

        /* insert case */
        fprintf(out, "    case %s_tag_%s_%s:\n", type_mangled_name_ch, field_mangled_type_ch, field_name_ch);
        fputs("      {\n", out);

        /* insert var to unpack */
        fprintf(out, "      %s %s = %s->u.%s;\n", field_mangled_type_ch, field_name_ch, sym_char, field_name_ch);

        if (!ic_b2c_compile_body(input_kludge, tcase->tbody, out)) {
            puts("ic_b2c_compile_stmt_match: call to ic_b2c_compile_body failed for case body");
            return 0;
        }
        fputs("      }\n", out);
        fputs("      break;\n", out);
    }

    fputs("    default:\n", out);
    if (match->else_body) {
        fputs("      {\n", out);
        /* if else_body provided, use that for default */
        /* else body */
        if (!ic_b2c_compile_body(input_kludge, match->else_body, out)) {
            puts("ic_b2c_compile_stmt_match: call to ic_b2c_compile_body failed for else_body");
            return 0;
        }
        fputs("      }\n", out);
    } else {
        /* otherwise insert our own default */
        fprintf(out, "    panic(\"unknown case for _tag on type '%s\");\n", type_mangled_name_ch);
    }
    fputs("      break;\n", out);

    fputs("  }\n", out);

    return 1;
}

unsigned int ic_b2c_compile_stmt_fcall(struct ic_kludge *input_kludge, struct ic_transform_ir_expr_fcall *fcall, FILE *out) {
    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_fcall: input_kludge was null");
        return 0;
    }

    if (!fcall) {
        puts("ic_b2c_compile_stmt_fcall: fcall was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_fcall: out was null");
        return 0;
    }

    fputs("  ", out);

    if (!ic_b2c_compile_expr_fcall(input_kludge, fcall, out)) {
        puts("ic_b2c_compile_stmt_fcall: call to ic_b2c_compile_expr failed");
        return 0;
    }

    /* must close off expr */
    fputs(";\n", out);

    return 1;
}
