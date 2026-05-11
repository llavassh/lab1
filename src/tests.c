#include "collection.h"
#include "mystring.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(cond, msg) \
    do { \
        if (cond) { \
            printf("  [OK] %s\n", msg); \
            tests_passed++; \
        } else { \
            printf("  [FAIL] %s\n", msg); \
            tests_failed++; \
        } \
    } while(0) 

// ===== ТЕСТЫ STRINGCREATE =====
static void test_create(void) {
    printf("\n=== StringCreate ===\n");
    
    //латиница
    String* s1 = StringCreate("Hello");
    TEST(s1 != NULL, "Latin string created");
    TEST(StringLength(s1) == 5, "Length = 5 bytes");
    
    //русские буквы (2 байта)
    String* s2 = StringCreate("привет");
    TEST(s2 != NULL, "Russian string created");
    TEST(StringLength(s2) == 12, "Length = 12 bytes (6 chars * 2)");
    
    //эмодзи (4 байта)
    String* s3 = StringCreate("😀🎉");
    TEST(s3 != NULL, "Emoji string created");
    TEST(StringLength(s3) == 8, "Length = 8 bytes (2 emoji * 4)");
    
    //смесь
    String* s4 = StringCreate("Hi привет 😀");
    TEST(s4 != NULL, "Mixed string created");
    TEST(StringLength(s4) == 20, "Length = 20 bytes");
    
    //пустая строка
    String* s5 = StringCreate("");
    TEST(s5 != NULL, "Empty string created");
    TEST(StringLength(s5) == 0, "Length = 0");
    
    //ошибка
    String* s6 = StringCreate(NULL);
    TEST(s6 == NULL, "NULL returns NULL");
    
    StringDestroy(s1);
    StringDestroy(s2);
    StringDestroy(s3);
    StringDestroy(s4);
    StringDestroy(s5);
}

// ===== ТЕСТЫ STRINGDESTROY =====
static void test_destroy(void) {
    printf("\n=== StringDestroy ===\n");
    
    String* s = StringCreate("test");
    TEST(s != NULL, "String created");
    StringDestroy(s);
    TEST(1, "Destroy completed without crash");
    
    StringDestroy(NULL);
    TEST(1, "Destroy NULL does nothing");
}

// ===== ТЕСТЫ STRINGLENGTH =====
static void test_length(void) {
    printf("\n=== StringLength ===\n");
    
    String* s1 = StringCreate("Hi");
    TEST(StringLength(s1) == 2, "Latin: 2 bytes");
    
    String* s2 = StringCreate("привет");
    TEST(StringLength(s2) == 12, "Russian: 12 bytes");
    
    String* s3 = StringCreate("");
    TEST(StringLength(s3) == 0, "Empty: 0 bytes");
    
    TEST(StringLength(NULL) == 0, "NULL returns 0");
    
    StringDestroy(s1);
    StringDestroy(s2);
    StringDestroy(s3);
}

// ===== ТЕСТЫ STRINGCHARAT =====
static void test_char_at(void) {
    printf("\n=== StringCharAt ===\n");
    
    String* s1 = StringCreate("ABC");
    TEST(StringCharAt(s1, 0) == 'A', "Latin: first char = 'A'");
    TEST(StringCharAt(s1, 1) == 'B', "Latin: second char = 'B'");
    TEST(StringCharAt(s1, 2) == 'C', "Latin: third char = 'C'");
    
    String* s2 = StringCreate("привет");
    unsigned char first_byte = (unsigned char)StringCharAt(s2, 0);
    TEST(first_byte == 0xD0, "Russian: first byte = 0xD0");
    
    String* s3 = StringCreate("😀");
    unsigned char emoji_byte = (unsigned char)StringCharAt(s3, 0);
    TEST(emoji_byte == 0xF0, "Emoji: first byte = 0xF0");
    
    //ошибки
    TEST(StringCharAt(s1, 999) == '\0', "Invalid index returns '\\0'");
    TEST(StringCharAt(NULL, 0) == '\0', "NULL returns '\\0'");
    
    StringDestroy(s1);
    StringDestroy(s2);
    StringDestroy(s3);
}

// ===== ТЕСТЫ STRINGCONCAT =====
static void test_concat(void) {
    printf("\n=== StringConcat ===\n");
    
    //латиница + латиница
    String* s1 = StringCreate("Hello");
    String* s2 = StringCreate(" World");
    String* s3 = StringConcat(s1, s2);
    TEST(s3 != NULL, "Latin + Latin works");
    char* cstr = StringToCstr(s3);
    TEST(strcmp(cstr, "Hello World") == 0, "Result = 'Hello World'");
    free(cstr);
    StringDestroy(s3);
    
    //русские + русские
    String* s4 = StringCreate("привет");
    String* s5 = StringCreate(" мир");
    String* s6 = StringConcat(s4, s5);
    TEST(s6 != NULL, "Russian + Russian works");
    TEST(StringLength(s6) == 19, "Length = 19 bytes");
    StringDestroy(s6);
    
    //латиница + русские
    String* s7 = StringCreate("Hello ");
    String* s8 = StringCreate("привет");
    String* s9 = StringConcat(s7, s8);
    TEST(s9 != NULL, "Latin + Russian works");
    TEST(StringLength(s9) == 18, "Length = 18");
    StringDestroy(s9);
    
    //ошибки
    TEST(StringConcat(NULL, s2) == NULL, "Concat(NULL, str) returns NULL");
    TEST(StringConcat(s1, NULL) == NULL, "Concat(str, NULL) returns NULL");
    
    StringDestroy(s1);
    StringDestroy(s2);
    StringDestroy(s4);
    StringDestroy(s5);
    StringDestroy(s7);
    StringDestroy(s8);
}

