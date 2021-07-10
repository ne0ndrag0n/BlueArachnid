#include <arachnid/utility.h>
#include <stdlib.h>
#include <string.h>

char* ar_copy_string( char* untrusted, int max_length ) {
	if( untrusted == NULL || max_length == 0 ) {
		return NULL;
	}

	int allocation_size = strlen( untrusted );

	if( allocation_size == 0 ) {
		char* result = malloc( sizeof( char ) );
		result[ 0 ] = 0;
		return result;
	}

	if( allocation_size > max_length ) {
		allocation_size = max_length;
	}

	// Don't forget the \0
	char* copy = malloc( sizeof( char ) * ( allocation_size + 1 ) );
	strncpy( copy, untrusted, allocation_size );
	copy[ allocation_size ] = 0;

	return copy;
}
