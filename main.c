#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

struct Includes {
	int stdio;
	int stdlib;
	int string;
}includes = {
	0, 0, 0
};

int isAlpha(char c) {
	if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) || c == '.' || c == '&') {
		return 1;
	}

	return 0;
}

int skipChar(char c) {
	if (c == ' ' || c == '\n' || c == '\t' || c == ',') {return 1;}
	return 0;
}

// Main lexer. Lexes a string from a given point.
int lex(struct Token *reading, char string[], int c) {
	while (1) {
		if (string[c] == '/' && string[c + 1] == '/') {
			c += 2;
			while (string[c] != '\n') {
				c++;
			}

			c++;
		} else if (skipChar(string[c])) {
			while (skipChar(string[c])) {
				c++;
			}
		} else {
			break;
		}
	}
	
	reading->type = 0;
	reading->value = 0;

	switch (string[c++]) {
	case '\0':
		reading->type = FILE_END;
		goto end;
	case '{':
		reading->type = '{';
		goto end;
	case '}':
		reading->type = '}';
		goto end;
	case '+':
		reading->type = '+'; 
		goto end;
	case '*':
		reading->type = '*';
		goto end;
	case '(':
		reading->type = '(';
		goto end;
	case ')':
		reading->type = ')';
		goto end;
	case ';':
		reading->type = ';';
		goto end;
	case '=':
		reading->type = '=';
		goto end;
	case '!':
		reading->type = '!';
		goto end;
	}

	// Nothing matched, go back
	c--;

	// Lex text (function names)
	while (isAlpha(string[c])) {
		reading->type = TEXT;
		reading->string[reading->value] = string[c];
		reading->value++;
		c++;
	}

	// Lex strings
	if (string[c] == '\"') {
		c++;
		while (string[c] != '\"') {
			reading->type = STRING;
			reading->string[reading->value] = string[c];
			reading->value++;
			c++;
		}
		
		c++;
	}

	// After parsing string/text, null terminate
	reading->string[reading->value] = '\0';

	if (string[c] == '\'') {
		c++;
		reading->type = INTEGER;
		reading->value = string[c];
		c += 2; // Skip 0'

		goto end;
	}

	// Lex numbers
	while (string[c] >= '0' && string[c] <= '9') {
		reading->type = INTEGER;
		reading->value *= 10;
		reading->value += string[c] - '0';
		c++;
	}

	end:
	return c;
}

// Handle reserved keywords
int handleText(char text[]) {
	if (!strcmp(text, "fn")) {
		return FN;
	}

	return -1;
}

int out(char text[]) {
	printf(text);
}

int outc(char c) {
	putchar(c);
}

int main() {
	FILE *f = fopen("test.rs", "r");
	char *code = malloc(10000);
	fread(code, 1, 10000, f);
	fclose(f);

	int space = 0;
	struct Token tok;
	struct Token tok2;
	while (1) {
		space = lex(&tok, code, space);

		if (tok.type == FILE_END) {
			break;
		}

		if (tok.type == TEXT) {
			int space2 = lex(&tok2, code, space);

			// Handle macros
			if (tok2.type == '!') {
				if (!strcmp(tok.string, "println")) {
					out("puts");
					includes.stdio = 1;
				}

				space = space2;
				continue;
			}
			
			int r = handleText(tok.string);
			switch (r) {
			case FN:
				out("int ");
				break;
			default:
				out(tok.string);
			}
		} else if (tok.type == STRING) {
			outc('"');
			out(tok.string);
			outc('"');
		} else {
			outc(tok.type);
		}
	}

	free(code);
}
