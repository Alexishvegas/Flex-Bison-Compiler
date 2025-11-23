#include "Generator.h"

/* PRIVATE FUNCTIONS DECLARATIONS*/
static char *html_escape(const char *s);
static const char* lookup_color_hex(ColorList *list, const char *id);
static bool generateCode(Program *prog, const char *outputFile);

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownGeneratorModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Generator...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeGeneratorModule() {
	_logger = createLogger("Generator");
	return _shutdownGeneratorModule;
}

/** PRIVATE FUNCTIONS */

// static char * _indentation(const unsigned int indentationLevel);
// static const char _expressionTypeToCharacter(const ExpressionType type);
// static void _generateConstant(const unsigned int indentationLevel, Constant * constant);
// static void _generateEpilogue(const int value);
// static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
// static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
// static void _generateProgram(Program * program);
// static void _generatePrologue(void);
// static void _output(const unsigned int indentationLevel, const char * const format, ...);

// static void _generateStatement(Statement * statement);

/**
 * Converts and expression type to the proper character of the operation
 * involved, or returns '\0' if that's not possible.
 */
// static const char _expressionTypeToCharacter(const ExpressionType type) {
// 	switch (type) {
// 		case ADDITION: return '+';
// 		case DIVISION: return '/';
// 		case MULTIPLICATION: return '*';
// 		case SUBTRACTION: return '-';
// 		default:
// 			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
// 			return '\0';
// 	}
// }

/**
 * Generates the output of a constant.
 */
// static void _generateConstant(const unsigned int indentationLevel, Constant * constant) {
// 	_output(indentationLevel, "%s", "[ $C$, circle, draw, black!20\n");
// 	_output(1 + indentationLevel, "%s%d%s", "[ $", constant->value, "$, circle, draw ]\n");
// 	_output(indentationLevel, "%s", "]\n");
// }

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
// static void _generateEpilogue(const int value) {
// 	_output(0, "%s%d%s",
// 		"            [ $", value, "$, circle, draw, blue ]\n"
// 		"        ]\n"
// 		"    \\end{forest}\n"
// 		"\\end{document}\n\n"
// 	);
// }

/**
 * Generates the output of an expression.
 */
// static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
// 	_output(indentationLevel, "%s", "[ $E$, circle, draw, black!20\n");
// 	switch (expression->type) {
// 		case ADDITION:
// 		case DIVISION:
// 		case MULTIPLICATION:
// 		case SUBTRACTION:
// 			_generateExpression(1 + indentationLevel, expression->leftExpression);
// 			_output(1 + indentationLevel, "%s%c%s", "[ $", _expressionTypeToCharacter(expression->type), "$, circle, draw, purple ]\n");
// 			_generateExpression(1 + indentationLevel, expression->rightExpression);
// 			break;
// 		case FACTOR:
// 			_generateFactor(1 + indentationLevel, expression->factor);
// 			break;
// 		default:
// 			logError(_logger, "The specified expression type is unknown: %d", expression->type);
// 			break;
// 	}
// 	_output(indentationLevel, "%s", "]\n");
// }

/**
 * Generates the output of a factor.
 */
// static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
// 	_output(indentationLevel, "%s", "[ $F$, circle, draw, black!20\n");
// 	switch (factor->type) {
// 		case CONSTANT:
// 			_generateConstant(1 + indentationLevel, factor->constant);
// 			break;
// 		case EXPRESSION:
// 			_output(1 + indentationLevel, "%s", "[ $($, circle, draw, purple ]\n");
// 			_generateExpression(1 + indentationLevel, factor->expression);
// 			_output(1 + indentationLevel, "%s", "[ $)$, circle, draw, purple ]\n");
// 			break;
// 		default:
// 			logError(_logger, "The specified factor type is unknown: %d", factor->type);
// 			break;
// 	}
// 	_output(indentationLevel, "%s", "]\n");
// }

/**
 * Generates the output of the program.
 */
// static void _generateProgram(Program * program) {
// 	if(program != NULL){
// 		// _generateStatement(program->firstStatement);
// 		_output(2, "ÉXITO");
// 	}
// }

