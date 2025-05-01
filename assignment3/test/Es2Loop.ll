; ModuleID = '/home/andrea/Documenti/uni/compilatori/parte2/cmpltra1/assignment3/test/Es2Loop.bc'
source_filename = "/home/andrea/Documenti/uni/compilatori/parte2/cmpltra1/assignment3/test/Es2Loop.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local void @f() #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %cmp1 = icmp sgt i32 1, 0
  br i1 %cmp1, label %if.then, label %if.else

if.then:                                          ; preds = %for.body
  %add = add nsw i32 1, 1
  br label %if.end9

if.else:                                          ; preds = %for.body
  %add2 = add nsw i32 1, 2
  %rem = srem i32 %add2, 2
  %cmp3 = icmp eq i32 %rem, 0
  br i1 %cmp3, label %if.then4, label %if.else6

if.then4:                                         ; preds = %if.else
  %add5 = add nsw i32 %add2, 3
  br label %if.end

if.else6:                                         ; preds = %if.else
  %add7 = add nsw i32 %add2, 4
  br label %if.end

if.end:                                           ; preds = %if.else6, %if.then4
  %x.0 = phi i32 [ %add5, %if.then4 ], [ %add7, %if.else6 ]
  %add8 = add nsw i32 %x.0, 5
  br label %if.end9

if.end9:                                          ; preds = %if.end, %if.then
  %x.1 = phi i32 [ %add, %if.then ], [ %add8, %if.end ]
  %add10 = add nsw i32 %x.1, 10
  br label %for.inc

for.inc:                                          ; preds = %if.end9
  %inc = add nsw i32 %i.0, 1
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
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
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
