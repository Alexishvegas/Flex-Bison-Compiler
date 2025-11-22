#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../domain-specific/SymbolTable.h"
#include "../../support/language/String.h"
#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include "../../support/utils/TimeUtils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/** Initialize module's internal state. */
ModuleDestructor initializeGeneratorModule();

/**
 * Generates the final output using the current compiler state.
 */
bool executeGenerator(CompilerState * compilerState);

#endif