// ===== ТЕСТЫ STRINGSUBSTRING =====
static void test_substring(void) {
    printf("\n=== StringSubstring ===\n");
    
    String* s = StringCreate("Hello World");
    
    //латиница
    String* sub1 = StringSubstring(s, 0, 5);
    char* cstr = StringToCstr(sub1);
    TEST(strcmp(cstr, "Hello") == 0, "Latin substring works");
    free(cstr);
    StringDestroy(sub1);
    
    //русские (по байтам)
    String* s2 = StringCreate("привет мир");
    String* sub2 = StringSubstring(s2, 0, 6);  
    TEST(sub2 != NULL, "Russian substring works");
    StringDestroy(sub2);
    
    //пустая подстрока
    String* sub3 = StringSubstring(s, 3, 3);
    TEST(sub3 != NULL, "Empty substring created");
    TEST(StringLength(sub3) == 0, "Empty substring length = 0");
    StringDestroy(sub3);
    
    //ошибки
    TEST(StringSubstring(s, 100, 200) == NULL, "Start > length returns NULL");
    TEST(StringSubstring(s, 5, 3) == NULL, "Start > end returns NULL");
    TEST(StringSubstring(NULL, 0, 5) == NULL, "NULL string returns NULL");
    
    StringDestroy(s);
    StringDestroy(s2);
}

// ===== ТЕСТЫ STRINGSPLIT =====
static void test_split(void) {
    printf("\n=== StringSplit ===\n");
    
    //латиница
    String* s1 = StringCreate("Hello world from C");
    Collection* words1 = StringSplit(s1, " ");
    TEST(words1 != NULL, "Split Latin works");
    TEST(CollectionSize(words1) == 4, "4 words");
    CollectionDestroy(words1);
    
    //русские (разделитель - пробел)
    String* s2 = StringCreate("привет мир из C");
    Collection* words2 = StringSplit(s2, " ");
    TEST(words2 != NULL, "Split Russian works");
    TEST(CollectionSize(words2) == 4, "4 Russian words");
    CollectionDestroy(words2);
    
    //разные разделители
    String* s3 = StringCreate("one,two;three:four");
    Collection* words3 = StringSplit(s3, ",;:");
    TEST(words3 != NULL, "Multiple delimiters work");
    TEST(CollectionSize(words3) == 4, "4 words");
    CollectionDestroy(words3);
    
    //ведущие разделители
    String* s4 = StringCreate("  hello  world  ");
    Collection* words4 = StringSplit(s4, " ");
    TEST(words4 != NULL, "Leading spaces handled");
    TEST(CollectionSize(words4) == 2, "2 words (spaces ignored)");
    CollectionDestroy(words4);
    
    //пустая строка
    String* empty = StringCreate("");
    Collection* words5 = StringSplit(empty, " ");
    TEST(words5 != NULL, "Split empty string works");
    TEST(CollectionSize(words5) == 0, "Empty result");
    CollectionDestroy(words5);
    
    //ошибки
    TEST(StringSplit(NULL, " ") == NULL, "Split NULL string returns NULL");
    TEST(StringSplit(s1, NULL) == NULL, "Split NULL delimiters returns NULL");
    
    StringDestroy(s1);
    StringDestroy(s2);
    StringDestroy(s3);
    StringDestroy(s4);
    StringDestroy(empty);
}

// ===== ТЕСТЫ STRINGTOCSTR =====
static void test_to_cstr(void) {
    printf("\n=== StringToCstr ===\n");
    
    String* s1 = StringCreate("Hello");
    char* cstr1 = StringToCstr(s1);
    TEST(strcmp(cstr1, "Hello") == 0, "Latin conversion works");
    free(cstr1);
    
    String* s2 = StringCreate("привет");
    char* cstr2 = StringToCstr(s2);
    TEST(cstr2 != NULL, "Russian conversion works");
    free(cstr2);
    
    String* s3 = StringCreate("😀");
    char* cstr3 = StringToCstr(s3);
    TEST(cstr3 != NULL, "Emoji conversion works");
    free(cstr3);
    
    TEST(StringToCstr(NULL) == NULL, "NULL returns NULL");
    
    StringDestroy(s1);
    StringDestroy(s2);
    StringDestroy(s3);
}

//====== ТЕСТЫ STRINGPRINT ======
static void test_print(void) {
    printf("\n=== StringPrint ===\n");
    
    String* s1 = StringCreate("Hello");
    printf("  Latin: ");
    StringPrint(s1);
    printf("\n");
    
    String* s2 = StringCreate("привет");
    printf("  Russian: ");
    StringPrint(s2);
    printf("\n");
    
    String* s3 = StringCreate("😀🎉");
    printf("  Emoji: ");
    StringPrint(s3);
    printf("\n");
    
    TEST(1, "Print works without crash");
    
    StringPrint(NULL);
    TEST(1, "Print NULL does nothing");
    
    StringDestroy(s1);
    StringDestroy(s2);
    StringDestroy(s3);
}

int main()
{
    printf("========================================\n");
    printf("MODULE TESTS (Option 25)\n");
    printf("========================================\n");
    
    test_create();
    test_destroy();
    test_length();
    test_char_at();
    test_concat();
    test_substring();
    test_split();
    test_to_cstr();
    test_print();
    
    printf("\n========================================\n");
    printf("TEST RESULTS:\n");
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("========================================\n");
    
    return tests_failed ? 1 : 0;
}