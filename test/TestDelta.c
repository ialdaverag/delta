#include "unity.h"
#include "TestFile.h"

int main(void) {
    UNITY_BEGIN();
    
    // Tests para file.c
    RUN_TEST(test_File_init_with_valid_file);
    RUN_TEST(test_File_free);
    
    return UNITY_END();
}