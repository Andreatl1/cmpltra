; ModuleID = '/home/andrea/Documenti/uni/compilatori/parte2/cmpltra1/assignment1/test/AITests.bc'
source_filename = "/home/andrea/Documenti/uni/compilatori/parte2/cmpltra1/assignment1/test/AITests.ll"

define dso_local i32 @test(i32 %x, i32 %y, i32 %z) {

  %add1 = add nsw i32 %x, 0    ; Deve diventare %x
  
  %mul1 = mul nsw i32 %add1, 1 ; Deve diventare %add1 → %x
  
  %mul2 = mul nsw i32 %y, 15   ; Deve diventare (y << 4) - y
  
  %div = sdiv i32 %z, 8        ; Deve diventare z >> 3
  
  %b = add nsw i32 %mul1, 1    ; b = x + 1
  %c = sub nsw i32 %b, 1       ; c = (x + 1) - 1 → x
  
  %res1 = add i32 %mul2, %div
  %res2 = add i32 %res1, %c
  ret i32 %res2
}

define i32 @test_identities(i32 %x, i32 %y) {
  %add1 = add i32 %x, 0
  %add2 = add i32 0, %y
  %mul1 = mul i32 %x, 1
  %mul2 = mul i32 1, %y
  %tmp1 = add i32 %add1, %add2
  %tmp2 = add i32 %mul1, %mul2
  %result = add i32 %tmp1, %tmp2
  ret i32 %result
}

; mul section

define i32 @test_mul_zero(i32 %x) {
  %res = mul i32 %x, 0  
  ret i32 %res
}

define i32 @test_mul_negative_1(i32 %x) {
  %res = mul i32 %x, -1
  ret i32 %res
}

define i32 @test_mul_15(i32 %x) {
  %res = mul i32 %x, 15
  ret i32 %res
}

define i32 @test_mul_17(i32 %x) {
  %res = mul i32 %x, 17
  ret i32 %res
}


define i32 @test_mul_25(i32 %x) {
  %res = mul i32 %x, 25
  ret i32 %res
}

define i32 @test_mul_28(i32 %x) {
  %res = mul i32 %x, 28
  ret i32 %res
}

define i32 @test_mul_31(i32 %x) {
  %res = mul i32 %x, 31
  ret i32 %res
}

define i32 @test_mul_32(i32 %x) {
  %res = mul i32 %x, 32
  ret i32 %res
}


; div section

define i32 @test_div_zero(i32 %x) {
  %res = sdiv i32 %x, 0
  ret i32 %res
}

define i32 @test_div_negative_1(i32 %x) {
  %res = sdiv i32 %x, -1
  ret i32 %res
}

define i32 @test_div_32(i32 %x) {
  %res = sdiv i32 %x, 32
  ret i32 %res
}


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


define i32 @test4(i32 %0) {
  %2 = add i32 %0, 1    ; a = b + 1
  %3 = sub i32 %2, 1    ; c = a - 1
  ret i32 %3
}

define i32 @test5(i32 %0) {
  %2 = add nsw i32 %0, 4  ; a = b + 4
  %3 = sub nsw i32 %2, 4  ; c = a - 4
  %4 = add nsw i32 %3, 2  ; d = c + 2
  %5 = sub nsw i32 %4, 2  ; e = d - 2
  %6 = sub nsw i32 %5, 3  ; f = e - 3
  %7 = add nsw i32 %6, 3  ; g = f + 3
  ret i32 %7
}

define i32 @test6(i32 %0) {
  %2 = mul i32 %0, 4     ; a = b * 4
  %3 = sdiv i32 %2, 4    ; c = a / 4
  %4 = add nsw i32 %3, 2 ; d = c + 2
  %5 = sub nsw i32 %4, 2 ; e = d - 2
  ret i32 %5
}


