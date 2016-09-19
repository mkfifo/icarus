#!/usr/bin/env perl
use strict;
use warnings;
use v5.10;
use IPC::Open3;

my $path = "./icarus";

die "Could not find '$path'\n" unless -e $path;

my $cases = [
  {
    input => '
      fn main()
        println("Hello world")
      end
      ',
    expected => '
      lexer output:
      ----------------

      fn main()
        println("Hello world")
      end
      ----------------


      parser output:
      ----------------
      # main()
      fn main() -> Void
          println("Hello world")
      end
      ----------------


      analyse output:
      ----------------
      warning: main implementation pending, icarus is currently only partially functional
      analysis complete
      ----------------


      transform output (PENDING):
      ----------------
      ----------------
      fn main() -> Void
          let _l1::String = "Hello world"
          println(_l1)
      end

      backend pancake selected (PENDING):
      Pancake bytecode:
      ==========================
      label entry
      call main() 0
      exit
      label main()
      pushstr "Hello world"
      call_builtin println(String) 1
      clean_stack
      return_void
      ==========================

      Pancake interpreter output
      ==========================
      Hello world
      '
  },

  {
    input => '
      fn bar(a::Sint) -> Sint
        return 7 + a
      end

      fn foo(a::Sint) -> Sint
        return bar(a + 4)
      end

      fn baz(a::Sint) -> Sint
        let b = a + 3
        return bar(b)
      end

      fn main()
        println(foo(7))
        println(baz(7))
      end
      ',
    expected => '
      lexer output:
      ----------------

      fn bar(a::Sint) -> Sint
        return 7 + a
      end

      fn foo(a::Sint) -> Sint
        return bar(a + 4)
      end

      fn baz(a::Sint) -> Sint
        let b = a + 3
        return bar(b)
      end

      fn main()
        println(foo(7))
        println(baz(7))
      end
      ----------------


      parser output:
      ----------------
      # bar(Sint)
      fn bar(a::Sint) -> Sint
          return 7 + a
      end

      # foo(Sint)
      fn foo(a::Sint) -> Sint
          return bar(a + 4)
      end

      # baz(Sint)
      fn baz(a::Sint) -> Sint
          let b = a + 3
          return bar(b)
      end

      # main()
      fn main() -> Void
          println(foo(7))
          println(baz(7))
      end
      ----------------


      analyse output:
      ----------------
      warning: main implementation pending, icarus is currently only partially functional
      analysis complete
      ----------------


      transform output (PENDING):
      ----------------
      ----------------
      fn bar(a::Sint) -> Sint
          let _l1::Sint = 7
          let _t1::Sint = plus(_l1, a)
          return _t1
      end
      fn foo(a::Sint) -> Sint
          let _l1::Sint = 4
          let _t2::Sint = plus(a, _l1)
          let _t1::Sint = bar(_t2)
          return _t1
      end
      fn baz(a::Sint) -> Sint
          let _l1::Sint = 3
          let b::Sint = plus(a, _l1)
          let _t1::Sint = bar(b)
          return _t1
      end
      fn main() -> Void
          let _l1::Sint = 7
          let _t1::Sint = foo(_l1)
          println(_t1)
          let _l2::Sint = 7
          let _t2::Sint = baz(_l2)
          println(_t2)
      end

      backend pancake selected (PENDING):
      Pancake bytecode:
      ==========================
      label entry
      call main() 0
      exit
      label bar(Sint)
      pushint 7
      copyarg 0
      call_builtin plus(Sint,Sint) 2
      store _t1
      load _t1
      save
      clean_stack
      restore
      return_value
      clean_stack
      return_void
      label foo(Sint)
      copyarg 0
      pushint 4
      call_builtin plus(Sint,Sint) 2
      store _t2
      load _t2
      call bar(Sint) 1
      store _t1
      load _t1
      save
      clean_stack
      restore
      return_value
      clean_stack
      return_void
      label baz(Sint)
      copyarg 0
      pushint 3
      call_builtin plus(Sint,Sint) 2
      store b
      load b
      call bar(Sint) 1
      store _t1
      load _t1
      save
      clean_stack
      restore
      return_value
      clean_stack
      return_void
      label main()
      pushint 7
      call foo(Sint) 1
      store _t1
      load _t1
      call_builtin println(Sint) 1
      pushint 7
      call baz(Sint) 1
      store _t2
      load _t2
      call_builtin println(Sint) 1
      clean_stack
      return_void
      ==========================

      Pancake interpreter output
      ==========================
      18
      17
      ',
  },
];

# whitespace sensitivity sucks
sub cleanup {
  my $str = shift // die;
  die if @_;

  # strip up to 6 leading (horizontal whitespace) spaces
  $str =~ s/^\h{0,6}//mg;

  return $str;
}

sub run {
  my $input = shift // die;
  my $expected = shift // die;
  die if @_;

  my $in_tmp_file = `mktemp TESTING_BACKEND_PANCAKE_END_TO_END_XXX.ic`;
  chomp($in_tmp_file);
  open( my $fh, ">", "$in_tmp_file" ) or die "failed to open tmp file '$in_tmp_file'";
  print $fh $input;
  close $fh;

  my $output = `$path $in_tmp_file pancake`;
  my $status = $?;

  `rm $in_tmp_file`;

  if( $status != 0 ){
      say "Command return non-zero exit code '${status}";
      die "Command return non-zero exit code '${status}";
  }

  if( $output ne $expected ){
      say "Output was not as expected";
      say "=======\nExpected:\n$expected";
      say "=======\nGot:\n$output";
      say "=======\n";
      die "Output not as expected";
  }

  say "=======\nGot correct output:\n$output\n=======";
}

for my $case (@$cases) {
  my $input = cleanup $case->{input};
  my $expected = cleanup $case->{expected};
  run $input, $expected;
}

say "test_backend_pancake_end_to_end successs";
