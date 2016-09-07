#include <stdio.h>  /* puts, printf */
#include <stdlib.h> /* exit, free */
#include <string.h>

#include "../../src/backends/pancake/data/instructions.h"
#include "../../src/backends/pancake/data/runtime_data.h"
#include "../../src/backends/pancake/pancake.h"
#include "helpers.h"

int main(int argc, char **argv) {
    char *in_filename = 0;
    struct ic_kludge *kludge = 0;
    struct ic_backend_pancake_instructions *instructions = 0;
    struct ic_backend_pancake_runtime_data *runtime_data = 0;

    if (argc < 2) {
        puts("source not supplied");
        exit(1);
    } else if (argc > 2) {
        puts("Too many arguments supplied, only source file expected expected");
        exit(1);
    }

    in_filename = argv[1];

    kludge = test_custom_helper_gen_kludge();
    test_custom_helper_load_core(kludge);
    test_custom_helper_load_filename(kludge, in_filename);

    if (!ic_analyse(kludge)) {
        puts("main: call to ic_analyse failed");
        exit(1);
    }

    if (!ic_transform(kludge)) {
        puts("main: call to ic_transform failed");
        exit(1);
    }

    instructions = ic_backend_pancake_compile(kludge);
    if (!instructions) {
        puts("main: call to ic_backend_pancake_compile failed");
        exit(1);
    }

    runtime_data = ic_backend_pancake_runtime_data_new(instructions);
    if (!runtime_data) {
        puts("main: call to ic_backend_pancake_runtime_data_new failed");
        return 0;
    }

    if (!ic_backend_pancake_interpret(runtime_data)) {
        puts("main: call to ic_backend_pancake_interpret failed");
        return 0;
    }

    /* clean up time
     * this will destroy both the kludge and the ast (via aast)
     */
    if (!ic_kludge_destroy(kludge, 1)) {
        puts("main: ic_kludge_destroy call failed");
    }

    return 0;
}