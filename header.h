#ifndef HEADER_H
#define HEADER_H

// Types for 
enum Tokens {
	MULT,
	ADD,
	INTEGER,
	TEXT,
	STRING,
	PAREN_LEFT,
	PAREN_RIGHT,
	SEMICOLON,
	EQUAL,
	EXCLAMATION,
	FILE_END,
	BRACKET_LEFT,
	BRACKET_RIGHT
};

enum Text {
	FN
};

struct Token {
	int type;
	int value; // Also length
	char string[1024];
};

#endif
