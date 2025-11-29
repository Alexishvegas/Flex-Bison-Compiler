#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../support/utils/TimeUtils.h"

typedef struct Symbol Symbol;
typedef struct SymbolTable SymbolTable;
typedef struct ColorData ColorData;
typedef struct EventData EventData;
typedef struct DayNumber DayNumber;

struct Symbol{
    char *id;
    EntityType type;
    void *data;
};

struct SymbolTable {
    Symbol *symbols;
    int count;
    int capacity;
};

struct ColorData {
    char * hexColor;
};

struct EventData {
    int year; 
    int month; 
    Time * start;
    Time * end;
    DayNumber * days;
    EventDecl * ptr;
};

struct DayNumber {
    int day;
    DayNumber * next;
};

void initSymbolTable(SymbolTable * symbolTable);
bool addSymbol(char *id, EntityType type, void *data);
Symbol *findSymbol(char *id);
void freeSymbolTable();

#endif