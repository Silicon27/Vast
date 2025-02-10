; ModuleID = '__main__'
source_filename = "__main__"

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 10, ptr %x, align 4
  ret i32 3
}
