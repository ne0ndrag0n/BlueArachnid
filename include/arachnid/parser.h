/**
 * @author Ashley N.
 * @description Arachnid Stylesheet/Markup parser
 */
#ifndef AR_PARSER_H
#define AR_PARSER_H

#include <stdbool.h>

#define AR_STYLESHEET_MAX_STRING_SIZE	255

typedef enum ar_tokentype_t {
	AR_TOKENTYPE_NULL,
	AR_TOKENTYPE_WHITESPACE,
	AR_TOKENTYPE_IDENTIFIER,
	AR_TOKENTYPE_HASH,
	AR_TOKENTYPE_DOT,
	AR_TOKENTYPE_ASTERISK,
	AR_TOKENTYPE_ANGLEBRACKET_LEFT,
	AR_TOKENTYPE_ANGLEBRACKET_RIGHT,
	AR_TOKENTYPE_COLON,
	AR_TOKENTYPE_BRACE_LEFT,
	AR_TOKENTYPE_BRACE_RIGHT,
	AR_TOKENTYPE_PAREN_LEFT,
	AR_TOKENTYPE_PAREN_RIGHT,
	AR_TOKENTYPE_SEMICOLON,
	AR_TOKENTYPE_LONG,
	AR_TOKENTYPE_DOUBLE,
	AR_TOKENTYPE_STRING
} ar_tokentype_t;

typedef struct ar_token_t {
	ar_tokentype_t type;
	struct ar_token_t* next;
	int position_row;
	int position_column;
	union {
		char* as_string;
		long as_long;
		double as_double;
	};
} ar_token_t;

typedef struct ar_token_result_t {
	bool is_good;
	union {
		char* as_error;
		ar_token_t* as_value;
	};
} ar_token_result_t;

/**
 * Read stylesheet and output tokens
 */
ar_token_result_t* ar_tokenize_style( const char* body );

/**
 * Destroy a token or list of tokens
 */
void ar_destroy_token( ar_token_t* token );

#endif
