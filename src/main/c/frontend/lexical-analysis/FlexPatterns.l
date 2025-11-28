%top{

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/TokenLabel.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"

}

%{

/**
 * @see https://westes.github.io/flex/manual/Definitions-Section.html#Definitions-Section
 */


extern CompilationStatus EOFLexemeAction();
extern CompilationStatus IgnoredLexemeAction();
extern CompilationStatus IntegerLexemeAction();
extern CompilationStatus UnknownLexemeAction();
extern CompilationStatus TokenLexemeAction(TokenLabel label);
extern CompilationStatus StringValueLexemeAction(TokenLabel label);
extern CompilationStatus StringLexemeAction();
extern CompilationStatus TimeLexemeAction();
extern CompilationStatus DateLexemeAction();
extern CompilationStatus WeekdayLexemeAction(TokenLabel label, int weekday);
extern CompilationStatus MonthLexemeAction(TokenLabel label, int month);

%}

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x IMPORT_EXPRESSION
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
whitespace			[[:space:]]+
digit               [0-9]
letter              [A-Za-z]
identifier          {letter}({letter}|{digit}|[_\-])*
time                {digit}{digit}:{digit}{digit}
date                [0-9]{4}-[0-9]{2}-[0-9]{2}
timezone_name       ([A-Z]{1,5}[\+\-][0-9]{1,2})
url_pattern         https?:\/\/[A-Za-z0-9\.\-_/]+
string              \"([^\"\\]|\\.)*\"
hex_value           #[0-9A-Fa-f]{6}

%%

%{

/**
 * @see https://westes.github.io/flex/manual/Rules-Section.html#Rules-Section
 */

%}


"{"                                 { return TokenLexemeAction(LBRACE); }
"}"                                 { return TokenLexemeAction(RBRACE); }

"test"              { return TokenLexemeAction(TEST); }

"year"                              { return TokenLexemeAction(YEAR); }
"month"                             { return TokenLexemeAction(MONTH); }
"timezone"                          { return TokenLexemeAction(TIMEZONE); }

"event"                             { return TokenLexemeAction(EVENT); }
"override"                          { return TokenLexemeAction(OVERRIDE); }

"define"                            { return TokenLexemeAction(DEFINE); }

"on"                                { return TokenLexemeAction(ON); }
"and"                               { return TokenLexemeAction(AND); }
"from"                              { return TokenLexemeAction(FROM); }
"to"                                { return TokenLexemeAction(TO); }
"at"                                { return TokenLexemeAction(AT); }
"since"                             { return TokenLexemeAction(SINCE); }
"till"                              { return TokenLexemeAction(TILL); }

"every"                             { return TokenLexemeAction(EVERY); }
"starting"                          { return TokenLexemeAction(STARTING); }
"only"                              { return TokenLexemeAction(ONLY); }
"if"                                { return TokenLexemeAction(IF); }

"color"                             { return TokenLexemeAction(COLOR); }
"description"                       { return TokenLexemeAction(DESCRIPTION); }
"url"                               { return TokenLexemeAction(URL);}

"Monday"                            { return WeekdayLexemeAction(MONDAY, 1); }
"Tuesday"                           { return WeekdayLexemeAction(TUESDAY, 2); }
"Wednesday"                         { return WeekdayLexemeAction(WEDNESDAY, 3); }
"Thursday"                          { return WeekdayLexemeAction(THURSDAY, 4); }
"Friday"                            { return WeekdayLexemeAction(FRIDAY, 5); }
"Saturday"                          { return WeekdayLexemeAction(SATURDAY, 6); }
"Sunday"                            { return WeekdayLexemeAction(SUNDAY, 7); }

"January"                           { return MonthLexemeAction(JANUARY, 1); }
"February"                          { return MonthLexemeAction(FEBRUARY, 2); }
"March"                             { return MonthLexemeAction(MARCH, 3); }
"April"                             { return MonthLexemeAction(APRIL, 4); }
"May"                               { return MonthLexemeAction(MAY, 5); }
"June"                              { return MonthLexemeAction(JUNE, 6); }
"July"                              { return MonthLexemeAction(JULY, 7); }
"August"                            { return MonthLexemeAction(AUGUST, 8); }
"September"                         { return MonthLexemeAction(SEPTEMBER, 9); }
"October"                           { return MonthLexemeAction(OCTOBER, 10); }
"November"                          { return MonthLexemeAction(NOVEMBER, 11); }
"December"                          { return MonthLexemeAction(DECEMBER, 12); }

{time}                              { return TimeLexemeAction(); }
{date}                              { return DateLexemeAction(); }  //revisar, misma (por ahora no se usa)

{timezone_name}                     { return StringValueLexemeAction(TIMEZONE_NAME); }

{url_pattern}                       { return StringValueLexemeAction(URL_PATTERN); } //URL_PATTERN

{string}                            { return StringLexemeAction(); }

{hex_value}                         { return StringValueLexemeAction(HEXVALUE); }

{identifier}                        { return StringValueLexemeAction(IDENTIFIER); }

[[:digit:]]+						{ return IntegerLexemeAction(); }

{whitespace}						{ return IgnoredLexemeAction(); }
.									{ return UnknownLexemeAction(); }

<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"
