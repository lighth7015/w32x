#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "windows.h"
#include "event_log.h"

List eventlog;

void* list_poll(List list){
	ListItem first = list->first;
	
	if (first == NULL)
		return NULL;
	
	if (list->first == list->last) {
		list->first = NULL;
		list->last = NULL;
	} else {
		list->first = first->next;
		first->next->prev = NULL;
	}
	
	void* callback = first->callback;
	free(first);
	
	list->length--;
	return callback;
}

static void* list_pop(List list){
	ListItem last = list->last;
	if(last == NULL) return NULL;

	if (list->first == list->last) {
		list->first = NULL;
		list->last = NULL;
	} else {
		list->last = last->prev;
		last->prev->next = NULL;
	}
		
	void* callback = last->callback;
	free(last);
	
	list->length--;
	return callback;
}

List CreateList(){
	List list = (List) calloc(1, sizeof(List));
	list->destructor = free;
	return list;
}

ListIter iterator(List list, char init)
{	
	ListIter iter = (ListIter) calloc(1, sizeof(ListIter));
	iter->started = 0;
	
	if(init==FRONT){
		iter->current = list->first;
	}
	else if(init==BACK){
		iter->current = list->last;
	}
	else {
		free(iter);
		iter = NULL;
	} 
	
	return iter;
}

void ListAppend(List list, Callback callback, unsigned int condition, int size) {
	ListItem item = (ListItem)calloc(1, sizeof(ListItem));
	
	item->callback = callback;
	item->condition = condition;
	
	if(list->first==NULL){
		item->prev = NULL;
		item->next = NULL;
		list->first = item;
		list->last = item;
	}
	else{
		list->last->next = item;
		item->prev = list->last;
		item->next = NULL;
		list->last = item;
	}
	
	list->length++;
}

void* CurrentItem(ListIter iter)
{	
	if(iter->started && iter->current!=NULL)
		return iter->current->callback;
	
	return NULL;
}

void* IterNext(ListIter iter){
	if(!iter->started&&iter->current!=NULL){
		iter->started=1;
		return iter->current->callback;
	}
	if(iter->current!=NULL){
		iter->current = iter->current->next;
		return CurrentItem(iter);
	}
	return NULL;
}

void* IterReverse(ListIter iter){
	ListItem current = NULL;
	
	if(!iter->started&&iter->current!=NULL){
		iter->started = 1;
		current = CurrentItem(iter);
	}
	if(iter->current!=NULL){
		iter->current = iter->current->prev;
		current = CurrentItem(iter);
	}
	return current;
}

void* ListHead(List list){
	void* target;

	if (list->first)
	{
		target = (void *) list->first->callback;
	}
	else { 
		target = list->first;
	}
	
	return target;
}

void* ListTail(List list) {
	void* target;

	if (list->last)
	{
		target = (void *) list->last->callback;
	}
	else { 
		target = list->last;
	}
	
	return target;
}

int ListEmpty(List list) {
	return list->first? 0: 1;
}

void* ListTrunc(List list){
	ListItem last = list->last;
	if(last == NULL) return NULL;

	if (list->first == list->last) {
		list->first = NULL;
		list->last = NULL;
	} else {
		list->last = last->prev;
		last->prev->next = NULL;
	}
		
	void* callback = last->callback;
	free(last);
	
	list->length--;
	return callback;
}

void ListRemove(List list, int end)
{
	void* callback;
	
	if(end == FRONT)
	{
		callback = list_poll(list);
		list->destructor(callback);
	}
	else if (end == BACK)
	{
		callback = list_pop(list);
		list->destructor(callback);
	}
	else 
	{
		
	}
}

void ListFree(List list){
	ListItem cur = list->first;
	ListItem next;
	while(cur!=NULL){
		next = cur->next;
		list->destructor(cur->callback);
		free(cur);
		cur = next;
	}
	free(list);
}

void ListInsert(List list, ListItem before, Callback callback, unsigned int condition, int size)
{	
	ListItem item = (ListItem) calloc(1, sizeof(ListItem));
	item->callback = callback;
    
	if (list->first == NULL || before == list->last)
	{
        ListAppend(list, callback, condition, size);
    } 
    else if (before == NULL)
	{	
		item->next = list->first;
		item->prev = NULL;
		
		item->next->prev = item;
		list->first = item;
		
		list->length++;
    }
    else
	{	
		item->next = before->next;
		item->prev = before;
		
		before->next = item;
		
		item->next->prev = item;
		list->length++;
    }
}

__attribute__((constructor))
int eventlog_init()
{	
	eventlog = CreateList();
	printf("Initializing Event Log.\n");
	
	return 0;
}
