#include "BisonActions.h"


/* MODULE INTERNAL STATE */

static CompilerState * _compilerState = NULL;
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownBisonActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: BisonActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	_compilerState = NULL;
}

ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState) {
	_compilerState = compilerState;
	_logger = createLogger("BisonActions");
	return _shutdownBisonActionsModule;
}

/* IMPORTED FUNCTIONS */

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

EventProp * ColorSemanticAction(char * colorId) { 
    _logSyntacticAnalyzerAction(__FUNCTION__);
    EventProp * prop = calloc(1, sizeof(EventProp));
    prop->type = EVENT_PROP_COLOR;
    prop->colorId = colorId; //strdup
    return prop;
}

EventProp * DescriptionSemanticAction(char * description) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    EventProp * prop = calloc(1, sizeof(EventProp));
    prop->type = EVENT_PROP_DESCRIPTION;
    prop->description = description;
    return prop;
}

EventProp * UrlSemanticAction(char * url) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    EventProp * prop = calloc(1, sizeof(EventProp));
    prop->type = EVENT_PROP_URL;
    prop->url = url; //strdup
    return prop;
}


EventBody * EventBodySingleSemanticAction(EventProp * prop) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    EventBody * body = calloc(1, sizeof(EventBody));

    switch (prop->type) {
        case EVENT_PROP_COLOR: body->colorId = prop->colorId; break;
        case EVENT_PROP_DESCRIPTION: body->description = prop->description; break;
        case EVENT_PROP_URL: body->url = prop->url; break;
        default: 
            logError(_logger, "Event property type not recognized");
            return NULL;
    }

    free(prop); 
    return body;
}

EventBody * EventBodyAppendSemanticAction(EventBody * body, EventProp * prop) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    switch (prop->type) {
        case EVENT_PROP_COLOR:
            if (body->colorId != NULL) logError(_logger, "Color already defined in event body");
            body->colorId = prop->colorId;
            break;
        case EVENT_PROP_DESCRIPTION:
            if (body->description != NULL) logError(_logger, "Description already defined in event body");
            body->description = prop->description;
            break;
        case EVENT_PROP_URL:
            if (body->url != NULL) logError(_logger, "URL already defined in event body");
            body->url = prop->url;
            break;
        default: 
            logError(_logger, "Event property type not recognized");
            return NULL;
    }
    free(prop);
    return body;
}

int WeekdaySemanticAction(int day){
    return day;
}


DayList * SingleDaySemanticAction(int day) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    DayList * dayList = calloc(1, sizeof(DayList));
    dayList->days[day - 1] = day;
    logDebugging(_logger, "DAY SAVED: %d in %d", day, day-1);
    dayList->count = 1;

    return dayList;
}

DayList * AppendDaySemanticAction(DayList * dayList, int day) {
    _logSyntacticAnalyzerAction(__FUNCTION__);

    int index = day - 1;
    if (dayList->days[index] != 0) {
        return dayList;
    }
    dayList->days[index] = day;
    dayList->count++;
    return dayList;
}

Time * TimeSemanticAction(char * timeString) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    int hour, minute;
    sscanf(timeString, "%d:%d", &hour, &minute); 
    Time * time = calloc(1, sizeof(Time));
    time->hour = hour;
    time->minute = minute;
	free(timeString);
    return time;
}


EventSpec * DayListSpecSemanticAction(DayList * dayList, Time * start, Time * end){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	EventSpec * spec = calloc(1, sizeof(EventSpec));
	spec->type = SPEC_DAYLIST;
	spec->dayList = dayList;
	spec->start = start;
	spec->end = end;
	return spec;
}

EventSpec * DayOfMonthSpecSemanticAction(int dayOfMonth, Time * start, Time * end){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	EventSpec * spec = calloc(1, sizeof(EventSpec));
	spec->type = SPEC_DAYOFMONTH;
	spec->dayOfMonth = dayOfMonth;
	spec->start = start;
	spec->end = end;
	return spec;
}


OverrideDecl * CreateOverrideSemanticAction(char * identifier, EventBody * body) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	OverrideDecl * override = calloc(1, sizeof(OverrideDecl));
	override->identifier = identifier; //strdup
	override->eventBody = body;
	return override;
}

EventDecl * CreateEventWithoutBodySemanticAction(char * identifier, EventSpec * spec){
    return CreateEventSemanticAction(identifier, spec, NULL);
}

