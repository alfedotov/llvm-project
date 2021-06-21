; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -S -instcombine < %s | FileCheck %s

define ptr @bitcast_opaque_to_opaque(ptr %a) {
; CHECK-LABEL: @bitcast_opaque_to_opaque(
; CHECK-NEXT:    ret ptr [[A:%.*]]
;
  %b = bitcast ptr %a to ptr
  ret ptr %b
}

define ptr @bitcast_typed_to_opaque(i8* %a) {
; CHECK-LABEL: @bitcast_typed_to_opaque(
; CHECK-NEXT:    [[B:%.*]] = bitcast i8* [[A:%.*]] to ptr
; CHECK-NEXT:    ret ptr [[B]]
;
  %b = bitcast i8* %a to ptr
  ret ptr %b
}

define i8* @bitcast_opaque_to_typed(ptr %a) {
; CHECK-LABEL: @bitcast_opaque_to_typed(
; CHECK-NEXT:    [[B:%.*]] = bitcast ptr [[A:%.*]] to i8*
; CHECK-NEXT:    ret i8* [[B]]
;
  %b = bitcast ptr %a to i8*
  ret i8* %b
}

;define ptr @addrspacecast_opaque_to_opaque(ptr addrspace(1) %a) {
;  %b = addrspacecast ptr addrspace(1) %a to ptr
;  ret ptr %b
;}

;define ptr @addrspacecast_typed_to_opaque(i8 addrspace(1)* %a) {
;  %b = addrspacecast i8 addrspace(1)* %a to ptr
;  ret ptr %b
;}

;define i8* @addrspacecast_opaque_to_typed(ptr addrspace(1) %a) {
;  %b = addrspacecast ptr addrspace(1) %a to i8*
;  ret i8* %b
;}