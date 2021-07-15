/**
 * Arachnid toolbox functions
 */
#ifndef AR_UTILITY_H
#define AR_UTILITY_H

/**
 * Size of any error buffer
 */
#define AR_ERROR_BUFFER_SIZE	255

/**
 * Copy distrusted string, clamping to upper bound
 */
char* ar_copy_string( const char* untrusted, int max_length );

#endif
