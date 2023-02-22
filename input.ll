; ModuleID = 'input.txt'
source_filename = "input.txt"

@0 = private unnamed_addr constant [14 x i8] c"Hello World!\0A\00", align 1

declare void @c4_exit(i32)

declare void @c4_assert(i32)

declare ptr @c4_malloc(i32)

declare ptr @c4_calloc(i32, i32)

declare ptr @c4_realloc(ptr, i32)

declare void @c4_free(ptr)

declare void @c4_print_str(ptr)

declare void @c4_sprintf(ptr, ptr, i32)

declare void @c4_print_int(i32)

declare void @c4_error_str(ptr)

declare void @c4_error_int(i32)

declare i32 @c4_strcmp(ptr, ptr)

declare i32 @c4_strncmp(ptr, ptr, i32)

declare ptr @c4_strcpy(ptr, ptr)

declare ptr @c4_memcpy(ptr, ptr, i32)

declare void @c4_qsort(ptr, i32, i32, ptr)

declare i32 @c4_strlen(ptr)

declare i32 @c4_file_open(ptr, ptr)

declare void @c4_file_close()

declare i32 @c4_file_getc()

declare i32 @c4_abs(i32)

declare i32 @c4_atoi(ptr)

define void @execute_function(ptr %func) {
execute_function_entry:
  %func1 = alloca ptr, align 8
  store ptr %func, ptr %func1, align 8
  %0 = load ptr, ptr %func1, align 8
  call void %0()
  ret void
}

define void @execute_function_ptr(ptr %func) {
execute_function_ptr_entry:
  %func1 = alloca ptr, align 8
  store ptr %func, ptr %func1, align 8
  %0 = load ptr, ptr %func1, align 8
  call void %0()
  ret void
}

define void @execute_function_deref(ptr %func) {
execute_function_deref_entry:
  %func1 = alloca ptr, align 8
  store ptr %func, ptr %func1, align 8
  %0 = load ptr, ptr %func1, align 8
  call void %0()
  ret void
}

define void @execute_function_ptr_deref(ptr %func) {
execute_function_ptr_deref_entry:
  %func1 = alloca ptr, align 8
  store ptr %func, ptr %func1, align 8
  %0 = load ptr, ptr %func1, align 8
  call void %0()
  ret void
}

define void @print_hello() {
print_hello_entry:
  call void @c4_print_str(ptr @0)
  ret void
}

define i32 @main() {
main_entry:
  %print_hello_3 = alloca ptr, align 8
  %print_hello_2 = alloca ptr, align 8
  %print_hello_1 = alloca ptr, align 8
  store ptr @print_hello, ptr %print_hello_1, align 8
  %0 = load ptr, ptr %print_hello_1, align 8
  store ptr @print_hello, ptr %print_hello_2, align 8
  %1 = load ptr, ptr %print_hello_2, align 8
  store ptr @print_hello, ptr %print_hello_3, align 8
  %2 = load ptr, ptr %print_hello_3, align 8
  call void @execute_function(ptr @print_hello)
  %3 = load ptr, ptr %print_hello_1, align 8
  call void @execute_function_ptr(ptr %3)
  %4 = load ptr, ptr %print_hello_2, align 8
  call void @execute_function_deref(ptr %4)
  %5 = load ptr, ptr %print_hello_3, align 8
  call void @execute_function_ptr_deref(ptr %5)
  %6 = load ptr, ptr %print_hello_3, align 8
  call void @execute_function_ptr_deref(ptr %6)
  %7 = load ptr, ptr %print_hello_3, align 8
  call void @execute_function_ptr_deref(ptr %7)
  ret i32 0
}
