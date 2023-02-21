; ModuleID = 'input.txt'
source_filename = "input.txt"

@i1 = common global i32 0

define void @labeledLabeled() {
labeledLabeled_entry:
  %v = alloca i32, align 4
  br label %stupid

dead-block:                                       ; No predecessors!
  store i32 2, ptr %v, align 4
  %0 = load i32, ptr %v, align 4
  br label %crazy

crazy:                                            ; preds = %dead-block
  br label %stupid

stupid:                                           ; preds = %crazy, %labeledLabeled_entry
  br label %alarming

alarming:                                         ; preds = %stupid
  %1 = load i32, ptr %v, align 4
  %2 = icmp eq i32 %1, 0
  %3 = sext i1 %2 to i32
  store i32 %3, ptr %v, align 4
  %4 = load i32, ptr %v, align 4
  br label %happens

dead-block1:                                      ; No predecessors!
  br label %happens

happens:                                          ; preds = %dead-block1, %alarming
  store i32 7, ptr %v, align 4
  %5 = load i32, ptr %v, align 4
  br label %yesweare

dead-block2:                                      ; No predecessors!
  br label %if-header

if-header:                                        ; preds = %dead-block2
  %6 = load i32, ptr %v, align 4
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %than-block, label %else-block

than-block:                                       ; preds = %if-header
  br label %yesweare

yesweare:                                         ; preds = %than-block, %happens
  store i32 0, ptr %v, align 4
  %8 = load i32, ptr %v, align 4
  br label %if-end

else-block:                                       ; preds = %if-header
  store i32 6, ptr %v, align 4
  %9 = load i32, ptr %v, align 4
  br label %if-end

if-end:                                           ; preds = %else-block, %yesweare
  br label %whileHeader

whileHeader:                                      ; preds = %afterWhile5, %if-end
  %10 = load i32, ptr %v, align 4
  %11 = add i32 %10, 1
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %whileBody, label %afterWhile

whileBody:                                        ; preds = %whileHeader
  br label %whileHeader3

afterWhile:                                       ; preds = %whileHeader
  br label %plsend

whileHeader3:                                     ; preds = %dead-block6, %whileBody
  br i1 true, label %whileBody4, label %afterWhile5

whileBody4:                                       ; preds = %whileHeader3
  br label %plsend

afterWhile5:                                      ; preds = %whileHeader3
  br label %whileHeader

dead-block6:                                      ; No predecessors!
  br label %whileHeader3

plsend:                                           ; preds = %afterWhile, %whileBody4
  ret void
}

define i32 @reallyBlockyInsideInit(i32 %k) {
reallyBlockyInsideInit_entry:
  %k3 = alloca i32, align 4
  %z = alloca i32, align 4
  %k2 = alloca i32, align 4
  %k1 = alloca i32, align 4
  store i32 %k, ptr %k1, align 4
  %0 = load i32, ptr %z, align 4
  store i32 %0, ptr %k3, align 4
  %1 = load i32, ptr %k3, align 4
  ret i32 0
}

define void @takingAWhile(ptr %s) {
takingAWhile_entry:
  %s1 = alloca ptr, align 8
  store ptr %s, ptr %s1, align 8
  br label %whileHeader

whileHeader:                                      ; preds = %whileBody, %takingAWhile_entry
  %0 = load ptr, ptr %s1, align 8
  %1 = load i8, ptr %0, align 1
  %2 = icmp ne i8 %1, 104
  br i1 %2, label %whileBody, label %afterWhile

whileBody:                                        ; preds = %whileHeader
  %3 = load ptr, ptr %s1, align 8
  %4 = load ptr, ptr %s1, align 8
  %5 = load i8, ptr %4, align 1
  %6 = sext i8 %5 to i32
  %7 = add i32 %6, 1
  %8 = trunc i32 %7 to i8
  store i8 %8, ptr %3, align 1
  %9 = load i8, ptr %3, align 1
  br label %whileHeader

afterWhile:                                       ; preds = %whileHeader
  br label %whileHeader2

whileHeader2:                                     ; preds = %whileBody3, %afterWhile
  %10 = load ptr, ptr %s1, align 8
  %11 = load i8, ptr %10, align 1
  %12 = icmp ne i8 %11, 104
  br i1 %12, label %whileBody3, label %afterWhile4

whileBody3:                                       ; preds = %whileHeader2
  br label %whileHeader2

afterWhile4:                                      ; preds = %whileHeader2
  br label %whileHeader5

whileHeader5:                                     ; preds = %dead-block9, %than-block, %afterWhile4
  %13 = load ptr, ptr %s1, align 8
  store i8 1, ptr %13, align 1
  %14 = load i8, ptr %13, align 1
  %15 = icmp ne i8 %14, 0
  br i1 %15, label %whileBody6, label %afterWhile7

whileBody6:                                       ; preds = %whileHeader5
  br label %if-header

afterWhile7:                                      ; preds = %else-block, %whileHeader5
  ret void

if-header:                                        ; preds = %whileBody6
  %16 = load ptr, ptr %s1, align 8
  store i8 0, ptr %16, align 1
  %17 = load i8, ptr %16, align 1
  %18 = icmp ne i8 %17, 0
  br i1 %18, label %than-block, label %else-block

than-block:                                       ; preds = %if-header
  br label %whileHeader5

dead-block:                                       ; No predecessors!
  br label %if-end

else-block:                                       ; preds = %if-header
  br label %afterWhile7

dead-block8:                                      ; No predecessors!
  br label %if-end

if-end:                                           ; preds = %dead-block8, %dead-block
  ret void

dead-block9:                                      ; No predecessors!
  br label %whileHeader5
}

define void @functionPointerDeclarationOperation() {
functionPointerDeclarationOperation_entry:
  ret void
}

declare i32 @getMe()

declare ptr @getYou(ptr)
