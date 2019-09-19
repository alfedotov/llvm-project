# RUN: llc -o - %s -m=armv8r-- -verify-machineinstrs | FileCheck %s

;Pass to optimize following pattern and eliminate extra checks & branches:
;shift: u8, int: u32
;==
;    if (shift == 0) {
;        return 0;
;    } else {
;        return (int >> @truncate(u5, (32 - shift)));
;    }
;== 

target triple = "armv8r-none-unknown-eabi"

; CHECK: lsr
; Function Attrs: norecurse nounwind readnone
define dso_local i32 @test_opt_shift(i32 %input, i8 zeroext %shift) local_unnamed_addr #0 {
entry:
  %cmp = icmp eq i8 %shift, 0
  %conv = zext i8 %shift to i32
  %sub = sub nsw i32 32, %conv
  %shr = lshr i32 %input, %sub
  %and = and i32 %shr, 31
  %retval.0 = select i1 %cmp, i32 0, i32 %and
  ret i32 %retval.0
}

