#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef struct Symbol Symbol;
typedef struct SymbolTable SymbolTable;


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

void initSymbolTable(SymbolTable *table);
void addSymbol(SymbolTable *table, char *id, EntityType type, void *data);
Symbol *findSymbol(SymbolTable *table, char *id);
void freeSymbolTable(SymbolTable *table);

#endif