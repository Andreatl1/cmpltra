; Funzione che contiene identità algebriche da ottimizzare
define i32 @test_identities(i32 %x, i32 %y) {
entry:
  %add1 = add i32 %x, 0          
  %add2 = add i32 0, %y            
  
  %mul1 = mul i32 %x, 1         
  %mul2 = mul i32 1, %y        
  
  %tmp1 = add i32 %add1, %add2
  %tmp2 = add i32 %mul1, %mul2
  %result = add i32 %tmp1, %tmp2
  
  ret i32 %result
}

