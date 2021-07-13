#include <arachnid/node.h>
#include <arachnid/utility.h>
#include <stdlib.h>
#include <string.h>

ar_node_attribute_t* ar_create_node_attribute( const char* name, const char* value ) {
	ar_node_attribute_t* result = malloc( sizeof( ar_node_attribute_t ) );

	// An ar_node type always owns its own strings
	result->name = ar_copy_string( name, AR_NODE_ID_LENGTH );
	result->value = ar_copy_string( value, AR_NODE_VALUE_LENGTH );

	return result;
}

ar_node_t* ar_create_node( const char* namespace_id, const char* tag_id ) {
	ar_node_t* result = malloc( sizeof( ar_node_t ) );

	result->namespace_id = ar_copy_string( namespace_id, AR_NODE_ID_LENGTH );
	result->tag_id = ar_copy_string( tag_id, AR_NODE_ID_LENGTH );

	result->attributes = calloc( AR_INITIAL_CHILD_ELEMENTS, sizeof( ar_node_attribute_t* ) );
	result->attributes_size = AR_INITIAL_CHILD_ELEMENTS;

	result->children = calloc( AR_INITIAL_CHILD_ELEMENTS, sizeof( ar_node_t* ) );
	result->children_size = AR_INITIAL_CHILD_ELEMENTS;

	return result;
}

void ar_destroy_node_attribute( ar_node_attribute_t* attribute ) {
	if( attribute != NULL ) {
		free( attribute->name );
		free( attribute->value );

		free( attribute );
	}
}

void ar_destroy_node( ar_node_t* node ) {
	if( node != NULL ) {
		free( node->namespace_id );
		free( node->tag_id );

		for( int i = 0; i < node->attributes_size; i++ ) {
			ar_destroy_node_attribute( node->attributes[ i ] );
		}

		free( node->attributes );

		for( int i = 0; i < node->children_size; i++ ) {
			ar_destroy_node( node->children[ i ] );
		}

		free( node->children );

		free( node );
	}
}

void ar_add_node_attribute( ar_node_t* target, ar_node_attribute_t* attribute ) {
	if( target != NULL && attribute != NULL ) {
		// Verify elements do not already contain attribute->name
		for( int i = 0; i < target->attributes_size; i++ ) {
			if( target->attributes[ i ] != NULL && strcmp( target->attributes[ i ]->name, attribute->name ) == 0 ) {
				// Cannot add a new attribute with the same name
				return;
			}
		}

		// Find the nearest NULL value and insert attribute
		for( int i = 0; i < target->attributes_size; i++ ) {
			if( target->attributes[ i ] == NULL ) {
				target->attributes[ i ] = attribute;
				return;
			}
		}

		// If we got here, there wasn't any room in the existing list so realloc
		int insert_index = target->attributes_size;

		target->attributes_size += AR_INITIAL_CHILD_ELEMENTS;
		target->attributes = realloc( target->attributes, target->attributes_size );

		target->attributes[ insert_index ] = attribute;
	}
}

ar_node_attribute_t* ar_get_attribute( ar_node_t* target, const char* name ) {
	if( target != NULL && name != NULL ) {
		for( int i = 0; i < target->attributes_size; i++ ) {
			if( target->attributes[ i ] != NULL && strcmp( target->attributes[ i ]->name, name ) == 0 ) {
				return target->attributes[ i ];
			}
		}
	}

	return NULL;
}

void ar_delete_attribute( ar_node_t* target, const char* name ) {
	if( target != NULL && name != NULL ) {
		for( int i = 0; i < target->attributes_size; i++ ) {
			if( target->attributes[ i ] != NULL && strcmp( target->attributes[ i ]->name, name ) == 0 ) {
				ar_destroy_node_attribute( target->attributes[ i ] );
				target->attributes[ i ] = NULL;
			}
		}
	}
}

void ar_add_node_child( ar_node_t* target, ar_node_t* child ) {
	if( target != NULL && child != NULL ) {
		// Search for an existing null
		for( int i = 0; i < target->children_size; i++ ) {
			if( target->children[ i ] == NULL ) {
				target->children[ i ] = child;
				return;
			}
		}

		// If we get here there wasn't any room so do a realloc
		int insert_index = target->children_size;

		target->children_size += AR_INITIAL_CHILD_ELEMENTS;
		target->children = realloc( target->children, target->children_size );

		target->children[ insert_index ] = child;
	}
}

void ar_delete_node_child( ar_node_t* target, ar_node_t* child ) {
	if( target != NULL && child != NULL ) {
		for( int i = 0; i < target->children_size; i++ ) {
			if( target->children[ i ] == child ) {
				ar_destroy_node( child );
				target->children[ i ] = NULL;
				return;
			}
		}
	}
}

ar_node_t* ar_detach_child_node( ar_node_t* target, ar_node_t* child ) {
	if( target != NULL && child != NULL ) {
		for( int i = 0; i < target->children_size; i++ ) {
			if( target->children[ i ] == child ) {
				target->children = NULL;
				return child;
			}
		}
	}

	// The item wasn't found in this node, return the child provided
	return child;
}


ar_selector_t* ar_create_selector() {
	ar_selector_t* selector = calloc( 1, sizeof( ar_selector_t ) );

	selector->attributes = calloc( AR_INITIAL_CHILD_ELEMENTS, sizeof( ar_node_attribute_t* ) );
	selector->attributes_size = AR_INITIAL_CHILD_ELEMENTS;

	selector->is_wildcard = false;
	selector->is_immediate_child = false;

	return selector;
}

void ar_destroy_selector( ar_selector_t* selector ) {
	free( selector->namespace_id );
	free( selector->tag_id );
	free( selector->node_id );

	for( int i = 0; i < selector->attributes_size; i++ ) {
		ar_destroy_node_attribute( selector->attributes[ i ] );
	}

	free( selector->attributes );

	free( selector->pseudo );

	free( selector );
}

void ar_add_selector_attribute( ar_selector_t* target, ar_node_attribute_t* attribute ) {
        if( target != NULL && attribute != NULL ) {
                // Verify elements do not already contain attribute->name
                for( int i = 0; i < target->attributes_size; i++ ) {
                        if( target->attributes[ i ] != NULL && strcmp( target->attributes[ i ]->name, attribute->name ) == 0 ) {
                                // Cannot add a new attribute with the same name
                                return;
                        }
                }

                // Find the nearest NULL value and insert attribute
                for( int i = 0; i < target->attributes_size; i++ ) {
                        if( target->attributes[ i ] == NULL ) {
                                target->attributes[ i ] = attribute;
                                return;
                        }
                }

                // If we got here, there wasn't any room in the existing list so realloc
                int insert_index = target->attributes_size;

                target->attributes_size += AR_INITIAL_CHILD_ELEMENTS;
                target->attributes = realloc( target->attributes, target->attributes_size );

                target->attributes[ insert_index ] = attribute;
        }
}
