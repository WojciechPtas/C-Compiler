; ModuleID = 'input.txt'
source_filename = "input.txt"

%0 = type { i32 }
%1 = type { i32 }
%2 = type { i32 }
%3 = type { i32 }

@0 = private unnamed_addr constant [4 x i8] c"bla\00", align 1

define i32 @f(i32 %x, i32 %y) {
f_entry:
  %p9 = alloca ptr, align 8
  %p = alloca ptr, align 8
  %s = alloca %0, align 8
  %y2 = alloca i32, align 4
  %x1 = alloca i32, align 4
  store i32 %x, ptr %x1, align 4
  store i32 %y, ptr %y2, align 4
  %0 = getelementptr inbounds %1, ptr %s, i32 0, i32 0
  %1 = load i32, ptr %x1, align 4
  store i32 %1, ptr %0, align 4
  %2 = load i32, ptr %0, align 4
  store ptr %s, ptr %p, align 8
  %3 = load ptr, ptr %p, align 8
  br label %if-header

if-header:                                        ; preds = %f_entry
  br i1 true, label %than-block, label %else-block

than-block:                                       ; preds = %if-header
  %4 = getelementptr inbounds %2, ptr %s, i32 0, i32 0
  %5 = load ptr, ptr %p, align 8
  %6 = getelementptr inbounds %3, ptr %5, i32 0, i32 0
  %7 = load i32, ptr %6, align 4
  %8 = load i32, ptr %y2, align 4
  %9 = sub i32 %7, %8
  %10 = mul i32 4, %9
  store i32 %10, ptr %4, align 4
  %11 = load i32, ptr %4, align 4
  %negated = sub i32 0, %11
  ret i32 %negated

dead-block:                                       ; No predecessors!
  br label %if-end8

else-block:                                       ; preds = %if-header
  br label %if-header3

if-header3:                                       ; preds = %else-block
  br i1 true, label %than-block4, label %else-block6

than-block4:                                      ; preds = %if-header3
  ret i32 1

dead-block5:                                      ; No predecessors!
  br label %if-end

else-block6:                                      ; preds = %if-header3
  ret i32 0

dead-block7:                                      ; No predecessors!
  br label %if-end

if-end:                                           ; preds = %dead-block7, %dead-block5
  br label %if-end8

if-end8:                                          ; preds = %if-end, %dead-block
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
  br i1 %8, label %LogicalAndEnd, label %LogicalAndLazyEval

whileBody:                                        ; preds = %LogicalAndEnd
  br label %foo

afterWhile:                                       ; preds = %foo, %LogicalAndEnd
  br label %if-end9

LogicalAndLazyEval:                               ; preds = %whileHeader
  %9 = load i32, ptr %b, align 4
  %10 = call i32 @f(i32 23, i32 4)
  %11 = icmp eq i32 %9, %10
  %12 = or i1 %8, %11
  br label %LogicalAndEnd

LogicalAndEnd:                                    ; preds = %LogicalAndLazyEval, %whileHeader
  %13 = phi i1 [ true, %whileHeader ], [ %12, %LogicalAndLazyEval ]
  br i1 %13, label %whileBody, label %afterWhile

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
  %14 = load ptr, ptr %c, align 8
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
