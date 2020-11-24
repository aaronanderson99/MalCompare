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

#include "tokens.h"

#include <map>

void print_long_as_tokens(std::ostream &out, unsigned long feature,
		unsigned int length) {
	const char *char_feature = (const char *) &feature;
	for (unsigned int i = length; i >= 1; i--)
		out << print_token((TokenType) char_feature[i - 1]) << ' ';
}

const char *print_token(enum TokenType tt) {
	static bool first_run = true;
	static std::map<TokenType, const char *> tokenmap;

	if (first_run) {
		tokenmap[JS_TOK_ERROR] = "ERR";
		tokenmap[JS_TOK_EOF] = "EOF";
		tokenmap[JS_TOK_EOL] = "EOL";
		tokenmap[JS_TOK_SEMI] = ";";
		tokenmap[JS_TOK_COMMA] = ",";
		tokenmap[JS_TOK_ASSIGN] = "ASSIGN";
		tokenmap[JS_TOK_HOOK] = "CONDITIONAL?";
		tokenmap[JS_TOK_COLON] = "CONDITIONAL:";
		tokenmap[JS_TOK_OR] = "OR";
		tokenmap[JS_TOK_AND] = "AND";
		tokenmap[JS_TOK_BITOR] = "BITOR";
		tokenmap[JS_TOK_BITXOR] = "BITXOR";
		tokenmap[JS_TOK_BITAND] = "BITAND";
		tokenmap[JS_TOK_EQOP] = "(NON)EQUALITY";
		tokenmap[JS_TOK_RELOP] = "RELATION";
		tokenmap[JS_TOK_SHOP] = "SHIFT";
		tokenmap[JS_TOK_PLUS] = "+";
		tokenmap[JS_TOK_MINUS] = "-";
		tokenmap[JS_TOK_STAR] = "MULT";
		tokenmap[JS_TOK_DIVOP] = "DIV";
		tokenmap[JS_TOK_UNARYOP] = "UNARY_PREFIX";
		tokenmap[JS_TOK_INC] = "++";
		tokenmap[JS_TOK_DEC] = "--";
		tokenmap[JS_TOK_DOT] = ".";
		tokenmap[JS_TOK_LB] = "[";
		tokenmap[JS_TOK_RB] = "]";
		tokenmap[JS_TOK_LC] = "{";
		tokenmap[JS_TOK_RC] = "}";
		tokenmap[JS_TOK_LP] = "(";
		tokenmap[JS_TOK_RP] = ")";
		tokenmap[JS_TOK_NAME] = "NAME";
		tokenmap[JS_TOK_NUMBER] = "NUM";
		tokenmap[JS_TOK_STRING] = "STR";
		tokenmap[JS_TOK_REGEXP] = "REGEXP";
		tokenmap[JS_TOK_PRIMARY] = "PRIMARY_KW";
		tokenmap[JS_TOK_FUNCTION] = "FUN";
		tokenmap[JS_TOK_IF] = "IF";
		tokenmap[JS_TOK_ELSE] = "ELSE";
		tokenmap[JS_TOK_SWITCH] = "SWITCH";
		tokenmap[JS_TOK_CASE] = "CASE";
		tokenmap[JS_TOK_DEFAULT] = "DEFAULT";
		tokenmap[JS_TOK_WHILE] = "WHILE";
		tokenmap[JS_TOK_DO] = "DO";
		tokenmap[JS_TOK_FOR] = "FOR";
		tokenmap[JS_TOK_BREAK] = "BREAK";
		tokenmap[JS_TOK_CONTINUE] = "CONTINUE";
		tokenmap[JS_TOK_IN] = "IN";
		tokenmap[JS_TOK_VAR] = "VAR";
		tokenmap[JS_TOK_WITH] = "WITH";
		tokenmap[JS_TOK_RETURN] = "RETURN";
		tokenmap[JS_TOK_NEW] = "NEW";
		tokenmap[JS_TOK_DELETE] = "DELETE";
		tokenmap[JS_TOK_DEFSHARP] = "DEFSHARP";
		tokenmap[JS_TOK_USESHARP] = "USESHARP";
		tokenmap[JS_TOK_TRY] = "TRY";
		tokenmap[JS_TOK_CATCH] = "CATCH";
		tokenmap[JS_TOK_FINALLY] = "FINALLY";
		tokenmap[JS_TOK_THROW] = "THROW";
		tokenmap[JS_TOK_INSTANCEOF] = "INSTANCEOF";
		tokenmap[JS_TOK_DEBUGGER] = "DEBUGGER";
		tokenmap[JS_TOK_XMLSTAGO] = "XMLSTAGO";
		tokenmap[JS_TOK_XMLETAGO] = "XMLETAGO";
		tokenmap[JS_TOK_XMLPTAGC] = "XMLPTAGC";
		tokenmap[JS_TOK_XMLATTR] = "XMLATTR";
		tokenmap[JS_TOK_XMLSPACE] = "XMLSPACE";
		tokenmap[JS_TOK_XMLTEXT] = "XMLTEXT";
		tokenmap[JS_TOK_XMLCOMMENT] = "XMLCOMMENT";
		tokenmap[JS_TOK_XMLCDATA] = "XMLCDATA";
		tokenmap[JS_TOK_XMLPI] = "XMLPI";
		tokenmap[JS_TOK_AT] = "AT";
		tokenmap[JS_TOK_DBLCOLON] = "XML::";
		tokenmap[JS_TOK_ANYNAME] = "ANYNAME";
		tokenmap[JS_TOK_DBLDOT] = "XML..";
		tokenmap[JS_TOK_FILTER] = "XMLFILTER";
		tokenmap[JS_TOK_XMLELEM] = "XMLELEM";
		tokenmap[JS_TOK_XMLLIST] = "XMLLIST";
		tokenmap[JS_TOK_YIELD] = "YIELD";
		tokenmap[JS_TOK_ARRAYCOMP] = "ARRAYCOMP";
		tokenmap[JS_TOK_ARRAYPUSH] = "ARRAYPUSH";
		tokenmap[JS_TOK_LEXICALSCOPE] = "LEXICALSCOPE";
		tokenmap[JS_TOK_LET] = "LET";
		tokenmap[JS_TOK_SEQ] = "SEQ";
		tokenmap[JS_TOK_FORHEAD] = "FOR_LOOP_HEAD";
		tokenmap[JS_TOK_ARGSBODY] = "ARGSBODY";
		tokenmap[JS_TOK_UPVARS] = "UPVARS";
		tokenmap[JS_TOK_RESERVED] = "RESERVED";
		tokenmap[JS_TOK_LIMIT] = "LIMIT";
		tokenmap[JS_TOK_STR_10] = "STR<10";
		tokenmap[JS_TOK_STR_100] = "STR<100";
		tokenmap[JS_TOK_STR_1000] = "STR<1000";
		tokenmap[JS_TOK_STR_10000] = "STR<10000";
		tokenmap[JS_TOK_STR_UNBOUND] = "STR>10000";
		tokenmap[JS_TOK_UNESCAPE] = "UNESCAPE";
		tokenmap[JS_TOK_SETTIMEOUT] = "SETTIMEOUT";
		tokenmap[JS_TOK_FROMCHARCODE] = "FROMCHARCODE";
		tokenmap[JS_TOK_EVAL] = "EVAL";
		tokenmap[JS_TOK_ERR] = "ERR";
		first_run = false;
	}

	return tokenmap[tt];
}

