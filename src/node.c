#include <arachnid/node.h>
#include <arachnid/utility.h>
#include <stdlib.h>
#include <string.h>

ar_node_attribute_t* ar_create_node_attribute( char* name, char* value ) {
	ar_node_attribute_t* result = malloc( sizeof( ar_node_attribute_t ) );

	result->name = ar_copy_string( name, AR_NODE_ID_LENGTH );
	result->value = ar_copy_string( value, AR_NODE_VALUE_LENGTH );

	return result;
}

ar_node_t* ar_create_node( char* namespace_id, char* tag_id ) {
	ar_node_t* result = malloc( sizeof( ar_node_t ) );

	result->namespace_id = namespace_id;
	result->tag_id = tag_id;

	return result;
}

void ar_destroy_node( ar_node_t* node ) {

}
