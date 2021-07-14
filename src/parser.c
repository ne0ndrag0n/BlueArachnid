#include <arachnid/parser.h>
#include <arachnid/utility.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_numeric( char c ) {
	return c >= '0' && c <= '9';
}

bool is_alpha( char c ) {
	return ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || c == '-';
}

bool is_alphanumeric( char c ) {
	return is_alpha( c ) || is_numeric( c );
}

ar_token_t* append_token( ar_token_t* base, ar_tokentype_t type ) {
	base->next = calloc( 1, sizeof( ar_token_t ) );
	base->next->type = type;

	return base->next;
}

ar_token_result_t* ar_tokenize_style( const char* body ) {
	ar_token_result_t* result = ( ar_token_result_t* ) malloc( sizeof( ar_token_result_t ) );
	result->is_good = false;
	result->as_error = "<unknown>";

	const char* token = body;

	if( token == NULL ) {
		// There is nothing to generate
		return NULL;
	}

	ar_token_t* first_token = ( ar_token_t* ) calloc( 1, sizeof( ar_token_t ) );
	ar_token_t* current_token = first_token;

	while( token != NULL ) {
		switch( *token++ ) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				current_token = append_token( current_token, AR_TOKENTYPE_WHITESPACE );
				break;
			case '#':
				current_token = append_token( current_token, AR_TOKENTYPE_HASH );
				break;
			case '.':
				current_token = append_token( current_token, AR_TOKENTYPE_DOT );
				break;
			case ':':
				current_token = append_token( current_token, AR_TOKENTYPE_COLON );
				break;
			case '*':
				current_token = append_token( current_token, AR_TOKENTYPE_ASTERISK );
				break;
			case '(':
				current_token = append_token( current_token, AR_TOKENTYPE_PAREN_LEFT );
				break;
			case ')':
				current_token = append_token( current_token, AR_TOKENTYPE_PAREN_RIGHT );
				break;
			case '{':
				current_token = append_token( current_token, AR_TOKENTYPE_BRACE_LEFT );
				break;
			case '}':
				current_token = append_token( current_token, AR_TOKENTYPE_BRACE_RIGHT );
				break;
			case ';':
				current_token = append_token( current_token, AR_TOKENTYPE_SEMICOLON );
				break;
			case '>':
				current_token = append_token( current_token, AR_TOKENTYPE_ANGLEBRACKET_LEFT );
				break;
			case '<':
				current_token = append_token( current_token, AR_TOKENTYPE_ANGLEBRACKET_RIGHT );
				break;

			case '"': {
				// Burn characters until newline, eof, or " is encountered
				char string_builder[ AR_STYLESHEET_MAX_STRING_SIZE ] = { 0 };
				char* stylesheet_string = string_builder;
				while(
					*token != '"' ||
					*token != '\r' ||
					*token != '\n' ||
					*token != 0
				) {
					*stylesheet_string++ = *token++;
				}

				// Error condition if *token isn't " at this point
				if( *token == '\r' || *token == '\n' ) {
					result->is_good = false;
					result->as_error = "Unexpected newline";

					return result;
				}

				if( *token == 0 ) {
					result->is_good = false;
					result->as_error = "Unexpected end of file";

					return result;
				}

				// Standard string copy
				current_token = append_token( current_token, AR_TOKENTYPE_STRING );
				current_token->as_string = ar_copy_string( string_builder, AR_STYLESHEET_MAX_STRING_SIZE );
				break;
			}

			default:
				break;
		}
	}

	return result;
}
