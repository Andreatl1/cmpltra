; ModuleID = '/home/andrea/Documenti/uni/compilatori/parte2/cmpltra1/assignment3/test/Es2.bc'
source_filename = "/home/andrea/Documenti/uni/compilatori/parte2/cmpltra1/assignment3/test/Es2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local void @f() #0 {
entry:
  %cmp = icmp sgt i32 1, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %add = add nsw i32 1, 1
  br label %if.end8

if.else:                                          ; preds = %entry
  %add1 = add nsw i32 1, 2
  %rem = srem i32 %add1, 2
  %cmp2 = icmp eq i32 %rem, 0
  br i1 %cmp2, label %if.then3, label %if.else5

if.then3:                                         ; preds = %if.else
  %add4 = add nsw i32 %add1, 3
  br label %if.end

if.else5:                                         ; preds = %if.else
  %add6 = add nsw i32 %add1, 4
  br label %if.end

if.end:                                           ; preds = %if.else5, %if.then3
  %x.0 = phi i32 [ %add4, %if.then3 ], [ %add6, %if.else5 ]
  %add7 = add nsw i32 %x.0, 5
  br label %if.end8

if.end8:                                          ; preds = %if.end, %if.then
  %x.1 = phi i32 [ %add, %if.then ], [ %add7, %if.end ]
  %add9 = add nsw i32 %x.1, 10
  ret void
}

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local i32 @main() #0 {
entry:
  call void @f()
  ret i32 0
}

attributes #0 = { noinline nounwind sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 19.1.7"}