// static void _generateStatement(Statement * statement){
// 	switch (statement->type){
// 			case STATEMENT_COLOR_DECLARATION: break;
// 			case STATEMENT_EVENT:
// 				_output(2, "ÉXITO: ARMAMOS EL EVENTO %s\n", statement->event->name != NULL ? statement->event->name : "error: name is null");
// 				break;
// 			case STATEMENT_REPLACE: break;
// 			default:
// 			logError(_logger, "The specified statement type is unknown: %d\n", statement->type);
// 			break;
// 		}
// }

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
// static void _generatePrologue(void) {
// 	_output(0, "%s",
// 		"\\documentclass{standalone}\n\n"
// 		"\\usepackage[utf8]{inputenc}\n"
// 		"\\usepackage[T1]{fontenc}\n"
// 		"\\usepackage{amsmath}\n"
// 		"\\usepackage{forest}\n"
// 		"\\usepackage{microtype}\n\n"
// 		"\\begin{document}\n"
// 		"    \\centering\n"
// 		"    \\begin{forest}\n"
// 		"        [ \\text{$=$}, circle, draw, purple\n"
// 	);
// }

/**
 * Generates an indentation string for the specified level.
 */
// static char * _indentation(const unsigned int level) {
// 	return indentation(_indentationCharacter, level, _indentationSize);
// }

// /**
//  * Outputs a formatted string to standard output. The "fflush" instruction
//  * allows to see the output even close to a failure, because it drops the
//  * buffering.
//  */
// static void _output(const unsigned int indentationLevel, const char * const format, ...) {
// 	va_list arguments;
// 	va_start(arguments, format);
// 	char * indentation = _indentation(indentationLevel);
// 	char * effectiveFormat = concatenate(2, indentation, format);
// 	vfprintf(stdout, effectiveFormat, arguments);
// 	fflush(stdout);
// 	free(effectiveFormat);
// 	free(indentation);
// 	va_end(arguments);
// }

// /** PUBLIC FUNCTIONS */

bool executeGenerator(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
    
	// _generatePrologue();
	//_generateProgram(compilerState->abstractSyntaxtTree);
	// _generateEpilogue(compilerState->value);
	logDebugging(_logger, "Generation is done.");
    return generateCode(compilerState->abstractSyntaxtTree, "index.html");
}

