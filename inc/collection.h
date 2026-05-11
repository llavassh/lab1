#pragma once

#include "fieldinfo.h"
#include <stddef.h>

typedef struct Collection {
	void* data;
	size_t size;
	size_t capacity;
	FieldInfo* type;
} Collection;

Collection* CollectionCreate(FieldInfo* type);
void CollectionDestroy(Collection* col);
void CollectionClear(Collection* col);

int CollectionAppend(Collection* col, const void* element);
void* CollectionGet(const Collection* col, size_t index);
size_t CollectionSize(const Collection* col);
int CollectionIsEmpty(const Collection* col);

Collection* CollectionSlice(const Collection* col, size_t start, size_t end);
Collection* CollectionConcat(const Collection* col_a, const Collection* col_b);
