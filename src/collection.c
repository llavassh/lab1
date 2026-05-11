#include <stdlib.h>
#include "fieldinfo.h"
#include "collection.h"

#define INITIAL_CAPACITY 8

//увеличить место в 2 раза
static int Resize(Collection* col) {  
	size_t new_capacity = col->capacity * 2;
	void* new_data = realloc(col->data, new_capacity * col->type->size);
	if (!new_data) return 0;
	
	col->data = new_data;
	col->capacity = new_capacity;
	return 1;
}

//создает коллекцию
Collection* CollectionCreate(FieldInfo* type) {  
	if (!type) return NULL;
	
	Collection* col = malloc(sizeof(Collection));
	if (!col) return NULL;

	col->size = 0;
	col->capacity = INITIAL_CAPACITY;
	col->type = type;

	col->data = malloc(col->type->size * col->capacity);

	if (!col->data) {
		free(col);
		return NULL;
	}

	return col;
}

//удаляет коллекцию
void CollectionDestroy(Collection* col) {  
	if (!col) return;

	for (size_t i = 0; i < col->size; i++) {
		void* elem = (char*)col->data + i * col->type->size;
		col->type->destroy(elem);
	}

	free(col->data);
	free(col);
}

//очищает коллекцию
void CollectionClear(Collection* col) {
	if (!col) return;

	for (size_t i = 0; i < col->size; i++) {
		void* elem = (char*)col->data + i * col->type->size;
		col->type->destroy(elem);
	}
	col->size = 0;
}

//добавляет элемент в коллекцию
int CollectionAppend(Collection* col, const void* element) { 
	if (!col || !element) return 0;

	if (col->size == col->capacity) {
		if (!Resize(col)) return 0;
	}
	
	void* dest = (char*)col->data + col->size * col->type->size;
	col->type->copy(dest, element);
	col->size++;

	return 1;
}

//возвращает указатель на элемент коллекции
void* CollectionGet(const Collection* col, size_t index) { 
	if (!col || index >= col->size) return NULL;  
	return (char*)col->data + index * col->type->size;
}

//показывает сколько элементов в коллекции
size_t CollectionSize(const Collection* col) {  
	return col ? col->size : 0;
}

//показывает пустая ли коллекция
int CollectionIsEmpty(const Collection* col) {
    return col ? col->size == 0 : 1;
}

//возвращает какой-то отрезок коллекции
Collection* CollectionSlice(const Collection* col, size_t start, size_t end) {   
	if (!col || start > end || end > col->size) return NULL;

	Collection* result = CollectionCreate(col->type);
	if (!result) return NULL;

	for (size_t i = start; i < end; i++) {
		void* elem = (char*)col->data + i * col->type->size;
		if (!CollectionAppend(result, elem)) {
			CollectionDestroy(result);
			return NULL;
		}
	}

	return result;
}

//соединяет две коллекции
Collection* CollectionConcat(const Collection* col_a, const Collection* col_b) {  
	if (!col_a || !col_b || col_a->type != col_b->type) return NULL;

	Collection* result = CollectionCreate(col_a->type);
	if (!result) return NULL;

	for (size_t i = 0; i < col_a->size; i++) {
		void* elem = (char*)col_a->data + i * col_a->type->size;
		if (!CollectionAppend(result, elem)) {
			CollectionDestroy(result);
			return NULL;
		}
	}

	for (size_t i = 0; i < col_b->size; i++) {
		void* elem = (char*)col_b->data + i * col_b->type->size;
		if (!CollectionAppend(result, elem)) {
			CollectionDestroy(result);
			return NULL;
		}
	}

	return result;
}