/* PRIVATE FUNCTIONS */
static bool generateCode(Program *prog, const char *outputFile) {
    if (!prog || !prog->yearBlock || !outputFile) return false;

    FILE *f = fopen(outputFile, "w");
    if (!f) {
        logError(_logger, "Could not open file '%s'", outputFile);
        return false;
    }

    YearBlock *yb = prog->yearBlock;
    int year = yb->year;

    /* Escribimos un HTML + CSS + JS simple. Los eventos los volcaremos en un array JS. */
    fprintf(f,
"<!doctype html>\n"
"<html lang=\"es\">\n"
"<head>\n"
"  <meta charset=\"utf-8\">\n"
"  <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\n"
"  <title>Calendario %d</title>\n"
"  <style>\n"
"    body{font-family:system-ui, -apple-system, \"Segoe UI\", Roboto, \"Helvetica Neue\", Arial; margin:20px}\n" 
"    .controls{display:flex; gap:10px; align-items:center; margin-bottom:10px}\n" 
"    .calendar{display:grid; grid-template-columns:repeat(7,1fr); gap:4px}\n"
"    .dayHeader{font-weight:600; text-align:center}\n" 
"    .cell{min-height:80px; border:1px solid #e5e7eb; padding:6px; background:#fff; border-radius:6px; position:relative}\n"
"    .cell .date{position:absolute; top:6px; right:8px; font-size:12px; color:#666}\n"
"    .events{margin-top:20px; display:flex; flex-direction:column; gap:4px}\n"
"    .event{padding:4px 6px; border-radius:6px; color:#fff; cursor:pointer; font-size:13px; overflow:hidden; text-overflow:ellipsis; white-space:nowrap}\n"
"    .navBtn{padding:6px 10px; border-radius:8px; border:1px solid #ddd; background:#f8f9fa; cursor:pointer}\n"
"    .monthTitle{font-size:20px; font-weight:700}\n"
"    /* modal */\n"
"    .modalBg{position:fixed; inset:0; display:none; align-items:center; justify-content:center; background:rgba(0,0,0,0.4)}\n"
"    .modal{background:#fff; padding:20px; border-radius:10px; max-width:600px; width:90%}\n" 
"    .closeBtn{float:right; cursor:pointer; font-weight:700}\n" 
"  </style>\n"
"</head>\n"
"<body>\n", year);

    /* Generamos el array JS con todos los eventos calculados */
    fprintf(f, "<script>\n");
    fprintf(f, "const YEAR = %d;\n", year);
    fprintf(f, "const COLORS = {};\n");
    /* Dump colors mapping */
    ColorDef *cd = (prog->header && prog->header->colorList) ? prog->header->colorList->firstColorDef : NULL;
    while (cd) {
        if (cd->color && cd->color->name && cd->color->hexValue) {
            /* Asegurar escape en JS string */
            char *nameEsc = html_escape(cd->color->name);
            char *hexEsc = html_escape(cd->color->hexValue);
            fprintf(f, "COLORS[\"%s\"] = \"%s\";\n", cd->color->name, cd->color->hexValue); /* placeholder to keep structure */
            /* we'll write properly below to avoid duplication - but simpler: write as JS object literal */
            free(nameEsc); 
            free(hexEsc);
        }
        cd = cd->next;
    }
    /* Simpler: create a JS object literal by iterating again */
    fprintf(f, "const COLOR_MAP = {\n");
    cd = (prog->header && prog->header->colorList) ? prog->header->colorList->firstColorDef : NULL;
    int firstColor = 1;
    while (cd) {
        if (cd->color && cd->color->name && cd->color->hexValue) {
            char *nameEsc = html_escape(cd->color->name);
            char *hexEsc = html_escape(cd->color->hexValue);
            if (!firstColor) fprintf(f, ",\n");
            fprintf(f, "  \"%s\": \"%s\"", nameEsc, hexEsc);
            firstColor = 0;
            free(nameEsc);
            free(hexEsc);
        }
        cd = cd->next;
    }
    fprintf(f, "\n};\n");

    /* Empezamos array de eventos */
    fprintf(f, "const EVENTS = [\n");

    /* Recorremos los monthBlocks y statements para generar entradas concretas */
    if (yb->monthBlocks) {
        for (int mi = 0; mi < 12; ++mi) {
            MonthBlock *mb = yb->monthBlocks->months[mi];
            if (!mb) continue;
            int month = mb->month; /* asumo 1..12 */
            Statements *stmts = mb->statements;
            if (!stmts) continue;
            Statement *s = stmts->firstStatement;
            while (s) {
                if (s->type == STATEMENT_EVENT && s->eventDecl) {
                    EventDecl *ed = s->eventDecl;
                    EventSpec *es = ed->eventSpec;
                    EventBody *eb = ed->eventBody;
                    const char *colorHex = eb && eb->colorId ? lookup_color_hex(prog->header ? prog->header->colorList : NULL, eb->colorId) : "#888888";
                    char *idEsc = html_escape(ed->identifier ? ed->identifier : "");
                    char *descEsc = html_escape(eb && eb->description ? eb->description : "");
                    char *urlEsc = html_escape(eb && eb->url ? eb->url : "");
                    if (es->type == SPEC_DAYOFMONTH) {
                        int day = es->dayOfMonth;
                        /* emit single event */
                        char startS[16] = "";
                        char endS[16] = "";
                        timeToString(es->start, startS, sizeof(startS));
                        timeToString(es->end, endS, sizeof(endS));
                        fprintf(f, "  { id: \"%s\", color: \"%s\", description: \"%s\", url: \"%s\", date: \"%04d-%02d-%02d\", start: \"%s\", end: \"%s\" },\n",
                                idEsc, colorHex, descEsc, urlEsc, year, month, day,
                                startS, endS);
                    } else if (es->type == SPEC_DAYLIST && es->dayList) {
                        DayList *dl = es->dayList;
                        /* Para cada día del mes, chequeo si su weekday está en la lista y emito evento */
                        int dim = daysInMonth(year, month);
                        for (int d = 1; d <= dim; ++d) {
                            int wd = normalizeWeekday(weekday(year, month, d)); /* 0..6 */
                            /* verificar si wd está en dl->days[] */
                            int present = 0;
                            for (int k=0;k<dl->count;++k) {
                                if (dl->days[k] == wd) { present = 1; break; }
                            }
                            if (present) {
                                char startS[16] = "";
                                char endS[16] = "";
                                timeToString(es->start, startS, sizeof(startS));
                                timeToString(es->end, endS, sizeof(endS));
                                fprintf(f, "  { id: \"%s\", color: \"%s\", description: \"%s\", url: \"%s\", date: \"%04d-%02d-%02d\", start: \"%s\", end: \"%s\" },\n",
                                    idEsc, colorHex, descEsc, urlEsc, year, month, d,
                                    startS, endS);
                            }
                        }
                    }
                    free(idEsc);
                    free(descEsc);
                    free(urlEsc);
                }
                s = s->nextStatement;
            }
        }
    }

    fprintf(f, "];\n"); /* end EVENTS array */

    /* Script para renderizar calendario y modal */
    fprintf(f,
"/* UTILIDADES JS */\n"
"function firstDayOfMonth(y,m){ return new Date(y,m-1,1).getDay(); /* 0..6 (lun..sab) */ }\n"
"function daysInMonth(y,m){ return new Date(y,m,0).getDate(); }\n"
"\n"
"let currentYear = YEAR;\n"
"let currentMonth = (new Date()).getMonth() + 1; /* 1..12 */\n"
"\n"
"function renderCalendar(year, month){\n"
"  document.getElementById('monthTitle').textContent = year + ' - ' + month;\n"
"  const grid = document.getElementById('calGrid');\n"
"  grid.innerHTML = '';\n"
"  const startWeekday = firstDayOfMonth(year, month);\n"
"  const dim = daysInMonth(year, month);\n"
"  // encabezados\n"
"  const weekNames = ['Lun','Mar','Mié','Jue','Vie','Sáb','Dom'];\n"
"  const headerRow = document.createElement('div');\n"
"  headerRow.style.gridColumn = '1 / -1';\n"
"  for(let i=0;i<7;i++){ const h = document.createElement('div'); h.className='dayHeader'; h.textContent = weekNames[i]; grid.appendChild(h);} \n"
"  // celdas vacías antes del primer día\n"
"  for(let i=0;i<startWeekday;i++){ const empty = document.createElement('div'); grid.appendChild(empty); }\n"
"  // dias\n"
"  for(let d=1; d<=dim; ++d){\n"
"    const cell = document.createElement('div'); cell.className='cell';\n"
"    const dateSpan = document.createElement('div'); dateSpan.className='date'; dateSpan.textContent = d; cell.appendChild(dateSpan);\n"
"    const evcontainer = document.createElement('div'); evcontainer.className='events';\n"
"    // buscar eventos en EVENTS con date YYYY-MM-DD\n"
"    const dd = (''+year+'-'+String(month).padStart(2,'0')+'-'+String(d).padStart(2,'0'));\n"
"    for(const ev of EVENTS){ if(ev.date === dd){ const e = document.createElement('div'); e.className='event'; e.textContent = ev.id; e.style.background = ev.color || '#888';\n"
"        e.onclick = ()=> openModal(ev);\n"
"        evcontainer.appendChild(e);\n"
"    }}\n"
"    cell.appendChild(evcontainer);\n"
"    grid.appendChild(cell);\n"
"  }\n"
"}\n"
"\n"
"function openModal(ev){\n"
"  const modalBg = document.getElementById('modalBg');\n"
"  const modal = document.getElementById('modal');\n"
"  document.getElementById('m_title').textContent = ev.id;\n"
"  document.getElementById('m_date').textContent = ev.date;\n"
"  document.getElementById('m_time').textContent = (ev.start?ev.start:'') + (ev.end?(' - '+ev.end):'');\n"
"  document.getElementById('m_desc').innerHTML = ev.description || '';\n" 
"  const linkNode = document.getElementById('m_link');\n"
"  if(ev.url && ev.url.length>0){ linkNode.innerHTML = '<a href=\"'+ev.url+'\" target=\"_blank\">Abrir enlace</a>'; } else { linkNode.innerHTML = ''; }\n"
"  modalBg.style.display = 'flex';\n"
"}\n"
"function closeModal(){ document.getElementById('modalBg').style.display='none'; }\n"
"\n"
"document.addEventListener('DOMContentLoaded', ()=>{\n"
"  document.getElementById('prevBtn').addEventListener('click', ()=>{ currentMonth--; if(currentMonth<1){ currentMonth=12; currentYear--; } renderCalendar(currentYear,currentMonth); });\n"
"  document.getElementById('nextBtn').addEventListener('click', ()=>{ currentMonth++; if(currentMonth>12){ currentMonth=1; currentYear++; } renderCalendar(currentYear,currentMonth); });\n"
"  document.getElementById('modalBg').addEventListener('click', (e)=>{ if(e.target.id==='modalBg') closeModal(); });\n"
"  renderCalendar(currentYear,currentMonth);\n"
"});\n"
"</script>\n");

    /* Body HTML with container, calendar grid and modal */
    fprintf(f,
"<div class=\"controls\">\n"
"  <button id=\"prevBtn\" class=\"navBtn\">◀</button>\n"
"  <div class=\"monthTitle\" id=\"monthTitle\"></div>\n" 
"  <button id=\"nextBtn\" class=\"navBtn\">▶</button>\n"
"</div>\n"
"<div id=\"calGrid\" class=\"calendar\"></div>\n"
"\n"
"<div id=\"modalBg\" class=\"modalBg\">\n"
"  <div id=\"modal\" class=\"modal\">\n"
"    <div><span id=\"m_title\" style=\"font-weight:700\"></span> <span class=\"closeBtn\" onclick=\"closeModal()\">✕</span></div>\n" 
"    <div style=\"margin-top:8px\"><strong>Fecha:</strong> <span id=\"m_date\"></span></div>\n" 
"    <div><strong>Horario:</strong> <span id=\"m_time\"></span></div>\n" 
"    <div style=\"margin-top:8px\" id=\"m_desc\"></div>\n" 
"    <div id=\"m_link\" style=\"margin-top:8px\"></div>\n" 
"  </div>\n"
"</div>\n"
"</body>\n</html>\n");

    fclose(f);
    /* mensaje opcional en stderr */
    logDebugging(_logger, "HTML file successfully generated in: %s\n", outputFile);
    return true;
}


