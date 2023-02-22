; ModuleID = 'input.txt'
source_filename = "input.txt"

%S = type { i32 }

@p = common global ptr null
@0 = private unnamed_addr constant [4 x i8] c"bla\00", align 1

define i32 @f(i32 %x, i32 %y) {
f_entry:
  %p = alloca ptr, align 8
  %s = alloca %S, align 8
  %y2 = alloca i32, align 4
  %x1 = alloca i32, align 4
  store i32 %x, ptr %x1, align 4
  store i32 %y, ptr %y2, align 4
  %0 = getelementptr inbounds %S, ptr %s, i32 0, i32 0
  %1 = load i32, ptr %x1, align 4
  store i32 %1, ptr %0, align 4
  %2 = load i32, ptr %0, align 4
  store ptr %s, ptr %p, align 8
  %3 = load ptr, ptr %p, align 8
  br label %if-header

if-header:                                        ; preds = %f_entry
  br i1 true, label %than-block, label %else-block

than-block:                                       ; preds = %if-header
  %4 = getelementptr inbounds %S, ptr %s, i32 0, i32 0
  %5 = load ptr, ptr %p, align 8
  %6 = getelementptr inbounds %S, ptr %5, i32 0, i32 0
  %7 = load i32, ptr %6, align 4
  %8 = load i32, ptr %y2, align 4
  %9 = sub i32 %7, %8
  %10 = mul i32 4, %9
  store i32 %10, ptr %4, align 4
  %11 = load i32, ptr %4, align 4
  %negated = sub i32 0, %11
  ret i32 %negated

DEAD-BLOCK:                                       ; No predecessors!
  %12 = getelementptr inbounds %S, ptr %s, i32 0, i32 0
  %13 = load i32, ptr %12, align 4
  unreachable

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
  %ICmpSLT = icmp slt i32 %4, %5
  br i1 %ICmpSLT, label %than-block, label %else-block

than-block:                                       ; preds = %if-header
  br label %whileHeader

whileHeader:                                      ; preds = %dead-block, %than-block
  %6 = load i32, ptr %a, align 4
  %ICmpSLT1 = icmp slt i32 %6, 4
  br i1 %ICmpSLT1, label %"Logical OrEnd", label %"Logical OrLazyEval"

whileBody:                                        ; preds = %"Logical OrEnd"
  br label %foo

afterWhile:                                       ; preds = %foo, %"Logical OrEnd"
  br label %if-end10

DEAD-BLOCK:                                       ; No predecessors!
  %7 = load i32, ptr %b, align 4
  unreachable

"Logical OrLazyEval":                             ; preds = %whileHeader
  %8 = load i32, ptr %b, align 4
  %9 = call i32 @f(i32 23, i32 4)
  %ICmpEQ = icmp eq i32 %8, %9
  %LogicalNot = xor i1 %ICmpEQ, true
  %10 = or i1 %ICmpSLT1, %LogicalNot
  br label %"Logical OrEnd"

"Logical OrEnd":                                  ; preds = %"Logical OrLazyEval", %whileHeader
  %11 = phi i1 [ true, %whileHeader ], [ %10, %"Logical OrLazyEval" ]
  br i1 %11, label %whileBody, label %afterWhile

foo:                                              ; preds = %whileBody
  br label %afterWhile

dead-block:                                       ; No predecessors!
  br label %whileHeader

else-block:                                       ; preds = %if-header
  br label %if-header2

if-header2:                                       ; preds = %else-block
  br i1 false, label %than-block3, label %else-block8

than-block3:                                      ; preds = %if-header2
  br label %whileHeader4

whileHeader4:                                     ; preds = %dead-block7, %whileBody5, %than-block3
  br i1 true, label %whileBody5, label %afterWhile6

whileBody5:                                       ; preds = %whileHeader4
  store ptr @0, ptr %c, align 8
  %12 = load ptr, ptr %c, align 8
  br label %whileHeader4

afterWhile6:                                      ; preds = %whileHeader4
  br label %if-end

dead-block7:                                      ; No predecessors!
  br label %whileHeader4

else-block8:                                      ; preds = %if-header2
  br label %end

dead-block9:                                      ; No predecessors!
  br label %if-end

if-end:                                           ; preds = %dead-block9, %afterWhile6
  br label %if-end10

if-end10:                                         ; preds = %if-end, %afterWhile
  br label %bar

bar:                                              ; preds = %if-end10
  br label %end

end:                                              ; preds = %bar, %else-block8
  ret i32 0

dead-block11:                                     ; No predecessors!
  ret i32 0
}
