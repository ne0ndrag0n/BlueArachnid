/**
 * Arachnid toolbox functions
 */
#ifndef AR_UTILITY_H
#define AR_UTILITY_H

/**
 * Copy distrusted string, clamping to upper bound
 */
char* ar_copy_string( const char* untrusted, int max_length );

#endif
