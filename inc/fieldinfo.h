#pragma once
#include <stddef.h>

typedef struct FieldInfo { 
	size_t size;
	void (*print)(const void* data);  
	void (*copy)(void* dest, const void* src);
	void (*destroy)(void* data);
} FieldInfo;

FieldInfo* GetCharFieldInfo();
FieldInfo* GetStringFieldInfo();  