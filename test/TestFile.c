#include "TestFile.h"

static void test_File_init_with_valid_file() {
    File file;
    const char* test_file = "test_valid.delta";
    
    // Crear archivo de prueba
    FILE* fp = fopen(test_file, "w");
    fprintf(fp, "contenido de prueba");
    fclose(fp);
    
    // Ejecutar función bajo test
    File_init(&file, test_file);
    
    // Verificaciones
    TEST_ASSERT_NOT_NULL(file.content);
    TEST_ASSERT_EQUAL_STRING("contenido de prueba", file.content);
    
    // Limpieza
    File_free(&file);
    remove(test_file);
}

void test_File_init() {
    test_File_init_with_valid_file();
}

void test_File_free() {
    File file;
    const char* test_file = "test_valid.delta";
    
    // Crear archivo de prueba
    FILE* fp = fopen(test_file, "w");
    fprintf(fp, "contenido de prueba");
    fclose(fp);
    
    // Inicializar archivo
    File_init(&file, test_file);
    
    // Ejecutar función bajo test
    File_free(&file);
    
    // Verificaciones
    TEST_ASSERT_NULL(file.filename);
    TEST_ASSERT_NULL(file.content);
    TEST_ASSERT_EQUAL(0, file.length);
    
    // Limpieza
    remove(test_file);
}