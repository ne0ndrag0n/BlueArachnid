/**
 * @author Ashley N.
 * @description Arachnid Stylesheet/Markup parser
 */
#ifndef AR_PARSER_H
#define AR_PARSER_H

typedef enum ar_tokentype_t {
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
	AR_TOKENTYPE_PAREN_RIGHT
} ar_tokentype_t;

typedef struct ar_token_t {
	ar_tokentype_t type;
	struct ar_token_t* next;
	union {
		char* as_string;
		long as_numeric;
	};
} ar_token_t;

/**
 * Read stylesheet and output tokens
 */
ar_token_t* ar_tokenize_style( const char* body );

#endif