EventDecl * CreateEventSemanticAction(char * identifier, EventSpec * spec, EventBody * body) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	EventDecl * event = calloc(1, sizeof(EventDecl));
	event->identifier = identifier; //strdup
	event->eventSpec = spec;
	event->eventBody = body;
	return event;
}

Statement * OverrideStatementSemanticAction(OverrideDecl * override) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Statement * statement = calloc(1, sizeof(Statement));
	statement->nextStatement = NULL;
	statement->type = STATEMENT_OVERRIDE;
	statement->OverrideDecl = override;
	return statement;
}

Statement * EventStatementSemanticAction(EventDecl * event) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Statement * statement = calloc(1, sizeof(Statement));
	statement->nextStatement = NULL;
	statement->type = STATEMENT_EVENT;
	statement->eventDecl = event;
	return statement;
}

Statements * SingleStatementListSemanticAction(Statement * statement) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statements * statements = calloc(1, sizeof(Statements));
    statements->firstStatement = statement;
    return statements;
}

Statements * AppendStatementSemanticAction(Statements * statements, Statement * statement) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * current = statements->firstStatement;
    while (current->nextStatement != NULL) {
        current = current->nextStatement;
    }
    current->nextStatement = statement;
    return statements;
}

int MonthSemanticAction(int month){
    return month;
}

MonthBlock * StatementsMonthIntegerSemanticAction(int month, Statements * statements) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MonthBlock * monthBlock = calloc(1, sizeof(MonthBlock));
    if(month < 1 || month > 12){
        logError(_logger, "Unknown month number (%d)", month);
        return NULL;
    }
	monthBlock->month = month;
	monthBlock->statements = statements;
	return monthBlock;
}


MonthBlocks * SingleMonthBlockSemanticAction(MonthBlock * monthBlock) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    MonthBlocks * monthBlocks = calloc(1, sizeof(MonthBlocks));
    int index = monthBlock->month - 1;
    monthBlocks->months[index] = monthBlock;
    return monthBlocks;
}

MonthBlocks * AppendMonthBlockSemanticAction(MonthBlocks * monthBlocks, MonthBlock * monthBlock) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    // if (monthBlocks == NULL) {
    //     return SingleMonthBlockSemanticAction(monthBlock);
    // }
    int index = monthBlock->month - 1;
    if (monthBlocks->months[index] != NULL) {
        logError(_logger, "Duplicate month declaration in AppendMonthBlockSemanticAction");
		destroyMonthBlock(monthBlock);
        return monthBlocks;
    }
    monthBlocks->months[index] = monthBlock;
    return monthBlocks;
}

YearBlock * YearBlockSemanticAction(int year, MonthBlocks * monthBlocks) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	YearBlock * yearBlock = calloc(1, sizeof(YearBlock));
	yearBlock->year = year;
	yearBlock->monthBlocks = monthBlocks;
	return yearBlock;
}

ColorDef * DefineColorSemanticAction(char * identifier, char * hexColor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ColorDef * colorDef = calloc(1, sizeof(ColorDef));
	Color * color = calloc(1, sizeof(Color));
	color->name = identifier; //strdup
	color->hexValue = hexColor; //strdup
	colorDef->color = color;
	colorDef->next = NULL;
	return colorDef;
}

ColorList * SingleColorDefSemanticAction(ColorDef * colorDef) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ColorList * colorList = calloc(1, sizeof(ColorList));
    colorList->firstColorDef = colorDef;
    return colorList;
}

ColorList * AppendColorDefSemanticAction(ColorList * colorList, ColorDef * colorDef) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ColorDef * current = colorList->firstColorDef;
    while (current->next != NULL)
        current = current->next;

    current->next = colorDef;
    return colorList;
}

TimezoneDecl * CreateTimezoneSemanticAction(char * timezone) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TimezoneDecl * timezoneDecl = calloc(1, sizeof(TimezoneDecl));
	timezoneDecl->timezone = timezone; //strdup
	return timezoneDecl;
}

Header * createHeaderSemanticAction(TimezoneDecl * timezoneDecl, ColorList * colorList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
    Header * header = calloc(1, sizeof(Header));
    header->timezoneDecl = timezoneDecl;
    header->colorList = colorList;
    return header;
}

Program * CreateProgramSemanticAction(Header * header, YearBlock * yearBlock) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->header = header;
	program->yearBlock = yearBlock;
	_compilerState->abstractSyntaxtTree = program;
	return program;
}

