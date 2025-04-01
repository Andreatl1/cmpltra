define i32 @test1(i32 %0) {
  %2 = add i32 %0, 1    ; a = b + 1
  %3 = sub i32 %2, 1    ; c = a - 1
  ret i32 %3
}

define i32 @test2(i32 %0) {
  %2 = add nsw i32 %0, 4  ; a = b + 4
  %3 = sub nsw i32 %2, 4  ; c = a - 4
  %4 = add nsw i32 %3, 2  ; d = c + 2
  %5 = sub nsw i32 %4, 2  ; e = d - 2
  %6 = sub nsw i32 %5, 3  ; f = e - 3
  %7 = add nsw i32 %6, 3  ; g = f + 3
  ret i32 %7
}

define i32 @test3(i32 %0) {
  %2 = mul i32 %0, 4     ; a = b * 4
  %3 = sdiv i32 %2, 4    ; c = a / 4
  %4 = add nsw i32 %3, 2 ; d = c + 2
  %5 = sub nsw i32 %4, 2 ; e = d - 2
  ret i32 %5
}

