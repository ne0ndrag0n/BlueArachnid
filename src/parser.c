#include <arachnid/parser.h>
#include <arachnid/utility.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

bool is_numeric( char c ) {
	return c >= '0' && c <= '9';
}

bool is_alpha( char c ) {
	return ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || c == '-';
}

bool is_alphanumeric( char c ) {
	return is_alpha( c ) || is_numeric( c );
}

ar_token_t* append_token( ar_token_t* base, ar_tokentype_t type, int row, int column ) {
	ar_token_t* target = calloc( 1, sizeof( ar_token_t ) );

	if( base != NULL ) {
		base->next = target;
	}

	target->type = type;

	target->position_row = row;
	target->position_column = column;

	return target;
}

ar_token_result_t* ar_tokenize_style( const char* body ) {
	ar_token_result_t* result = ( ar_token_result_t* ) malloc( sizeof( ar_token_result_t ) );

	const char* token = body;
	if( token == NULL ) {
		// There is nothing to generate
		return NULL;
	}

	ar_token_t* current_token = NULL;

	int row = 0;
	int column = -1;

	while( token != NULL ) {
		column++;
		switch( *token++ ) {
			case '\n':
				column = -1;
				row++;
			case ' ':
			case '\t':
			case '\r':
				current_token = append_token( current_token, AR_TOKENTYPE_WHITESPACE, row, column );
				break;
			case '#':
				current_token = append_token( current_token, AR_TOKENTYPE_HASH, row, column );
				break;
			case '.':
				current_token = append_token( current_token, AR_TOKENTYPE_DOT, row, column );
				break;
			case ':':
				current_token = append_token( current_token, AR_TOKENTYPE_COLON, row, column );
				break;
			case '*':
				current_token = append_token( current_token, AR_TOKENTYPE_ASTERISK, row, column );
				break;
			case '(':
				current_token = append_token( current_token, AR_TOKENTYPE_PAREN_LEFT, row, column );
				break;
			case ')':
				current_token = append_token( current_token, AR_TOKENTYPE_PAREN_RIGHT, row, column );
				break;
			case '{':
				current_token = append_token( current_token, AR_TOKENTYPE_BRACE_LEFT, row, column );
				break;
			case '}':
				current_token = append_token( current_token, AR_TOKENTYPE_BRACE_RIGHT, row, column );
				break;
			case ';':
				current_token = append_token( current_token, AR_TOKENTYPE_SEMICOLON, row, column );
				break;
			case '>':
				current_token = append_token( current_token, AR_TOKENTYPE_ANGLEBRACKET_LEFT, row, column );
				break;
			case '<':
				current_token = append_token( current_token, AR_TOKENTYPE_ANGLEBRACKET_RIGHT, row, column );
				break;

			case '"': {
				// Burn characters until newline, eof, or " is encountered
				char string_builder[ AR_STYLESHEET_MAX_STRING_SIZE ] = { 0 };
				char* stylesheet_string = string_builder;
				int safety_counter = AR_STYLESHEET_MAX_STRING_SIZE - 1;

				while( safety_counter && ( *token != '"' || *token != '\r' || *token != '\n' || *token != 0 ) ) {
					column++;
					*stylesheet_string++ = *token++;

					safety_counter--;
				}

				// Verify safety counter
				if( safety_counter == 0 && *token != '"' ) {
					result->is_good = false;
					result->as_error = ( char* ) malloc( AR_ERROR_BUFFER_SIZE * sizeof( char ) );
					snprintf( result->as_error, AR_ERROR_BUFFER_SIZE - 1, "String too large for buffer near [%d, %d]", row, column );
					ar_destroy_token( current_token );
					return result;
				}

				// Error condition if *token isn't " at this point
				if( *token == '\r' || *token == '\n' ) {
					result->is_good = false;
					result->as_error = ( char* ) malloc( AR_ERROR_BUFFER_SIZE * sizeof( char ) );
					snprintf( result->as_error, AR_ERROR_BUFFER_SIZE - 1, "Unexpected newline near [%d, %d]", row, column );
					ar_destroy_token( current_token );
					return result;
				}

				if( *token == 0 ) {
					result->is_good = false;
					result->as_error = ( char* ) malloc( AR_ERROR_BUFFER_SIZE * sizeof( char ) );
					snprintf( result->as_error, AR_ERROR_BUFFER_SIZE - 1, "Unexpected end of file near [%d, %d]", row, column );
					ar_destroy_token( current_token );
					return result;
				}

				// The token is currently pointed at the closing quote, so eat the quote
				token++;
				column++;

				// Standard string copy
				current_token = append_token( current_token, AR_TOKENTYPE_STRING, row, column );
				current_token->as_string = ar_copy_string( string_builder, AR_STYLESHEET_MAX_STRING_SIZE );
				break;
			}

			default: {
				// Identifier or number? We'll need to step backward to find out
				char* previous = token - 1;

				if( is_numeric( *previous ) ) {
					char string_builder[ AR_STYLESHEET_MAX_STRING_SIZE ] = { 0 };
					string_builder[ 0 ] = *previous;
					char* number_string = string_builder + 1;
					bool is_float = false;

					// Continue from current token onward until a non-numeric character is encountered
					int safety_counter = AR_STYLESHEET_MAX_STRING_SIZE - 1;
					while( safety_counter && is_numeric( *token ) ) {
						// If *token is a dot, the value is a float
						if( *token == '.' ) {
							if( is_float ) {
								// If it's already a float, this is an unexpected dot
								result->is_good = false;
								result->as_error = ( char* ) malloc( AR_ERROR_BUFFER_SIZE * sizeof( char ) );
								snprintf( result->as_error, AR_ERROR_BUFFER_SIZE - 1, "Unexpected '.' near [%d, %d]", row, column );
								ar_destroy_token( current_token );
								return result;
							} else {
								is_float = true;
							}
						}

						*number_string++ = *token++;
						column++;
					}

					// Verify safety counter
					if( safety_counter == 0 && is_numeric( *token ) ) {
               	result->is_good = false;
               	result->as_error = ( char* ) malloc( AR_ERROR_BUFFER_SIZE * sizeof( char ) );
               	snprintf( result->as_error, AR_ERROR_BUFFER_SIZE - 1, "Number too large for buffer near [%d, %d]", row, column );
               	ar_destroy_token( current_token );
               	return result;
            	}

					// Parse resulting number string as an actual long or float
					if( is_float ) {
						current_token = append_token( current_token, AR_TOKENTYPE_DOUBLE, row, column );
						current_token->as_double = strtod( string_builder, NULL );
					} else {
						current_token = append_token( current_token, AR_TOKENTYPE_LONG, row, column );
						current_token->as_long = strtol( string_builder, NULL, 10 );
					}
				}
			}
		}
	}

	result->is_good = true;
	result->as_value = current_token;
	return result;
}

void ar_destroy_token( ar_token_t* token ) {
	if( token != NULL ) {
		// Free token->as_string if token contains a string variant
		switch( token->type ) {
			case AR_TOKENTYPE_STRING:
			case AR_TOKENTYPE_IDENTIFIER:
				free( token->as_string );
			default:
				ar_destroy_token( token->next );
				free( token );
		}
	}
}
