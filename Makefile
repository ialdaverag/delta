# Configuración del Sistema Operativo
ifeq ($(OS), Windows_NT)
    ifeq ($(shell uname -s),)
        CLEANUP = del /F /Q
        MKDIR = mkdir
        ECHO = echo
        FIND_PASS = findstr /C:"PASS"
        FIND_FAIL = findstr /C:"FAIL"
        FIND_IGNORE = findstr /C:"IGNORE"
    else
        CLEANUP = rm -f
        MKDIR = mkdir -p
        ECHO = echo -e
        FIND_PASS = grep "PASS"
        FIND_FAIL = grep "FAIL"
        FIND_IGNORE = grep "IGNORE"
    endif

    TARGET_EXTENSION = .exe
else
    CLEANUP = rm -f
    MKDIR = mkdir -p
    ECHO = echo -e
    FIND_PASS = grep "PASS"
    FIND_FAIL = grep "FAIL"
    FIND_IGNORE = grep "IGNORE"
    TARGET_EXTENSION = .out
endif

# Directorios
PATHU = lib/unity
PATHS = src/
PATHT = test/
PATHB = build/
PATHO = build/objs/
PATHBIN = build/bin/

# Creación automática de directorios
$(shell $(MKDIR) $(PATHB) 2>nul)
$(shell $(MKDIR) $(PATHO) 2>nul)
$(shell $(MKDIR) $(PATHBIN) 2>nul)

# Configuración del compilador
CC = gcc
CFLAGS = -I. -I$(PATHS) -I$(PATHU) -Wall
TEST_FLAGS = $(CFLAGS)

# Fuentes
SRCS = $(wildcard $(PATHS)*.c)
OBJS = $(patsubst $(PATHS)%.c,$(PATHO)%.o,$(SRCS))
TARGET = $(PATHBIN)delta$(TARGET_EXTENSION)

# Tests - Solo TestDelta.c necesita ser ejecutable
TEST_SRCS = $(wildcard $(PATHT)*.c)
TEST_OBJS = $(patsubst $(PATHT)%.c,$(PATHO)%.o,$(TEST_SRCS))
TEST_RUNNER = $(PATHBIN)TestDelta$(TARGET_EXTENSION)  # Solo un ejecutable de test

# Reglas principales
.PHONY: program test clean

program: $(TARGET)
	@$(ECHO) "\n\xF0\x9F\x92\xBE Programa principal construido: $(TARGET)"

test: $(TEST_RUNNER)
	@$(ECHO) "\n\xF0\x9F\xA7\xAA Ejecutando Tests..."
    
    # Ejecutar los tests y mostrar la salida directamente
	@$(TEST_RUNNER)

clean:
	@$(ECHO) "\n\xF0\x9F\x97\x91 Limpiando..."
	$(CLEANUP) $(PATHO)*.o
	$(CLEANUP) $(PATHBIN)*$(TARGET_EXTENSION)

# Reglas de compilación
$(PATHO)%.o: $(PATHS)%.c
	@$(ECHO) "\n\xF0\x9F\x94\xA7 Compilando $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(PATHO)%.o: $(PATHT)%.c
	@$(ECHO) "\n\xF0\x9F\x94\xA7 Compilando test $<"
	$(CC) $(TEST_FLAGS) -c $< -o $@

$(PATHO)unity.o: $(PATHU)/unity.c
	@$(ECHO) "\n\xF0\x9F\x94\xA7 Compilando Unity"
	$(CC) $(CFLAGS) -c $< -o $@

# Reglas de enlazado
$(TARGET): $(OBJS)
	@$(ECHO) "\n\xF0\x9F\x94\x97 Enlazando programa principal"
	$(CC) $(LINK_FLAGS) $^ -o $@

$(TEST_RUNNER): $(PATHO)TestDelta.o $(PATHO)TestFile.o $(PATHO)unity.o $(filter-out $(PATHO)delta.o,$(OBJS))
	@$(ECHO) "\n\xF0\x9F\x94\x97 Enlazando test runner"
	$(CC) $(LINK_FLAGS) $^ -o $@