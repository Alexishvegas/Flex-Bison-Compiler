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
        if(colorExists(id, cData->hexColor)){
            logError(logger, "Color collision: %s or %s already declared as color", id, cData->hexColor);
            return false;
        }
    }else if(type == ENTITY_EVENT){
        if(eventOverlap(id, (EventData*) data)){
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
        if(table->symbols[i].type == ENTITY_COLOR){
            ColorData * cData = (ColorData*) table->symbols[i].data;
            if (strcmp(table->symbols[i].id, id) == 0 || strcmp(cData->hexColor, hexColor == 0)) {
                return true;
            }
        }
    }
    return false;
}

static bool eventOverlap(char * id, EventData * data){
    for(int i=0; i < table->count; i++){
        if(table->symbols[i].type == ENTITY_EVENT){
            EventData * current = (EventData*) table->symbols[i].data;
            if(current->year != data->year || current->month != data->month){
                return false;
            }
            if(strcmp(table->symbols[i].id, id) == 0){
                logError(logger, "Event collision: '%s' already declared as event this month", id);
                return true;
            }
            // Primero verificamos si los intervalos horarios se superponen
            if (compareTime(data->start, current->end) < 0 &&
                compareTime(current->start, data->end) < 0) {
                // Ahora debemos verificar si ocurre en al menos un mismo día
                DayNumber *d1 = data->days;
                while (d1 != NULL) {
                    DayNumber *d2 = current->days;
                    while (d2 != NULL) {
                        if (d1->day == d2->day) {
                            char * s1 = toStringEventData(id, data->year, data->month, d1->day, data);
                            char * s2 = toStringEventData(table->symbols[i].id, current->year, current->month, d2->day, current);
                            logError(logger, "Event collision: Event overlapping %s and %s", s1, s2);
                            free(s1);
                            free(s2);
                            return true; // Se superponen en horario y día
                        }
                        d2 = d2->next;
                    }
                    d1 = d1->next;
                }
            }

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


char *toStringEventData(char *id, int year, int month, int day, EventData *data) {
    if (id == NULL || data == NULL) return NULL;

    char startBuf[6];
    char endBuf[6];

    // Convertir horarios a HH:MM
    snprintf(startBuf, sizeof(startBuf), "%02d:%02d", data->start->hour, data->start->minute);
    snprintf(endBuf, sizeof(endBuf), "%02d:%02d", data->end->hour, data->end->minute);

    // Crear buffer final
    char *buffer = malloc(128);
    if (buffer == NULL) return NULL;

    snprintf(
        buffer,
        128,
        "['%s' on %d-%d-%d from %s to %s]",
        id,
        year,
        month,
        day,
        startBuf,
        endBuf
    );

    return buffer;
}

