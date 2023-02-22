; ModuleID = 'input.txt'
source_filename = "input.txt"

%S = type { i32 }

@x = common global %S zeroinitializer

define i32 @main() {
main_entry:
  %a = alloca %S, align 8
  %0 = getelementptr inbounds %S, ptr %a, i32 0, i32 0
  store i32 2, ptr %0, align 4
  %1 = load i32, ptr %0, align 4
  ret i32 0
}
