; ModuleID = 'input.txt'
source_filename = "input.txt"

%C = type { i32, i32 }
%C.0 = type { i32, i32 }
%S = type { i32, i8, ptr }

@c = common global %C zeroinitializer
@a = common global %C.0 zeroinitializer
@s = common global %S zeroinitializer

define i32 @main() {
main_entry:
  ret i32 0
}
