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