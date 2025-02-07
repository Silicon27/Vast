; ModuleID = '__main__'
source_filename = "__main__"

@x = external global i32

define i32 @main(i32 %x) {
entry:
  %y = alloca i32, align 4
  store i32 10, ptr %y, align 4
  ret i32 0
}
