#include "linked_list.h"

void insert_last(List*, ListNode*);
void insert_first(List*, ListNode*);
void insert_after_node(List*, ListNode*, ListNode*);
void insert_before_node(List*, ListNode*, ListNode*);
void insert_only_node(List*, ListNode*);
void remove_first(List*);
void remove_last(List*);
void remove_between(List*, ListNode*);
void remove_only_node(List*);

/*
  Just initializes the list structure (the node pointers are NULL and
  the count = 0.
*/
List *list_create(void) {
	List *list = malloc(sizeof(*list));
	list->count = 0;
	list->first = NULL;
	list->last = NULL;
	return list;
}

/*
  Just initializes a ListNode structure (the node pointers are NULL and
  the pointer to the data is set to payload).
*/
ListNode *list_create_node(void *payload) {
	ListNode *node = malloc(sizeof(*node));
	node->value = payload;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

/*
  Frees any nodes in the list and then frees the list structure.
*/
List *list_destroy(List *list) {
  list_clear(list);
  free(list);
  return NULL;
}

/*
  Frees any nodes in the list but leaves the list structure.
*/
void list_clear(List *list) {
  ListNode *current = list->first;
  ListNode *next;
	
  while(current != NULL) 
  {
    next = current->next;
    free(current);
    current = next;
  }
	
  list->first = NULL;
  list->last = NULL;
  list->count = 0;
}

/*
  Returns the count in the list structure.
*/
int list_count(List *list) {
	return list->count;
}

/*
  Returns the first node in the list.
*/
ListNode *list_first(List *list) {
  return list->first;
}

/*
  Returns the last node in the list.
*/
ListNode *list_last(List *list) {
  return list->last;
}

/*
  Finds a node in the list by value and returns the pointer to the node.
  If no matching node is found, returns NULL.
*/
ListNode *list_find(List* list, void* value) {
	/*start at first, check value, no? then go next*/
	ListNode *current = list->first;
	while(current != NULL)
	{
		if(current->value == value){
			return current;
		}
		current = current->next;
	}
	return current;
}

/*
  Inserts a node in the list after the node containing value.  If no node
  has the passed value or if value == NULL, insert at the end of the list.
*/
void list_insert_after(List *list, ListNode *node, void *value) {
	ListNode *access_node = list_find(list, value);
	if(list_count(list)==0)
	{
		insert_only_node(list, node);
	}
	else if(access_node == list->last || value == NULL || access_node == NULL)
	{
		insert_last(list, node);
	}
	else//access node is not last, value is not null and access_node is not null
	{
    insert_after_node(list, access_node, node);
	}
}

/*
  Inserts a node in the list before the node containing value.  If no node has
  the passed value or if value == NULL, insert at the beginning of the list.
*/
void list_insert_before(List *list, ListNode *node, void *value) {
	ListNode *access_node = list_find(list, value);
	if(list_count(list)==0){
		insert_only_node(list,node);
  }
	else if(access_node == list->first || value == NULL || access_node == NULL){
		insert_first(list, node);
	}
	else{
		insert_before_node(list, access_node, node);
	}
}

/*
  Removes the specified node from the list and frees it.  The node's value
  is saved and returned.
*/
void *list_remove_node(List *list, ListNode* node) {
	void *val = node->value;
	if(node == list->first) 
	{
		list->first = node->next;
	}
	if(node == list->last) 
	{
		list->last = node->prev;
	}
	if(node->prev != NULL) 
	{
		node->prev->next = node->next;
	}
	if(node->next != NULL) 
	{
		node->next->prev = node->prev;
	}
	free(node);
	list->count--;
	return val;
}


/*
  Removes the node with the specified value from the list.  The node's value
  is saved and returned.  If a node with the value is not found in the list,
  return NULL
*/
void *list_remove_value(List* list, void* value) {
  	ListNode *current = list_find(list, value);
	
	if(current == NULL)
	{
		return NULL;
	}
	return list_remove_node(list, current);
}


void insert_only_node(List *list, ListNode *node){
	list->first = node;
	list->last = node;
	list->count += 1;
}

void insert_after_node(List *list, ListNode *access_node, ListNode *node){
	access_node->next->prev = node;
	node->next = access_node->next;
	access_node->next = node;
	node->prev = access_node;
	list->count += 1;
}

void insert_before_node(List *list, ListNode *access_node, ListNode *node){
	node->next = access_node;
	node->prev = access_node->prev;
	access_node->prev->next = node;
	access_node->prev = node;
	list->count += 1;
}

void insert_last(List *list, ListNode *node){
	list->last->next = node;
	node->prev = list->last;
	list->last = node;
	list->count += 1;
}

void insert_first(List *list, ListNode *node) {
	list->first->prev = node;
	node->next = list->first;
	list->first = node;
	list->count += 1;
}
