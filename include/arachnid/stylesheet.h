/**
 * @author Ashley N.
 * @description Arachnid Stylesheet Types
 */
#ifndef AR_STYLESHEET_H
#define AR_STYLESHEET_H

#include <arachnid/node.h>

/**
 * Stylesheet property data types
 */
typedef enum ar_stylesheet_datatype_t {
	AR_STYLESHEET_DATATYPE_NULL,
	AR_STYLESHEET_DATATYPE_STRING,
	AR_STYLESHEET_DATATYPE_LONG,
	AR_STYLESHEET_DATATYPE_DOUBLE
} ar_stylesheet_datatype_t;

/**
 * Stylesheet property
 */
typedef struct ar_stylesheet_property_t {
	char* name;
	ar_stylesheet_datatype_t type;
	union {
		char* value_string;
		long value_long;
		double value_double;
	};
} ar_stylesheet_property_t;

/**
 * Stylesheet object
 */
typedef struct ar_stylesheet_t {
	ar_selector_t** identifiers;
	int identifiers_size;

	ar_stylesheet_property_t** properties;
	int properties_size;

	struct ar_stylesheet_t** subsheets;
	int subsheets_size;
} ar_stylesheet_t;

/**
 * Create a stylesheet property
 */
ar_stylesheet_property_t* ar_create_stylesheet_property( const char* name );

/**
 * Destroy a stylesheet property
 */
void ar_destroy_stylesheet_property( ar_stylesheet_property_t* property );


/**
 * Set stylesheet property as string
 */
void ar_set_stylesheet_property_string( ar_stylesheet_property_t* property, const char* value );

/**
 * Set stylesheet property as long
 */
void ar_set_stylesheet_property_long( ar_stylesheet_property_t* property, long value );

/**
 * Set stylesheet property as double
 */
void ar_set_stylesheet_property_double( ar_stylesheet_property_t* property, double value );

#endif
