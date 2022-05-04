#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
void print_welcome_message();
List *get_nums(void);
int get_number_of_nums(void);
int *get_num(void);
void list_append_num(List*, int *);
void insertion_sort(List*);
void print_sort_step(List*, List*);
void print_list(List*);
void insert(List*, ListNode*);
List* list_copy(List*);
ListNode *find_max_node(List*);
void selection_sort(List*);

int main(void)
{
  print_welcome_message();
  List *unsorted_nums = get_nums();
  insertion_sort(list_copy(unsorted_nums));
  selection_sort(list_copy(unsorted_nums));
  return 0;
}

void print_welcome_message(void){
  printf("Welcome to my sorting program...\n\n\n");
}

List *get_nums(void) {
  int number_of_nums = get_number_of_nums();
  List *unsorted_nums = list_create();
  for(int i = 0; i < number_of_nums; i++) {
    int *num = get_num();
    list_append_num(unsorted_nums, num);
  }
  return unsorted_nums;
}

int get_number_of_nums(void) {
	printf("\tPlease enter the number of numbers:\n\t");
	int n;
	scanf("%d", &n);
	return n;
}

int *get_num(void){
	printf("Please enter the next number:\n");
	int *num = malloc(sizeof(int));
	scanf("%i", num);
	return num;
}

void list_append_num(List *list, int *num){
  list_insert_after(list, list_create_node(num), NULL);
}

void insertion_sort(List *unsorted){
	printf("Here are the results of sorting with insertion sort:\n");
	List *sorted = list_create();
	while(unsorted->count > 0) 
	{
		print_sort_step(unsorted, sorted);
		int *num = list_remove_node(unsorted, unsorted->first);
		ListNode *new_node = list_create_node(num);
		insert(sorted, new_node); //puts new_node in the right place
	}
	print_sort_step(unsorted, sorted);
}

void print_sort_step(List *unsorted, List *sorted) {
	printf("U: ");
	print_list(unsorted);
	printf("S: ");
	print_list(sorted);
	printf("\n");
}

void print_list(List *list) {
  printf("[");
  ListNode *current = list->first;
  while(current != NULL) 
  {
    printf("%i ", *((int*) current->value));
    current = current->next;
  }
  
  printf("]\n");
}

void insert(List *list, ListNode *node) {
  ListNode *current = list->first;
  while(current != NULL) {
    if((*(int*) current->value) <= *((int*) node->value)) {
      list_insert_before(list, node, current->value);
      return;
    }
    current = current->next;
  }
  list_insert_before(list, node, NULL);
}

List* list_copy(List* list) {
  List *copy_list = list_create();
  ListNode *current = list->first;
  while(current != NULL) {
    ListNode *copy_node = list_create_node(current->value);
    list_insert_after(copy_list, copy_node, NULL);
    current = current->next;
  }
  return copy_list;
}

void selection_sort(List *unsorted){
	printf("Here are the results of sorting with selection sort:\n");
  	List *sorted = list_create();
  	while(unsorted->count > 0) 
	{
		print_sort_step(unsorted, sorted);
		ListNode *max = find_max_node(unsorted); //find the max node of unsorted list
		int *num = list_remove_node(unsorted, max); //remove the node
		ListNode *new_node = list_create_node(num); //create a new node with the payload, num 
		list_insert_before(sorted, new_node, NULL); //puts new_node in the first spot
	}
	print_sort_step(unsorted, sorted);
}

ListNode *find_max_node(List *list)
{
  ListNode *current = list->first;
  ListNode *max = list->first;
  int max_value;
  int current_value;
  // We start at the first node in the list, take the value and cast to int for comparison. 
  while(current != NULL)
  {
    max_value = *((int *)(max->value));
    current_value = *((int *)(current->value));
    // If the current value is greater than the max, update the max node.
    if(current_value > max_value)
    {
      //printf("New max found: %d!", max_value);
      max = current;
    }
    // Move next until current->next is NULL.
    current = current->next;
  }
  //printf("Current->next is now null");
  return max;
}