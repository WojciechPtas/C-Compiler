; ModuleID = 'input.txt'
source_filename = "input.txt"

@p = common global ptr null

declare i32 @a(i32, i32)

define i32 @main(i32 %a, i32 %b) {
main_entry:
  %b2 = alloca i32, align 4
  %a1 = alloca i32, align 4
  store i32 %a, ptr %a1, align 4
  store i32 %b, ptr %b2, align 4
  ret i32 0
}
