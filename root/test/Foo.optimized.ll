; ModuleID = 'Foo.optimized.bc'
source_filename = "Foo.ll"

define dso_local i32 @foo(i32 noundef %0, i32 noundef %1) {
  %3 = add nsw i32 %1, 1
  %4 = mul nsw i32 %3, 4 ; operarzione non rimossa ma solo slinkata come da specifica
  %5 = shl i32 %3, 2 ; qui è stata applicata la strength reduction
  %6 = shl i32 %0, 1 ; questa istruzione era già presente
  %7 = sdiv i32 %6, 4
  %8 = mul nsw i32 %5, %7
  ret i32 %8
}
