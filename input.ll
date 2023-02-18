; ModuleID = 'input.txt'
source_filename = "input.txt"

@0 = private unnamed_addr constant [4 x i8] c"bla\00", align 1

define i32 @f(i32 %x, i32 %y) {
f_entry:
  %p = alloca ptr, align 8
  %y2 = alloca i32, align 4
  %x1 = alloca i32, align 4
  store i32 %x, ptr %x1, align 4
  store i32 %y, ptr %y2, align 4
  br label %if-header

if-header:                                        ; preds = %f_entry
  br i1 true, label %than-block, label %else-block

than-block:                                       ; preds = %if-header
  ret i32 1

dead-block:                                       ; No predecessors!
  br label %if-end

else-block:                                       ; preds = %if-header
  ret i32 0

dead-block3:                                      ; No predecessors!
  br label %if-end

if-end:                                           ; preds = %dead-block3, %dead-block
  ret i32 0
}

define i32 @main() {
main_entry:
  %c = alloca ptr, align 8
  %b = alloca i32, align 4
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %1 = load i32, ptr %a, align 4
  %2 = mul i32 %1, 2
  store i32 %2, ptr %b, align 4
  %3 = load i32, ptr %b, align 4
  br label %if-header

if-header:                                        ; preds = %main_entry
  %4 = load i32, ptr %a, align 4
  %5 = load i32, ptr %b, align 4
  %6 = icmp slt i32 %4, %5
  br i1 %6, label %than-block, label %else-block

than-block:                                       ; preds = %if-header
  br label %whileHeader

whileHeader:                                      ; preds = %dead-block, %than-block
  %7 = load i32, ptr %a, align 4
  %8 = icmp slt i32 %7, 4
  br i1 %8, label %whileBody, label %afterWhile

whileBody:                                        ; preds = %whileHeader
  br label %foo

afterWhile:                                       ; preds = %foo, %whileHeader
  br label %if-end9

DEAD-BLOCK:                                       ; No predecessors!
  %9 = load i32, ptr %b, align 4

foo:                                              ; preds = %whileBody
  br label %afterWhile

dead-block:                                       ; No predecessors!
  br label %whileHeader

else-block:                                       ; preds = %if-header
  br label %if-header1

if-header1:                                       ; preds = %else-block
  br i1 false, label %than-block2, label %else-block7

than-block2:                                      ; preds = %if-header1
  br label %whileHeader3

whileHeader3:                                     ; preds = %dead-block6, %whileBody4, %than-block2
  br i1 true, label %whileBody4, label %afterWhile5

whileBody4:                                       ; preds = %whileHeader3
  store ptr @0, ptr %c, align 8
  %10 = load ptr, ptr %c, align 8
  br label %whileHeader3

afterWhile5:                                      ; preds = %whileHeader3
  br label %if-end

dead-block6:                                      ; No predecessors!
  br label %whileHeader3

else-block7:                                      ; preds = %if-header1
  br label %end

dead-block8:                                      ; No predecessors!
  br label %if-end

if-end:                                           ; preds = %dead-block8, %afterWhile5
  br label %if-end9

if-end9:                                          ; preds = %if-end, %afterWhile
  br label %bar

bar:                                              ; preds = %if-end9
  br label %end

end:                                              ; preds = %bar, %else-block7
  ret i32 0

dead-block10:                                     ; No predecessors!
  ret i32 0
}
