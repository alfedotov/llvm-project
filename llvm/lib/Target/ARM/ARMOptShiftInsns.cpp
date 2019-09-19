//===-- ARMLowOverheadLoops.cpp - CodeGen Low-overhead Loops ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "ARM.h"
#include "ARMBaseInstrInfo.h"
#include "ARMBaseRegisterInfo.h"
#include "ARMBasicBlockInfo.h"
#include "ARMSubtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "arm-opt-shift-insns"
#define ARM_OPT_SHIFT_INSNS "ARM Optimize Shift Instructions pass"

using RegisterSet = SmallSet<unsigned, 4>;

namespace {

class ARMOptShiftInsns : public MachineFunctionPass {
  MachineFunction *MF = nullptr;
  const ARMBaseInstrInfo *TII = nullptr;
  MachineRegisterInfo *MRI = nullptr;
  std::unique_ptr<ARMBasicBlockUtils> BBUtils = nullptr;

public:
  static char ID;

  ARMOptShiftInsns() : MachineFunctionPass(ID) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    AU.addRequired<MachineLoopInfo>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  MachineFunctionProperties getRequiredProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::NoVRegs);
  }

  StringRef getPassName() const override { return ARM_OPT_SHIFT_INSNS; }

private:
  bool pattern_CMP_ANDne(MachineFunction &MF);
};
} // namespace

char ARMOptShiftInsns::ID = 0;

INITIALIZE_PASS(ARMOptShiftInsns, DEBUG_TYPE, ARM_OPT_SHIFT_INSNS, false, false)



bool ARMOptShiftInsns::runOnMachineFunction(MachineFunction &mf) {
  const ARMSubtarget &ST = static_cast<const ARMSubtarget &>(mf.getSubtarget());
  MF = &mf;
  MRI = &MF->getRegInfo();
  TII = static_cast<const ARMBaseInstrInfo *>(ST.getInstrInfo());

  bool Modified = false;

  // We can have different cases (depends on which ISA is used Thumb/ARM)
  Modified |= pattern_CMP_ANDne(mf);

  return Modified;
}


// Try to optimize pattern:
//
//	RSB r2,r1,#32
//  MOV reg r3, #31
//	CMP r1, #0
//  BB_ne:
//    ANDNE r1,r3,r0, lsr r2
//	BB_eq:
//    MOV r0, r1
//    BX lr
//
// where CMP + ANDne + MOV can be reduced to the:
//
//  RSB r2,r1,#32
//  LSR r0,r0,r1
//  BX lr

bool ARMOptShiftInsns::pattern_CMP_ANDne(MachineFunction &mf) {
  bool Modified = false;
  MachineInstr *candidate = nullptr;
  RegisterSet Defs, Uses;

  for (auto &MBB : mf) {
    for (MachineInstr &MI : MBB) {

      errs() << MI << "\n";
      //search for ANDne
      if (MI.getOpcode() == ARM::ANDrsr) {
        candidate = &MI;
      }

      /* now check all other stuff */

    }

  }

  return Modified;
}

FunctionPass *llvm::createARMOptShiftInsnsPass() {
  return new ARMOptShiftInsns();
}
