/**
 * @author Ashley N.
 * @description BlueArachnid AML (Arachnid Markup Language) Types
 */
#ifndef AR_NODE_H
#define AR_NODE_H

/**
 * The maximum size string allocated to each node's string fields
 */
#define AR_NODE_ID_LENGTH		255
#define AR_NODE_VALUE_LENGTH	1024

/**
 * Node Attribute
 */
typedef struct {
	char* name;
	char* value;
} ar_node_attribute_t;

/**
 * Node
 */
typedef struct {
	char* namespace_id;
	char* tag_id;

	struct ar_node_attribute_t* attributes;
	char attribute_count;

	struct ar_node_t* children;
	char child_count;
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
 * Deallocate a node and all its child objects
 */
void ar_destroy_node( ar_node_t* node );


#endif
