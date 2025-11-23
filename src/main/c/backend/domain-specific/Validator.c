#include "Validator.h"

/* STATIC FUNCTIONS */
static bool validateHeader(Header * header);
static bool validateTimezone(TimezoneDecl * timezoneDecl);
static bool validateColorList(ColorList * colorList);
static bool validateYearBlock(YearBlock * yearBlock);
static bool validateMonthBlocks(MonthBlocks * monthBlocks, int year);
static bool validateMonthBlock(MonthBlock * monthBlock, int year);
static bool validateStatement(Statement * statement, int month, int year);
static bool validateEvent(EventDecl * event, int month, int year);
static bool validateTimeInterval(Time *start, Time *end);
static bool validateDate(int year, int month, int day);
static bool validateOverride(OverrideDecl * override, int month, int year);
static DayNumber * expandWeekdays(int year, int month, int *weekdayList, int weekdayCount);

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
	logDebugging(_logger, "hola1");
	bool validHeader = validateHeader(program->header);
	bool validYearBlock = validateYearBlock(program->yearBlock);
	ValidationResult validationResult = {
 		.succeeded = validHeader && validYearBlock,
		.value = 0
	};
	return validationResult;
}

static bool validateHeader(Header * header){
	if(header == NULL){
		logDebugging(_logger, "Valid header (no header)");
		return true;
	}
	logDebugging(_logger, "in header");
	bool validTimezone = validateTimezone(header->timezoneDecl);
	logDebugging(_logger, "timezone ok");
	bool validColorList = validateColorList(header->colorList);
	return validTimezone && validColorList;
}

static bool validateTimezone(TimezoneDecl * timezoneDecl) {
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
	char prefix[prefixLen+1];
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

static bool validateColorList(ColorList * colorList) {
	if(colorList == NULL){
		logDebugging(_logger, "Valid ColorList (no colorList)");
		return true;
	}
	ColorDef * current = colorList->firstColorDef;
	
	bool success = true;
	while(current != NULL && success){  //mejorar estilo
		ColorData * data = calloc(1, sizeof(ColorData));
		data->hexColor = strdup(current->color->hexValue);
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

static bool validateYearBlock(YearBlock * yearBlock){
	if(yearBlock == NULL){
		logError(_logger, "Invalid yearBlock, (no yearBlock)");
		return false;
	}
	if(yearBlock->year < 0){
		logError(_logger, "Invalid year, year cannot be a negative number");
	}
	return validateMonthBlocks(yearBlock->monthBlocks, yearBlock->year);
}

static bool validateMonthBlocks(MonthBlocks * monthBlocks, int year){
	for(int i=0; i < 12; i++){
		if(!validateMonthBlock(monthBlocks->months[i], year)){
			logError(_logger, "Invalid monthBlock n°%d", monthBlocks->months[i]->month);
			return false;
		}
	}
	return true;
}

static bool validateMonthBlock(MonthBlock * monthBlock, int year){
	if(monthBlock == NULL){
		return true;
	}
	Statement * current = monthBlock->statements->firstStatement;
	while(current != NULL){
		if(!validateStatement(current, monthBlock->month, year)){
			return false;
		}
		current = current->nextStatement;
	}
	return true;
}

static bool validateStatement(Statement * statement, int month, int year) {
	if(statement->type == STATEMENT_EVENT){
		return validateEvent(statement->eventDecl, month, year);
	}else if(statement->type == STATEMENT_OVERRIDE){
		return validateOverride(statement->OverrideDecl, month, year);
	}else{
		logError(_logger, "Invalid statement: type not recognized");
		return false;
	}
}

static bool validateEvent(EventDecl * event, int month, int year){
	if(!validateTimeInterval(event->eventSpec->start, event->eventSpec->end)){
		logError(_logger, "Invalid Event declaration '%s': invalid time interval", event->identifier);
		return false;
	}
	if(event->eventSpec->type == SPEC_DAYOFMONTH && !validateDate(year, month, event->eventSpec->dayOfMonth)){
		logError(_logger, "Invalid Event declaration '%s': invalid date");
		return false;
	}

	EventData * data = calloc(1, sizeof(EventData));
	data->year = year;
	data->month = month;
	data->start = event->eventSpec->start;
	data->end = event->eventSpec->end;
	if(event->eventSpec->type == SPEC_DAYLIST){
		data->days = expandWeekdays(year, month, event->eventSpec->dayList->days, event->eventSpec->dayList->count);
	}else{
		DayNumber * day = calloc(1, sizeof(DayNumber));
		day = event->eventSpec->dayOfMonth;
		data->days = day;
	}
	if(!addSymbol(event->identifier, ENTITY_EVENT, data)){
		logError(_logger, "Invalid Event declaration '%s': collision", event->identifier);
		return false;
	}
	if(findSymbol(event->eventBody->colorId) == NULL){
		logError(_logger, "Invalid Event declaration '%s': color not previously declared", event->identifier);
		return false;
	}

	logDebugging(_logger, "Valid Event declaration: '%s'", event->identifier);
	return true;
}

static bool validateTimeInterval(Time *start, Time *end) {
    if (start->hour   < 0 || start->hour   > 23) return false;
    if (start->minute < 0 || start->minute > 59) return false;
    if (end->hour     < 0 || end->hour     > 23) return false;
    if (end->minute   < 0 || end->minute   > 59) return false;

    int startMinutes = start->hour * 60 + start->minute;
    int endMinutes   = end->hour   * 60 + end->minute;
    if (startMinutes >= endMinutes) return false;

    return true;
}

static bool validateDate(int year, int month, int day) {
    int dim = daysInMonth(year, month);

    if (day < 1 || day > dim){
		logError(_logger, "Invalid Date");
		return false;
	}
    return true;
}

//ACTUALIZAR (el override es mierda)
static bool validateOverride(OverrideDecl * override, int month, int year){
	if(findSymbol(override->identifier) == NULL){
		logError(_logger, "Invalid Override declaration: event '%s' not found", override->identifier);
		return false;
	}
	if(findSymbol(override->eventBody->colorId) == NULL){
		logError(_logger, "Invalid Override declaration '%s': color not previously declared", override->identifier);
		return false;
	}
	logDebugging(_logger, "Valid Override declaration");
	return true;
}


static DayNumber * expandWeekdays(int year, int month, int *weekdayList, int weekdayCount) {
    int dim = daysInMonth(year, month);
    DayNumber *head = NULL;
    DayNumber *tail = NULL;

    for (int d = 1; d <= dim; d++) {
        int tm_wday = weekday(year, month, d);      // 0=Sunday..6=Saturday
        int myday = normalizeWeekday(tm_wday);      // 0=Mon..6=Sun

        // Ver si myday está en weekdayList
        int match = 0;
        for (int i = 0; i < weekdayCount; i++) {
            if (weekdayList[i] == myday) {
                match = 1;
                break;
            }
        }

        if (match) {
            DayNumber *node = malloc(sizeof(DayNumber));
            node->day = d;
            node->next = NULL;

            if (head == NULL) {
                head = tail = node;
            } else {
                tail->next = node;
                tail = node;
            }
        }
    }

    return head;
}