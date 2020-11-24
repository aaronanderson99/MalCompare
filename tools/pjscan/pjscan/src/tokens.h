/*
 * Copyright 2012 Nedim Srndic, University of Tuebingen
 *
 * This file is part of pjscan.

 * pjscan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pjscan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pjscan.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  tokens.h
 *  Created on: Jun 3, 2011
 */

#ifndef TOKENS_H_
#define TOKENS_H_

#include <iostream>

/* SpiderMonkey token constant for errors */
const int SM_TOK_ERROR = -1;
/* SpiderMonkey token constant for EOF */
const int SM_TOK_EOF = 0;
/* Our token constant for errors, must be 0 <= x <= 255 */
const int TOK_ERROR = 200;

/*
 * SpiderMonkey lexical token constants, as returned by the JavaScript
 * tokenizer
*/
enum TokenType {
	JS_TOK_ERROR = -1,
	JS_TOK_EOF = 0,
	JS_TOK_EOL = 1,
	JS_TOK_SEMI = 2,
	JS_TOK_COMMA = 3,
	JS_TOK_ASSIGN = 4,
	JS_TOK_HOOK = 5,
	JS_TOK_COLON = 6,
	JS_TOK_OR = 7,
	JS_TOK_AND = 8,
	JS_TOK_BITOR = 9,
	JS_TOK_BITXOR = 10,
	JS_TOK_BITAND = 11,
	JS_TOK_EQOP = 12,
	JS_TOK_RELOP = 13,
	JS_TOK_SHOP = 14,
	JS_TOK_PLUS = 15,
	JS_TOK_MINUS = 16,
	JS_TOK_STAR = 17,
	JS_TOK_DIVOP = 18,
	JS_TOK_UNARYOP = 19,
	JS_TOK_INC = 20,
	JS_TOK_DEC = 21,
	JS_TOK_DOT = 22,
	JS_TOK_LB = 23,
	JS_TOK_RB = 24,
	JS_TOK_LC = 25,
	JS_TOK_RC = 26,
	JS_TOK_LP = 27,
	JS_TOK_RP = 28,
	JS_TOK_NAME = 29,
	JS_TOK_NUMBER = 30,
	JS_TOK_STRING = 31,
	JS_TOK_REGEXP = 32,
	JS_TOK_PRIMARY = 33,
	JS_TOK_FUNCTION = 34,
	JS_TOK_IF = 35,
	JS_TOK_ELSE = 36,
	JS_TOK_SWITCH = 37,
	JS_TOK_CASE = 38,
	JS_TOK_DEFAULT = 39,
	JS_TOK_WHILE = 40,
	JS_TOK_DO = 41,
	JS_TOK_FOR = 42,
	JS_TOK_BREAK = 43,
	JS_TOK_CONTINUE = 44,
	JS_TOK_IN = 45,
	JS_TOK_VAR = 46,
	JS_TOK_WITH = 47,
	JS_TOK_RETURN = 48,
	JS_TOK_NEW = 49,
	JS_TOK_DELETE = 50,
	JS_TOK_DEFSHARP = 51,
	JS_TOK_USESHARP = 52,
	JS_TOK_TRY = 53,
	JS_TOK_CATCH = 54,
	JS_TOK_FINALLY = 55,
	JS_TOK_THROW = 56,
	JS_TOK_INSTANCEOF = 57,
	JS_TOK_DEBUGGER = 58,
	JS_TOK_XMLSTAGO = 59,
	JS_TOK_XMLETAGO = 60,
	JS_TOK_XMLPTAGC = 61,
	JS_TOK_XMLATTR = 64,
	JS_TOK_XMLSPACE = 65,
	JS_TOK_XMLTEXT = 66,
	JS_TOK_XMLCOMMENT = 67,
	JS_TOK_XMLCDATA = 68,
	JS_TOK_XMLPI = 69,
	JS_TOK_AT = 70,
	JS_TOK_DBLCOLON = 71,
	JS_TOK_ANYNAME = 72,
	JS_TOK_DBLDOT = 73,
	JS_TOK_FILTER = 74,
	JS_TOK_XMLELEM = 75,
	JS_TOK_XMLLIST = 76,
	JS_TOK_YIELD = 77,
	JS_TOK_ARRAYCOMP = 78,
	JS_TOK_ARRAYPUSH = 79,
	JS_TOK_LEXICALSCOPE = 80,
	JS_TOK_LET = 81,
	JS_TOK_SEQ = 82,
	JS_TOK_FORHEAD = 83,
	JS_TOK_ARGSBODY = 84,
	JS_TOK_UPVARS = 85,
	JS_TOK_RESERVED = 86,
	JS_TOK_LIMIT = 87,
	JS_TOK_STR_10 = 101,
	JS_TOK_STR_100 = 102,
	JS_TOK_STR_1000 = 103,
	JS_TOK_STR_10000 = 104,
	JS_TOK_STR_UNBOUND = 105,
	JS_TOK_UNESCAPE = 120,
	JS_TOK_SETTIMEOUT = 121,
	JS_TOK_FROMCHARCODE = 122,
	JS_TOK_EVAL = 123,
	JS_TOK_ERR = 200
};

/*
 * Pretty-prints a token.
 */
const char *print_token(enum TokenType tt);

/*
 * Pretty-prints a variable of type unsigned long as tokens.
 */
void print_long_as_tokens(std::ostream &out, unsigned long feature,
		unsigned int length);

#endif /* TOKENS_H_ */
