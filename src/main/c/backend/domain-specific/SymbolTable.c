#include "SymbolTable.h"
#include "../../support/logging/Logger.h"
#include "../../support/language/String.h"
#include <stdlib.h>
#include <string.h>

static Logger *logger = NULL;
static SymbolTable * table = NULL;

void initSymbolTable(SymbolTable *symbolTable) {
    logger = createLogger("SymbolTable initialized");
    table = symbolTable;
    table->count = 0;
    table->capacity = 100;
    table->symbols = calloc(table->capacity, sizeof(Symbol));
}

bool addSymbol(char *id, EntityType type, void *data) {
    if(type == ENTITY_COLOR){
        ColorData * cData = (ColorData*) data;
        char * hexColor = cData->hexColor;
        if(colorExists(id, hexColor)){
            logError(logger, "Color collision: %s or %s already declared as color", id, hexColor);
            return false;
        }
    }
    if (table->count >= table->capacity) {
        table->capacity *= 2;
        table->symbols = realloc(table->symbols, table->capacity * sizeof(Symbol));
    }
    Symbol *symbol = &table->symbols[table->count++];
    symbol->id = strdup(id);
    symbol->type = type;
    symbol->data = data;
    logDebugging(logger, "Added symbol: %s (type: %d)", id, type);
    return true;
}

static bool colorExists(char * id, char * hexColor) {
    for (int i = 0; i < table->count; i++) {
        ColorData * cData = (ColorData*) table->symbols[i].data;
        char * hc = cData->hexColor;
        if (strcmp(table->symbols[i].id, id) == 0 || strcmp(hc, hexColor)) {
            return true;
        }
    }
    return false;
}

Symbol *findSymbol(char *id) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].id, id) == 0) {
            return &table->symbols[i];
        }
    }
    logWarning(logger, "Symbol not found: %s", id);
    return NULL;
}

void freeSymbolTable() {
    for (int i = 0; i < table->count; i++) {
        free(table->symbols[i].id);
        if (table->symbols[i].type == ENTITY_COLOR) {
            ColorData * data = (ColorData *) table->symbols[i].data;
            free(data->hexColor);
            free(data);
        } else if (table->symbols[i].type == ENTITY_EVENT) {
            EventData * data = (EventData *) table->symbols[i].data;
            free(data->start);
            free(data->end);
            DayNumber * current = data->days;
            while(current != NULL){
                DayNumber * next = current->next;
                free(current);
                current = next;
            }
            free(data);
        } 
    }
    free(table->symbols);
    table->symbols = NULL;
    table->count = 0;
    table->capacity = 0;
    destroyLogger(logger);
}