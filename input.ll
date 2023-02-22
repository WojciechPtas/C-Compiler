; ModuleID = 'input.txt'
source_filename = "input.txt"

%A.0 = type { i32 }
%A = type { i32 }

define %A.0 @main() {
main_entry:
  %g = alloca %A, align 8
  ret %A.0 zeroinitializer
}
