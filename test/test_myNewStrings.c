#include<stdlib.h>

#include "unity.h"
#include "myNewStrings.h"
#include "unity_internals.h"

void setUp(void) {
    // Se ejecuta antes de cada test
}

void tearDown(void) {
    // Se ejecuta después de cada test
}

void string_size_gives_right_length(void) {
    TEST_ASSERT_EQUAL_INT(5, stringSize("hello"));
    TEST_ASSERT_EQUAL_INT(0, stringSize(""));
    TEST_ASSERT_EQUAL_INT(1, stringSize("a"));
}

void sameString_detects_equal_strings(void) {
    TEST_ASSERT_TRUE(sameString("hello", "hello"));
    TEST_ASSERT_TRUE(sameString("", ""));
}

void sameString_detects_different_strings(void) {
    TEST_ASSERT_FALSE(sameString("hello", "hell"));
    TEST_ASSERT_FALSE(sameString("hello", "Hello"));
    TEST_ASSERT_FALSE(sameString("test", "tests"));
    TEST_ASSERT_FALSE(sameString("hi", "HI"));
}

void stoi_parses_numbers_correctly(void) {
    TEST_ASSERT_EQUAL_INT(123, stoi("123"));
    TEST_ASSERT_EQUAL_INT(0, stoi("0"));
    TEST_ASSERT_EQUAL_INT(987654, stoi("987654"));
}

void stoi_works_with_leading_zeroes(void) {
    TEST_ASSERT_EQUAL_INT(42, stoi("00042"));
}

void toUpper_works_with_mixed_case(void) {
    char str[] = "HeLLo";
    toUpper(str);
    TEST_ASSERT_EQUAL_STRING("HELLO", str);
}

void toUpper_in_already_caps_text(void) {
    char str[] = "WORLD";
    toUpper(str);
    TEST_ASSERT_EQUAL_STRING("WORLD", str);
}

void toUpper_in_lowercase_only(void) {
    char str[] = "abc";
    toUpper(str);
    TEST_ASSERT_EQUAL_STRING("ABC", str);
}

void toUpper_in_empty_string(void) {
    char str[] = "";
    toUpper(str);
    TEST_ASSERT_EQUAL_STRING("", str);
}

void stringCopy_gives_same_strings(void){
    char str1[] = "jyeBiugvi Iu   ";
    char* str2 = malloc(sizeof(char)*stringSize(str1)+1);
    stringCopy(str1, str2);

    TEST_ASSERT_EQUAL_STRING(str1, str2);
    free(str2);
}

void stof_transforms_correctly(void){
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 3.14, stof("3.14"));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, stof("0.0"));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 45, stof("45"));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0234, stof("0.0234"));
}

void stringSplicer_splits_correctly(void){
    char str[] = "Hello World";
    
    TEST_ASSERT_EQUAL_STRING("Hello World", stringSplicer(str, 0, 10));
    TEST_ASSERT_EQUAL_STRING("Hello", stringSplicer(str, 0, 4));
    TEST_ASSERT_EQUAL_STRING("H", stringSplicer(str, 0, 0));
}

void removeCharFromString_works(void){
    char str1[] = "AcAcA";
    char str2[] = "hello world how's it going?";

    TEST_ASSERT_EQUAL_STRING("AAA", removeCharFromString(str1, 'c'));
    TEST_ASSERT_EQUAL_STRING("helloworldhow'sitgoing?", removeCharFromString(str2, ' '));
}

void insertCharInString_works(void){
    char str[] = "Hello World";

    TEST_ASSERT_EQUAL_STRING("Hello0World", insertCharInString(str, '0', 5));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(string_size_gives_right_length);
    RUN_TEST(sameString_detects_equal_strings);
    RUN_TEST(sameString_detects_different_strings);
    RUN_TEST(stoi_parses_numbers_correctly);
    RUN_TEST(stoi_works_with_leading_zeroes);
    RUN_TEST(toUpper_works_with_mixed_case);
    RUN_TEST(toUpper_in_already_caps_text);
    RUN_TEST(toUpper_in_lowercase_only);
    RUN_TEST(toUpper_in_empty_string);
    RUN_TEST(stringCopy_gives_same_strings);
    RUN_TEST(stof_transforms_correctly);
    RUN_TEST(stringSplicer_splits_correctly);
    RUN_TEST(removeCharFromString_works);
    RUN_TEST(insertCharInString_works);
    return UNITY_END();
}
