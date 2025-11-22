#include "Validator.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownValidatorModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Validator...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeValidatorModule() {
	_logger = createLogger("Validator");
	return _shutdownValidatorModule;
}


ValidationResult executeValidator(CompilerState * compilerState, SymbolTable * table) {
	initSymbolTable(table);
	Program * program = compilerState->abstractSyntaxtTree;
	bool validHeader = validateHeader(program->header);
	bool validYearBlock = validateYearBlock(program->yearBlock);
	ValidationResult validationResult = {
 		.succeeded = validHeader && validYearBlock,
		.value = 0
	};
	return validationResult;
}

bool validateHeader(Header * header){
	bool validTimezone = validateTimezone(header->timezoneDecl);
	bool validColorList = validateColorList(header->colorList);
	return validTimezone && validColorList;
}

bool validateTimezone(TimezoneDecl * timezoneDecl) {
	if(timezoneDecl == NULL){
		logDebugging(_logger, "Valid Timezone (no timezone)");
		return true;
	}
	char * timezone = timezoneDecl->timezone;

	char *signPtr = strchr(timezone, '+');
    if (signPtr == NULL) {
        signPtr = strchr(timezone, '-');
    }
    if (signPtr == NULL) {
		logError(_logger, "Invalid Timezone, no sign");
        return false; 
    }

	const char *validPrefixes[] = {
        "UTC", "GMT",
        "EST","EDT","CST","CDT","MST","MDT","PST","PDT",
        "WET","CET","EET"
    };
	int prefixLen = signPtr - timezone;
	char * prefix;
	strncpy(prefix, timezone, prefixLen);
	prefix[prefixLen] = '\0';
	bool prefixOK = false;
    for (int i = 0; validPrefixes[i] != NULL && prefixOK==false; i++) {
        if (strcmp(prefix, validPrefixes[i]) == 0) {
            prefixOK = true;
        }
    }
    if (!prefixOK){
		logError(_logger, "Invalid Timezone, prefix not recognized");
		return false;
	}

	int sign = (*signPtr == '-') ? -1 : 1;
    int offset = atoi(signPtr + 1);  // La regex ya garantiza números válidos

    int finalOffset = sign * offset;
    if (finalOffset < -12 || finalOffset > 14) {
		logError(_logger, "Invalid Timezone, invalid offset");
        return false;
    }

	logDebugging(_logger, "Valid Timezone");
	return true;
}

bool validateColorList(ColorList * colorList) {
	if(colorList == NULL){
		logDebugging(_logger, "Valid ColorList (no colorList)");
	}
	ColorDef * current = colorList->firstColorDef;
	
	bool success = true;
	while(current != NULL && success){
		ColorData * data = {
							current->color->hexValue
						};
		success = addSymbol(current->color->name, ENTITY_COLOR, data);
		if(!success){
			logError(_logger, "Invalid color declaration");
			return false;
		}
		current = current->next;
	}

	logDebugging(_logger, "Valid ColorList");
	return true;
}

bool validateYearBlock(YearBlock * yearBlock){
	return true;
}