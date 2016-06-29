# Icarus [![Build Status](https://travis-ci.org/mkfifo/icarus.svg?branch=master)](https://travis-ci.org/mkfifo/icarus) [![Coverage Status](https://coveralls.io/repos/mkfifo/icarus/badge.svg?branch=master)](https://coveralls.io/r/mkfifo/icarus?branch=master) <a href="https://scan.coverity.com/projects/4854"> <img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/4854/badge.svg"/> </a>

Icarus is a toy language exploring static verifiability

I am currently exploring a few potential options for how Icarus' "permissions" (ownership, aliasing,mutation) system will work.

The current proposal can be seen in [docs/language/mutability/proposal_five/permissions.md](https://github.com/mkfifo/icarus/blob/master/docs/language/mutability/proposal_five/permissions.md)

Goals
=====

* Explicit, obvious and statically verifiable mutability
* Strong statically verified type system
* Runtime memory safety (no pointer arithmetic, arrays are bounds checked, all memory is intialised)

Work so far
===========

Icarus is aiming to support a functional subset as a proof of concept,
the initial target for this can be seen in `example/simple.ic` (reproduced below).

For this target the following systems are currently 'complete':

* lexer
* parser (syntactic analysis)
* analyse (semantic analysis)


We have the contents of `example/simple.ic`:

    example/simple.ic contents:
    ----------------
    # user defined type with 2 fields, an Int and a String
    type Foo
        a::Int
        b::String
    end

    fn d(i::Int)
        print(i)
    end

    fn d(s::String)
        print(s)
    end

    # break apart a Foo and call d on each field
    fn d(f::Foo)
        d(f.a)
        d(f.b)
    end

    # simple function to test return values
    fn add_one(i::Int) -> Int
        let tmp = i
        tmp = i + 1
        return tmp
    end

    fn maybe_add_one(i::Int, b::Bool) -> Int
        # FIXME this doesn't work `if i == 2 and b`
        # due to lack of operator precedence
        if b and i == 2
            i = i + 1
        end

        return i
    end

    # entry point for program
    fn main()
        let f::Foo = Foo(add_one(1) "hello")

        f.a = maybe_add_one(f.a, True)

        d(f)
    end

    # hack to work around lack of constructors
    builtin fn Foo(a::Int, b::String) -> Foo

    ----------------



We can see what Icarus makes of this by running:

    make example

Hidden in the output we see the new lexer output:

    lexer output:
    ----------------
    # user defined type with 2 fields, an Int and a String
    type Foo
        a::Int
        b::String
    end

    fn d(i::Int)
        print(i)
    end

    fn d(s::String)
        print(s)
    end

    # break apart a Foo and call d on each field
    fn d(f::Foo)
        d(f.a)
        d(f.b)
    end

    # simple function to test return values
    fn add_one(i::Int) -> Int
        let tmp = i
        tmp = i + 1
        return tmp
    end

    fn maybe_add_one(i::Int, b::Bool) -> Int
        # FIXME this doesn't work `if i == 2 and b`
        # due to lack of operator precedence
        if b andi == 2
            i = i + 1
        end

        return i
    end

    # entry point for program
    fn main()
        let f::Foo = Foo(add_one(1) "hello")

        f.a = maybe_add_one(f.a, True)

        d(f)
    end

    # hack to work around lack of constructors
    builtin fn Foo(a::Int, b::String) -> Foo

    ----------------


Hidden elsewhere in the output we can see the parser reconstructing the program from it's current understanding (based on new lexer):

    parser output:
    ----------------
    type Foo
        a::Int
        b::String
    end

    # d(Int)
    fn d(i::Int) -> Void
        print(i)
    end

    # d(String)
    fn d(s::String) -> Void
        print(s)
    end

    # d(Foo)
    fn d(f::Foo) -> Void
        d(f.a)
        d(f.b)
    end

    # add_one(Int)
    fn add_one(i::Int) -> Int
        let tmp = i
        tmp = i + 1
        return tmp
    end

    # maybe_add_one(Int Bool)
    fn maybe_add_one(i::Int, b::Bool) -> Int
        if b and i == 2
            i = i + 1
        end
        return i
    end

    # main()
    fn main() -> Void
        let f::Foo = Foo(add_one(1), "hello")
        f.a = maybe_add_one(f.a, True)
        d(f)
    end

    builtin fn Foo(a::Int, b::String) -> Foo
    ----------------

Finally the analyse step's outputs showing no errors, but making it clear that it isn't complete yet

    analyse output:
    ----------------
    warning: main implementation pending, icarus is currently only partially functional
    analysis complete
    ----------------


Compiler
========

The compiler is still a WIP, currently work on icarus is focussed on adding an IR stage,
the start of an IR-less compiler can be seen below

if we input the following file (milestone/01.ic):

    fn main()
      println("hello world")
    end

we can then compile this via:

    ./icarus milestone/01.ic out.c

which will output (to out.c):

    #include "backends/2c/builtins.c"
    /* main() -> Void */
    Void i_main_a();
    /* main() -> Void */
    Void i_main_a(){
    i_println_a_String(ic_string_new("hello world", 11));
    }
    #include "backends/2c/entry.c"

if we compile and run this, we can see:

    gcc out.c
    ./a.out

the output:

    hello world

Current focus
=============

The `transform` phase - which outputs the new Icarus Transform IR

