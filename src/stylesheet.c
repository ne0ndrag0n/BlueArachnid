#include <arachnid/stylesheet.h>
#include <arachnid/utility.h>
#include <stdlib.h>

ar_stylesheet_property_t* ar_create_stylesheet_property( const char* name ) {
	ar_stylesheet_property_t* result = malloc( sizeof( ar_stylesheet_property_t ) );

	result->name = ar_copy_string( name, AR_NODE_ID_LENGTH );
	result->type = AR_STYLESHEET_DATATYPE_NULL;

	return result;
}

void ar_destroy_stylesheet_property( ar_stylesheet_property_t* property ) {
	free( property->name );

	if( property->type == AR_STYLESHEET_DATATYPE_STRING ) {
		free( property->value_string );
	}

	free( property );
}

void ar_set_stylesheet_property_string( ar_stylesheet_property_t* property, const char* value ) {
	// Destroy existing string if present
	if( property->type == AR_STYLESHEET_DATATYPE_STRING ) {
		free( property->value_string );
	}

	property->type = AR_STYLESHEET_DATATYPE_STRING;
	property->value_string = ar_copy_string( value, AR_NODE_VALUE_LENGTH );
}

void ar_set_stylesheet_property_long( ar_stylesheet_property_t* property, long value ) {
	// Destroy existing string if present
	if( property->type == AR_STYLESHEET_DATATYPE_STRING ) {
		free( property->value_string );
	}

	property->type = AR_STYLESHEET_DATATYPE_LONG;
	property->value_long = value;
}

void ar_set_stylesheet_property_double( ar_stylesheet_property_t* property, double value ) {
	// Destroy existing string if present
	if( property->type == AR_STYLESHEET_DATATYPE_STRING ) {
		free( property->value_string );
	}

	property->type = AR_STYLESHEET_DATATYPE_DOUBLE;
	property->value_long = value;
}
