; ModuleID = 'input.txt'
source_filename = "input.txt"

define i32 @main() {
main_entry:
  ret i32 1

dead-block:                                       ; No predecessors!
  ret i32 0
}
