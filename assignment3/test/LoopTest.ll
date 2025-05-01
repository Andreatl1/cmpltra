; ModuleID = '/home/andrea/Documenti/uni/compilatori/parte2/cmpltra1/assignment3/test/LoopTest.bc'
source_filename = "/home/andrea/Documenti/uni/compilatori/parte2/cmpltra1/assignment3/test/LoopTest.c"
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
define dso_local void @nested_loop(i32 noundef %n) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc4, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc5, %for.inc4 ]
  %cmp = icmp slt i32 %i.0, %n
  br i1 %cmp, label %for.body, label %for.end6

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %j.0 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %cmp2 = icmp slt i32 %j.0, %n
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %mul = mul nsw i32 %i.0, %j.0
  %0 = load i32, ptr @g, align 4
  %add = add nsw i32 %0, %mul
  store i32 %add, ptr @g, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %j.0, 1
  br label %for.cond1, !llvm.loop !9

for.end:                                          ; preds = %for.cond1
  br label %for.inc4

for.inc4:                                         ; preds = %for.end
  %inc5 = add nsw i32 %i.0, 1
  br label %for.cond, !llvm.loop !10

for.end6:                                         ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local void @complex_loops(i32 noundef %x) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc10, %entry
  %a.0 = phi i32 [ 0, %entry ], [ %inc11, %for.inc10 ]
  %cmp = icmp slt i32 %a.0, %x
  br i1 %cmp, label %for.body, label %for.end12

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %b.0 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %cmp2 = icmp slt i32 %b.0, 5
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %0 = load i32, ptr @g, align 4
  %add = add nsw i32 %0, %b.0
  store i32 %add, ptr @g, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %b.0, 1
  br label %for.cond1, !llvm.loop !11

for.end:                                          ; preds = %for.cond1
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc7, %for.end
  %c.0 = phi i32 [ 0, %for.end ], [ %inc8, %for.inc7 ]
  %cmp5 = icmp slt i32 %c.0, 10
  br i1 %cmp5, label %for.body6, label %for.end9

for.body6:                                        ; preds = %for.cond4
  %1 = load i32, ptr @g, align 4
  %sub = sub nsw i32 %1, %c.0
  store i32 %sub, ptr @g, align 4
  br label %for.inc7

for.inc7:                                         ; preds = %for.body6
  %inc8 = add nsw i32 %c.0, 1
  br label %for.cond4, !llvm.loop !12

for.end9:                                         ; preds = %for.cond4
  br label %for.inc10

for.inc10:                                        ; preds = %for.end9
  %inc11 = add nsw i32 %a.0, 1
  br label %for.cond, !llvm.loop !13

for.end12:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local i32 @main() #0 {
entry:
  call void @sequential_loops(i32 noundef 3, i32 noundef 5)
  call void @nested_loop(i32 noundef 4)
  call void @complex_loops(i32 noundef 2)
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
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
