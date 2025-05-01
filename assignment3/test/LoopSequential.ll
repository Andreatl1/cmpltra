; ModuleID = '/home/andrea/Documenti/uni/compilatori/parte2/cmpltra1/assignment3/test/LoopSequential.bc'
source_filename = "/home/andrea/Documenti/uni/compilatori/parte2/cmpltra1/assignment3/test/LoopSequential.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@g = dso_local global i32 0, align 4

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local void @sequential_loops(i32 noundef %a, i32 noundef %b) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc1, %for.inc ]
  %cmp = icmp slt i32 %i.0, %a
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %0 = load i32, ptr @g, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, ptr @g, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc1 = add nsw i32 %i.0, 1
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc5, %for.end
  %j.0 = phi i32 [ 0, %for.end ], [ %inc6, %for.inc5 ]
  %cmp3 = icmp slt i32 %j.0, %b
  br i1 %cmp3, label %for.body4, label %for.end7

for.body4:                                        ; preds = %for.cond2
  %1 = load i32, ptr @g, align 4
  %dec = add nsw i32 %1, -1
  store i32 %dec, ptr @g, align 4
  br label %for.inc5

for.inc5:                                         ; preds = %for.body4
  %inc6 = add nsw i32 %j.0, 1
  br label %for.cond2, !llvm.loop !8

for.end7:                                         ; preds = %for.cond2
  ret void
}

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local i32 @main() #0 {
entry:
  call void @sequential_loops(i32 noundef 3, i32 noundef 5)
  %0 = load i32, ptr @g, align 4
  ret i32 %0
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
!8 = distinct !{!8, !7}
