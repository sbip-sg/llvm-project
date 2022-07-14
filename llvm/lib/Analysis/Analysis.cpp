//===-- Analysis.cpp ------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm-c/Analysis.h"
#include "llvm-c/Initialization.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/PassRegistry.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/TypeBasedAliasAnalysis.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include <cstring>

using namespace llvm;

/// initializeAnalysis - Initialize all passes linked into the Analysis library.
void llvm::initializeAnalysis(PassRegistry &Registry) {
  initializeAAEvalLegacyPassPass(Registry);
  initializeAliasSetPrinterPass(Registry);
  initializeBasicAAWrapperPassPass(Registry);
  initializeBlockFrequencyInfoWrapperPassPass(Registry);
  initializeBranchProbabilityInfoWrapperPassPass(Registry);
  initializeCallGraphWrapperPassPass(Registry);
  initializeCallGraphDOTPrinterPass(Registry);
  initializeCallGraphPrinterLegacyPassPass(Registry);
  initializeCallGraphViewerPass(Registry);
  initializeCostModelAnalysisPass(Registry);
  initializeCFGViewerLegacyPassPass(Registry);
  initializeCFGPrinterLegacyPassPass(Registry);
  initializeCFGOnlyViewerLegacyPassPass(Registry);
  initializeCFGOnlyPrinterLegacyPassPass(Registry);
  initializeCFLAndersAAWrapperPassPass(Registry);
  initializeCFLSteensAAWrapperPassPass(Registry);
  initializeCycleInfoWrapperPassPass(Registry);
  initializeDependenceAnalysisWrapperPassPass(Registry);
  initializeDelinearizationPass(Registry);
  initializeDemandedBitsWrapperPassPass(Registry);
  initializeDominanceFrontierWrapperPassPass(Registry);
  initializeDomViewerPass(Registry);
  initializeDomPrinterPass(Registry);
  initializeDomOnlyViewerPass(Registry);
  initializePostDomViewerPass(Registry);
  initializeDomOnlyPrinterPass(Registry);
  initializePostDomPrinterPass(Registry);
  initializePostDomOnlyViewerPass(Registry);
  initializePostDomOnlyPrinterPass(Registry);
  initializeAAResultsWrapperPassPass(Registry);
  initializeGlobalsAAWrapperPassPass(Registry);
  initializeIVUsersWrapperPassPass(Registry);
  initializeInstCountLegacyPassPass(Registry);
  initializeIntervalPartitionPass(Registry);
  initializeIRSimilarityIdentifierWrapperPassPass(Registry);
  initializeLazyBranchProbabilityInfoPassPass(Registry);
  initializeLazyBlockFrequencyInfoPassPass(Registry);
  initializeLazyValueInfoWrapperPassPass(Registry);
  initializeLazyValueInfoPrinterPass(Registry);
  initializeLegacyDivergenceAnalysisPass(Registry);
  initializeLintLegacyPassPass(Registry);
  initializeLoopInfoWrapperPassPass(Registry);
  initializeMemDepPrinterPass(Registry);
  initializeMemDerefPrinterPass(Registry);
  initializeMemoryDependenceWrapperPassPass(Registry);
  initializeModuleDebugInfoLegacyPrinterPass(Registry);
  initializeModuleSummaryIndexWrapperPassPass(Registry);
  initializeMustExecutePrinterPass(Registry);
  initializeMustBeExecutedContextPrinterPass(Registry);
  initializeObjCARCAAWrapperPassPass(Registry);
  initializeOptimizationRemarkEmitterWrapperPassPass(Registry);
  initializePhiValuesWrapperPassPass(Registry);
  initializePostDominatorTreeWrapperPassPass(Registry);
  initializeRegionInfoPassPass(Registry);
  initializeRegionViewerPass(Registry);
  initializeRegionPrinterPass(Registry);
  initializeRegionOnlyViewerPass(Registry);
  initializeRegionOnlyPrinterPass(Registry);
  initializeSCEVAAWrapperPassPass(Registry);
  initializeScalarEvolutionWrapperPassPass(Registry);
  initializeStackSafetyGlobalInfoWrapperPassPass(Registry);
  initializeStackSafetyInfoWrapperPassPass(Registry);
  initializeTargetTransformInfoWrapperPassPass(Registry);
  initializeTypeBasedAAWrapperPassPass(Registry);
  initializeScopedNoAliasAAWrapperPassPass(Registry);
  initializeLCSSAVerificationPassPass(Registry);
  initializeMemorySSAWrapperPassPass(Registry);
  initializeMemorySSAPrinterLegacyPassPass(Registry);
}