/* escape básico para insertar texto en HTML (muy simple) */
static char *html_escape(const char *s) {
    if (!s) return strdup("");
    size_t len = strlen(s);
    /* reserva un buffer grande (factor 6 por seguridad: & -> &amp; etc) */
    size_t maxlen = len * 6 + 1;
    char *out = (char*)malloc(maxlen);
    char *p = out;
    for (; *s; ++s) {
        switch(*s) {
            case '&': strcpy(p, "&amp;"); p += 5; break;
            case '<': strcpy(p, "&lt;"); p += 4; break;
            case '>': strcpy(p, "&gt;"); p += 4; break;
            case '"': strcpy(p, "&quot;"); p += 6; break;
            case '\'': strcpy(p, "&#39;"); p += 5; break;
            default: *p++ = *s;
        }
    }
    *p = '\0';
    return out;
}

/* busca color hex por id en la lista de colores del header, devuelve "#xxxxxx" o "#888888" por defecto */
static const char* lookup_color_hex(ColorList *list, const char *id) {
    if (!list || !id) return "#888888";
    ColorDef *c = list->firstColorDef;
    while (c) {
        if (c->color && c->color->name && strcmp(c->color->name, id) == 0) {
            return c->color->hexValue ? c->color->hexValue : "#888888";
        }
        c = c->next;
    }
    return "#888888";
}