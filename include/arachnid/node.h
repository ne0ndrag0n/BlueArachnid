/**
 * @author Ashley N.
 * @description BlueArachnid AML (Arachnid Markup Language) Types
 *
 * The ~zen of ownership~:
 * All structs seen in this file own their subobjects. This means you should no longer
 * consider the target object under your control when calling an add function. All
 * strings will be copied.
 */
#ifndef AR_NODE_H
#define AR_NODE_H

/**
 * The maximum size string allocated to each node's string fields
 */
#define AR_NODE_ID_LENGTH		255
#define AR_NODE_VALUE_LENGTH	1024

/**
 * The initial size of the pointer list assigned to each ar_node_t's subobject fields
 */
#define AR_INITIAL_CHILD_ELEMENTS	64

/**
 * Node Attribute
 */
typedef struct ar_node_attribute_t {
	char* name;
	char* value;
} ar_node_attribute_t;

/**
 * Node
 */
typedef struct ar_node_t {
	char* namespace_id;
	char* tag_id;

	struct ar_node_attribute_t** attributes;
	int attributes_size;

	struct ar_node_t** children;
	int children_size;
} ar_node_t;


/**
 * Create a new node attribute type
 */
ar_node_attribute_t* ar_create_node_attribute( char* name, char* value );

/**
 * Create a new node type
 */
ar_node_t* ar_create_node( char* namespace_id, char* tag_id );

/**
 * Deallocate node attribute
 */
void ar_destroy_node_attribute( ar_node_attribute_t* attribute );

/**
 * Deallocate node type
 */
void ar_destroy_node( ar_node_t* node );

/**
 * Add attribute object to node.
 */
void ar_add_attribute( ar_node_t* target, ar_node_attribute_t* attribute );

/**
 * Find attribute by name. Returns NULL if attribute not found.
 */
ar_node_attribute_t* ar_get_attribute( ar_node_t* target, const char* name );

/**
 * Delete attribute from node by name. This will deallocate the object.
 */
void ar_delete_attribute( ar_node_t* target, const char* name );

#endif
