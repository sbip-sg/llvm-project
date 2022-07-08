/*===-- llvm-c/Analysis.h - Analysis Library C Interface --------*- C++ -*-===*\
|*                                                                            *|
|* Part of the LLVM Project, under the Apache License v2.0 with LLVM          *|
|* Exceptions.                                                                *|
|* See https://llvm.org/LICENSE.txt for license information.                  *|
|* SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception                    *|
|*                                                                            *|
|*===----------------------------------------------------------------------===*|
|*                                                                            *|
|* This header declares the C interface to libLLVMAnalysis.a, which           *|
|* implements various analyses of the LLVM IR.                                *|
|*                                                                            *|
|* Many exotic languages can interoperate with C code but have a harder time  *|
|* with C++ due to name mangling. So in addition to C, this interface enables *|
|* tools written in such languages.                                           *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifndef LLVM_C_ANALYSIS_H
#define LLVM_C_ANALYSIS_H

#include "llvm-c/ExternC.h"
#include "llvm-c/Types.h"

LLVM_C_EXTERN_C_BEGIN

/**
 * @defgroup LLVMCAnalysis Analysis
 * @ingroup LLVMC
 *
 * @{
 */

typedef enum {
  LLVMAbortProcessAction, /* verifier will print to stderr and abort() */
  LLVMPrintMessageAction, /* verifier will print to stderr and return 1 */
  LLVMReturnStatusAction  /* verifier will just return 1 */
} LLVMVerifierFailureAction;


/* Verifies that a module is valid, taking the specified action if not.
   Optionally returns a human-readable description of any invalid constructs.
   OutMessage must be disposed with LLVMDisposeMessage. */
LLVMBool LLVMVerifyModule(LLVMModuleRef M, LLVMVerifierFailureAction Action,
                          char **OutMessage);

/* Verifies that a single function is valid, taking the specified action. Useful
   for debugging. */
LLVMBool LLVMVerifyFunction(LLVMValueRef Fn, LLVMVerifierFailureAction Action);

/* Open up a ghostview window that displays the CFG of the current function.
   Useful for debugging. */
void LLVMViewFunctionCFG(LLVMValueRef Fn);
void LLVMViewFunctionCFGOnly(LLVMValueRef Fn);

/* Check alias between two pointers using the basic-aa analysis.
   Inputs are the current Module, the FuncName of the current function and the
   two pointers V1 and V2 to be checked.

   Output is a LLVMAliasResult.
*/
LLVMAliasResult LLVMBasicAAQuery(LLVMModuleRef Module, char *FuncName, LLVMValueRef V1, LLVMValueRef V2);

/* Check alias between two pointers using the type-based alias analysis.
   Inputs are the current Module, the FuncName of the current function and the
   two pointers V1 and V2 to be checked.

   Output is a LLVMAliasResult.
*/
LLVMAliasResult LLVMTypeBasedAAQuery(LLVMModuleRef Module, LLVMValueRef V1, LLVMValueRef V2);

/**
 * @}
 */

LLVM_C_EXTERN_C_END

#endif