void LLVMInitializeAnalysis(LLVMPassRegistryRef R) {
  initializeAnalysis(*unwrap(R));
}

void LLVMInitializeIPA(LLVMPassRegistryRef R) {
  initializeAnalysis(*unwrap(R));
}

LLVMBool LLVMVerifyModule(LLVMModuleRef M, LLVMVerifierFailureAction Action,
                          char **OutMessages) {
  raw_ostream *DebugOS = Action != LLVMReturnStatusAction ? &errs() : nullptr;
  std::string Messages;
  raw_string_ostream MsgsOS(Messages);

  LLVMBool Result = verifyModule(*unwrap(M), OutMessages ? &MsgsOS : DebugOS);

  // Duplicate the output to stderr.
  if (DebugOS && OutMessages)
    *DebugOS << MsgsOS.str();

  if (Action == LLVMAbortProcessAction && Result)
    report_fatal_error("Broken module found, compilation aborted!");

  if (OutMessages)
    *OutMessages = strdup(MsgsOS.str().c_str());

  return Result;
}

LLVMBool LLVMVerifyFunction(LLVMValueRef Fn, LLVMVerifierFailureAction Action) {
  LLVMBool Result =
      verifyFunction(*unwrap<Function>(Fn),
                     Action != LLVMReturnStatusAction ? &errs() : nullptr);

  if (Action == LLVMAbortProcessAction && Result)
    report_fatal_error("Broken function found, compilation aborted!");

  return Result;
}

void LLVMViewFunctionCFG(LLVMValueRef Fn) {
  Function *F = unwrap<Function>(Fn);
  F->viewCFG();
}

void LLVMViewFunctionCFGOnly(LLVMValueRef Fn) {
  Function *F = unwrap<Function>(Fn);
  F->viewCFGOnly();
}

/* Check alias between two pointers.
   Using the basic alias analysis
 */
LLVMAliasResult LLVMBasicAAQuery(LLVMModuleRef ModuleRef, const char *FuncNameStr,
                                 LLVMValueRef VRef1, LLVMValueRef VRef2) {
  Module *M = unwrap(ModuleRef);
  SMDiagnostic Err;
  if (!M)
    Err.print("LLVMBasicAAQuery failed", errs());

  StringRef FuncName = llvm::StringRef(FuncNameStr);
  Value *V1 = unwrap<Value>(VRef1);
  Value *V2 = unwrap<Value>(VRef2);


  Function *Func = M->getFunction(FuncName);

  // Initialize the alias analysis.
  Triple Trip(M->getTargetTriple());
  TargetLibraryInfoImpl TLII(Trip);
  TargetLibraryInfo TLI(TLII);
  AAResults AA(TLI);
  DataLayout DL = M->getDataLayout();
  DominatorTree DT(*Func);
  LoopInfo LI(DT);
  AssumptionCache AC(*Func);

  BasicAAResult BAA(DL, *Func, TLI, AC, &DT);
  AA.addAAResult(BAA);

  AliasResult aares = AA.alias(V1, V2);

  if (aares == llvm::AliasResult::NoAlias) {
    return LLVMNoAlias;
  } else if (aares == llvm::AliasResult::MayAlias) {
    return LLVMMayAlias;
  } else if (aares == llvm::AliasResult::MustAlias) {
    return LLVMMustAlias;
  } else {
    return LLVMMayAlias;
  }

  return LLVMMayAlias;
}

/* Check alias between two pointers.
   Using the typed-based alias analysis
 */
LLVMAliasResult LLVMTypeBasedAAQuery(LLVMModuleRef ModuleRef,
                                     LLVMValueRef VRef1, LLVMValueRef VRef2) {
  Module *M = unwrap(ModuleRef);
  SMDiagnostic Err;
  if (!M)
    Err.print("LLVMTypeBasedAAQuery failed", errs());

  Value *V1 = unwrap<Value>(VRef1);
  Value *V2 = unwrap<Value>(VRef2);

  // Initialize the alias analysis.
  Triple Trip(M->getTargetTriple());
  TargetLibraryInfoImpl TLII(Trip);
  TargetLibraryInfo TLI(TLII);
  AAResults AA(TLI);

  TypeBasedAAResult TBAAR;
  AA.addAAResult(TBAAR);

  AliasResult aares = AA.alias(V1, V2);

  if (aares == llvm::AliasResult::NoAlias) {
    return LLVMNoAlias;
  } else if (aares == llvm::AliasResult::MayAlias) {
    return LLVMMayAlias;
  } else if (aares == llvm::AliasResult::MustAlias) {
    return LLVMMustAlias;
  } else {
    return LLVMMayAlias;
  }

  return LLVMMayAlias;
}
