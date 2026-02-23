# ==================== CONFIGURATION ====================
PROJECT_NAME 		:= VectorLibrary

ifeq ($(OS),Windows_NT)
    PROJECT_NAMESPACE := $(PROJECT_NAME)
    PROJECT_NAMESPACE := $(shell powershell -Command "& { '$(PROJECT_NAME)'.ToLower() }" 2>nul || echo $(PROJECT_NAME))
else
    PROJECT_NAMESPACE := $(shell echo $(PROJECT_NAME) | tr A-Z a-z)
endif

PROJECT_VERSION := 1.0.1

# ==================== OS DETECTION ====================
ifeq ($(OS),Windows_NT)
    HOST_OS 	:= Windows
    RM 			= del /Q /F
    RMDIR 		= rmdir /S /Q
    MKDIR 		= mkdir
    CP 			= copy /Y
    MV 			= move /Y
    PATH_SEP 	= \\
    NULL 		= NUL
    ifneq ($(shell echo %MSYSTEM% 2>nul),)
        IS_MSYS := true
        RM 		= rm -f
        RMDIR 	= rm -rf
        MKDIR 	= mkdir -p
        CP 		= cp
        MV 		= mv
    else
        IS_MSYS := false
    endif
else
    HOST_OS 	:= $(shell uname -s)
    RM 			= rm -f
    RMDIR 		= rm -rf
    MKDIR 		= mkdir -p
    CP 			= cp
    MV 			= mv
    PATH_SEP 	= /
    NULL 		= /dev/null
endif

# ==================== COMPILER ====================
CC 		?= gcc
AR 		?= ar
CFLAGS 	:= -std=c90 -Wall -Wextra -pedantic
ARFLAGS := rcs

# ==================== OPTIONS ====================
VECTOR_CHECK_ON 		?= OFF
VECTOR_LITE 			?= OFF
VECTOR_OPTIMIZE_SIZE	?= OFF
VECTOR_SMALL_MEMORY 	?= OFF
VECTOR_RUN_GENERATOR 	?= ON
VECTOR_BUILD_TESTS 		?= OFF

# ==================== DIRECTORIES ====================
SRC_DIR 	:= src
INCLUDE_DIR := include/$(PROJECT_NAMESPACE)
GEN_DIR 	:= $(SRC_DIR)/gen
PRIV_DIR 	:= $(SRC_DIR)/priv
SCRIPT_DIR 	:= script
TEST_DIR 	:= $(SRC_DIR)/test
DOC_DIR 	:= doc
LOG_DIR 	:= log
BUILD_DIR 	:= build

# ==================== INSTALLATION PATHS ====================
ifeq ($(HOST_OS),Windows)
    INSTALL_DIR 		?= $(ProgramFiles)/$(PROJECT_NAME)
    INSTALL_DIR_ESCAPED := $(subst $(SPACE),\$(SPACE),$(INSTALL_DIR))
else
    INSTALL_DIR 		?= /usr/local
    INSTALL_DIR_ESCAPED := $(INSTALL_DIR)
endif

# ==================== GENERATOR SELECTION ====================
exists = $(shell test -f $(1) && echo yes)

GEN_SCRIPT_PY := $(SCRIPT_DIR)/vector_gen.py
GEN_SCRIPT_BAT := $(SCRIPT_DIR)/vector_gen.bat
GEN_SCRIPT_SH := $(SCRIPT_DIR)/vector_gen.sh

PYTHON := $(shell command -v python3 2>/dev/null || command -v python 2>/dev/null)
ifdef PYTHON
    ifeq ($(call exists,$(GEN_SCRIPT_PY)),yes)
        GEN_COMMAND := $(PYTHON) $(GEN_SCRIPT_PY)
        GEN_TYPE := python
        $([INFO] Using Python generator: $(GEN_SCRIPT_PY))
    endif
endif

ifeq ($(GEN_TYPE),)
    ifeq ($(HOST_OS),Windows)
        ifeq ($(shell if exist $(GEN_SCRIPT_BAT) echo yes 2>nul),yes)
            GEN_COMMAND := $(GEN_SCRIPT_BAT)
            GEN_TYPE := batch
            $([INFO] Using Batch generator: $(GEN_SCRIPT_BAT))
        endif
    else
        BASH := $(shell command -v bash 2>/dev/null)
        ifdef BASH
            ifeq ($(call exists,$(GEN_SCRIPT_SH)),yes)
                GEN_COMMAND := $(BASH) $(GEN_SCRIPT_SH)
                GEN_TYPE := bash
                $([INFO] Using Bash generator: $(GEN_SCRIPT_SH))
            endif
        endif
    endif
endif

ifndef GEN_COMMAND
    $([WARNING] No generator found! Generated files may be missing.)
    GENERATOR_AVAILABLE := no
else
    GENERATOR_AVAILABLE := yes
endif

# ==================== GENERATED FILES ====================
GENERATED_FILES := \
	$(GEN_DIR)/vector_aliases.h \
	$(GEN_DIR)/vector_decl.h \
	$(GEN_DIR)/vector_impl.c \
	$(DOC_DIR)/vector_types.txt \
	$(LOG_DIR)/vector_gen.txt

# ==================== SOURCE FILES ====================
LIB_SOURCES := \
	$(SRC_DIR)/vector.c \
	$(PRIV_DIR)/vector_template.c \
	$(GEN_DIR)/vector_impl.c

PUBLIC_HEADERS := \
	$(SRC_DIR)/vector.h \
	$(SRC_DIR)/vector_error.h \
	$(SRC_DIR)/vector_range.h \
	$(SRC_DIR)/vector_size.h

GEN_HEADERS := \
	$(GEN_DIR)/vector_aliases.h \
	$(GEN_DIR)/vector_decl.h

PRIV_HEADERS := \
	$(PRIV_DIR)/template.h \
	$(PRIV_DIR)/vector_initialize_type.h \
	$(PRIV_DIR)/vector_template.h

ALL_HEADERS := $(PUBLIC_HEADERS) $(GEN_HEADERS) $(PRIV_HEADERS)

ifeq ($(HOST_OS),Windows)
    ifeq ($(IS_MSYS),true)
        OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(LIB_SOURCES))
    else
        OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.obj,$(LIB_SOURCES))
    endif
else
    OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(LIB_SOURCES))
endif

# ==================== COMPILE FLAGS ====================
ifneq ($(VECTOR_CHECK_ON),OFF)
    CFLAGS += -DVECTOR_CHECK_ON
endif

ifneq ($(VECTOR_LITE),OFF)
    CFLAGS += -DVECTOR_LITE
endif

ifneq ($(VECTOR_OPTIMIZE_SIZE),OFF)
    CFLAGS += -DVECTOR_OPTIMIZE_SIZE
endif

ifneq ($(VECTOR_SMALL_MEMORY),OFF)
    CFLAGS += -DVECTOR_SMALL_MEMORY
endif

SYS_TYPES_H := $(shell $(CC) -E -xc - $(CFLAGS) 2>$(NULL) <<< "#include <sys/types.h>" && echo "YES" || echo "NO")
ifeq ($(SYS_TYPES_H),YES)
    CFLAGS += -DHAVE_SYS_TYPES_H
endif

INCLUDES := -I$(SRC_DIR) -I$(GEN_DIR) -I$(PRIV_DIR)

ifeq ($(HOST_OS),Windows)
    ifneq ($(IS_MSYS),true)
        CFLAGS += -D_WIN32
    endif
endif

COMPILE_FLAGS := $(CFLAGS) $(INCLUDES)

# ==================== PHONY TARGETS ====================
.PHONY: all clean clean_build clean_gen generate run_generator install uninstall test help info

# ==================== MAIN TARGET ====================
all: generate $(BUILD_DIR)/lib$(PROJECT_NAMESPACE).a

$(BUILD_DIR)/lib$(PROJECT_NAMESPACE).a: $(OBJECTS) | $(BUILD_DIR)
	@echo "========================================"
	@echo "Creating library: $@"
	$(AR) $(ARFLAGS) $@ $(OBJECTS)
	@echo "========================================"

ifeq ($(HOST_OS),Windows)
    ifeq ($(IS_MSYS),true)
        $(BUILD_DIR)/%.o: %.c | $(BUILD_DIR) generate
		@mkdir -p $(dir $@)
		@echo "Compiling: $<"
		$(CC) $(COMPILE_FLAGS) -c $< -o $@
    else
        $(BUILD_DIR)/%.obj: %.c | $(BUILD_DIR) generate
		@if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
		@echo Compiling: $<
		$(CC) $(COMPILE_FLAGS) -c $< -o $@
    endif
else
    $(BUILD_DIR)/%.o: %.c | $(BUILD_DIR) generate
	@mkdir -p $(dir $@)
	@echo "Compiling: $<"
	$(CC) $(COMPILE_FLAGS) -c $< -o $@
endif

$(BUILD_DIR):
	$(MKDIR) "$(BUILD_DIR)"

# ==================== GENERATION ====================
directories:
	@$(MKDIR) "$(GEN_DIR)" "$(LOG_DIR)" "$(DOC_DIR)" 2>$(NULL) || true

ifeq ($(GENERATOR_AVAILABLE),yes)
generate: directories $(GENERATED_FILES)

$(GENERATED_FILES): $(SRC_DIR)/vector.h
	@echo "========================================"
	@echo "Running vector generator..."
	@echo "   Type:    $(GEN_TYPE)"
	@echo "   Command: $(GEN_COMMAND)"
	@echo "========================================"
	$(GEN_COMMAND)
	@echo "========================================"
	@echo "Generation complete!"
	@echo "========================================"

run_generator: generate
else
generate:
	@echo "========================================"
	@echo "[WARNING] No generator available!"
	@echo ""
	@echo "Please install one of:"
	@echo "  • Python 3 with $(GEN_SCRIPT_PY)"
    ifeq ($(HOST_OS),Windows)
	@echo "  • Windows batch script: $(GEN_SCRIPT_BAT)"
    else
	@echo "  • Bash script: $(GEN_SCRIPT_SH)"
    endif
	@echo ""
	@echo "Or set VECTOR_RUN_GENERATOR=OFF to skip generation"
	@echo "========================================"
	@exit 1

run_generator:
	@echo "No generator available"
	@exit 1
endif

# ==================== DEPENDENCIES ====================
ifeq ($(HOST_OS),Windows)
    ifeq ($(IS_MSYS),true)
        -include $(OBJECTS:.o=.d)
        
        $(BUILD_DIR)/%.d: %.c | $(BUILD_DIR)
		@mkdir -p $(dir $@)
		@$(CC) $(INCLUDES) -MM -MT $(@:.d=.o) $< > $@
    else
        $(BUILD_DIR)/%.d: %.c | $(BUILD_DIR)
		@if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
		$(CC) $(INCLUDES) -MM $< | sed 's/\($*\)\.o[ :]*/$(subst /,\,$(subst .c,.obj,$<)) $@ : /g' > $@
    endif
else
    -include $(OBJECTS:.o=.d)
    
    $(BUILD_DIR)/%.d: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(INCLUDES) -MM -MT $(@:.d=.o) $< > $@
endif

# ==================== TESTS ====================
ifeq ($(VECTOR_BUILD_TESTS),ON)
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.c)

ifeq ($(HOST_OS),Windows)
    ifeq ($(IS_MSYS),true)
        TEST_TARGETS := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test_%,$(TEST_SOURCES))
        
        test: $(TEST_TARGETS)
		@echo "========================================"
		@echo "Running tests:"
		@for test in $(TEST_TARGETS); do \
			echo "  Running $$(basename $$test)"; \
			$$test; \
		done
		@echo "========================================"
        
        $(BUILD_DIR)/test_%: $(TEST_DIR)/%.c $(BUILD_DIR)/lib$(PROJECT_NAMESPACE).a | $(BUILD_DIR)
		@echo "Building test: $<"
		$(CC) $(COMPILE_FLAGS) $< -o $@ -L$(BUILD_DIR) -l$(PROJECT_NAMESPACE)
    else
        TEST_TARGETS := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test_%.exe,$(TEST_SOURCES))
        
        test: $(TEST_TARGETS)
		@echo ========================================
		@echo Running tests:
		@for %i in ($(TEST_TARGETS)) do @(echo   Running %~ni & %i)
		@echo ========================================
        
        $(BUILD_DIR)/test_%.exe: $(TEST_DIR)/%.c $(BUILD_DIR)/lib$(PROJECT_NAMESPACE).a | $(BUILD_DIR)
		@echo Building test: $<
		$(CC) $(COMPILE_FLAGS) $< -o $@ -L$(BUILD_DIR) -l$(PROJECT_NAMESPACE)
    endif
else
    TEST_TARGETS := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test_%,$(TEST_SOURCES))
    
    test: $(TEST_TARGETS)
	@echo "========================================"
	@echo "Running tests:"
	@for test in $(TEST_TARGETS); do \
		echo "  Running $$(basename $$test)"; \
		$$test; \
	done
	@echo "========================================"
    
    $(BUILD_DIR)/test_%: $(TEST_DIR)/%.c $(BUILD_DIR)/lib$(PROJECT_NAMESPACE).a | $(BUILD_DIR)
	@echo "Building test: $<"
	$(CC) $(COMPILE_FLAGS) $< -o $@ -L$(BUILD_DIR) -l$(PROJECT_NAMESPACE)
endif
else
test:
	@echo "Tests disabled (VECTOR_BUILD_TESTS=OFF)"
endif

# ==================== INSTALLATION ====================
prefix       ?= $(INSTALL_DIR)
exec_prefix  ?= $(prefix)
bindir       ?= $(exec_prefix)/bin
libdir       ?= $(exec_prefix)/lib
includedir   ?= $(prefix)/include
datarootdir  ?= $(prefix)/share

INSTALL_LIB_DIR 	:= $(libdir)
INSTALL_INCLUDE_DIR := $(includedir)/$(PROJECT_NAMESPACE)
INSTALL_CMAKE_DIR 	:= $(libdir)/cmake/$(PROJECT_NAME)

install: all
	@echo "========================================"
	@echo "Installing $(PROJECT_NAME) to $(DESTDIR)$(prefix)"
	@echo "		Library dir: $(DESTDIR)$(INSTALL_LIB_DIR)"
	@echo "		Include dir: $(DESTDIR)$(INSTALL_INCLUDE_DIR)"
	@echo "		CMake dir:   $(DESTDIR)$(INSTALL_CMAKE_DIR)"
	@echo "========================================"
	
	$(MKDIR) "$(DESTDIR)$(INSTALL_LIB_DIR)"
	$(MKDIR) "$(DESTDIR)$(INSTALL_INCLUDE_DIR)/gen"
	$(MKDIR) "$(DESTDIR)$(INSTALL_INCLUDE_DIR)/priv"
	$(MKDIR) "$(DESTDIR)$(INSTALL_CMAKE_DIR)"
	
	$(CP) "$(BUILD_DIR)/lib$(PROJECT_NAMESPACE).a" "$(DESTDIR)$(INSTALL_LIB_DIR)/"
	$(CP) $(PUBLIC_HEADERS) "$(DESTDIR)$(INSTALL_INCLUDE_DIR)/"
	$(CP) $(GEN_HEADERS) "$(DESTDIR)$(INSTALL_INCLUDE_DIR)/gen/"
	$(CP) $(PRIV_HEADERS) "$(DESTDIR)$(INSTALL_INCLUDE_DIR)/priv/"
	
	@echo "Creating CMake config files..."
	@echo "# $(PROJECT_NAME)Config.cmake" > "$(DESTDIR)$(INSTALL_CMAKE_DIR)/$(PROJECT_NAME)Config.cmake"
	@echo "get_filename_component($(PROJECT_NAME)_CMAKE_DIR \"\$${CMAKE_CURRENT_LIST_FILE}\" PATH)" >> "$(DESTDIR)$(INSTALL_CMAKE_DIR)/$(PROJECT_NAME)Config.cmake"
	@echo "get_filename_component($(PROJECT_NAME)_ROOT_DIR \"\$${$(PROJECT_NAME)_CMAKE_DIR}/..\" ABSOLUTE)" >> "$(DESTDIR)$(INSTALL_CMAKE_DIR)/$(PROJECT_NAME)Config.cmake"
	@echo "set($(PROJECT_NAME)_VERSION $(PROJECT_VERSION))" >> "$(DESTDIR)$(INSTALL_CMAKE_DIR)/$(PROJECT_NAME)Config.cmake"
	@echo "set($(PROJECT_NAME)_INCLUDE_DIRS \"\$${$(PROJECT_NAME)_ROOT_DIR}/include/$(PROJECT_NAMESPACE)\")" >> "$(DESTDIR)$(INSTALL_CMAKE_DIR)/$(PROJECT_NAME)Config.cmake"
	@echo "set($(PROJECT_NAME)_LIBRARIES \"\$${$(PROJECT_NAME)_ROOT_DIR}/lib/lib$(PROJECT_NAMESPACE).a\")" >> "$(DESTDIR)$(INSTALL_CMAKE_DIR)/$(PROJECT_NAME)Config.cmake"
	
	@echo "Installation complete!"
	@echo "========================================"

uninstall:
	@echo "========================================"
	@echo "Uninstalling $(PROJECT_NAME) from $(DESTDIR)$(prefix)"
	@echo "========================================"
	
	$(RM) "$(DESTDIR)$(INSTALL_LIB_DIR)/lib$(PROJECT_NAMESPACE).a" 2>$(NULL)
	$(RM) $(addprefix $(DESTDIR)$(INSTALL_INCLUDE_DIR)/,$(notdir $(PUBLIC_HEADERS))) 2>$(NULL)
	$(RM) $(addprefix $(DESTDIR)$(INSTALL_INCLUDE_DIR)/gen/,$(notdir $(GEN_HEADERS))) 2>$(NULL)
	$(RM) $(addprefix $(DESTDIR)$(INSTALL_INCLUDE_DIR)/priv/,$(notdir $(PRIV_HEADERS))) 2>$(NULL)
	$(RM) "$(DESTDIR)$(INSTALL_CMAKE_DIR)/$(PROJECT_NAME)Config.cmake" 2>$(NULL)
	
	-$(RMDIR) "$(DESTDIR)$(INSTALL_INCLUDE_DIR)/gen" 2>$(NULL)
	-$(RMDIR) "$(DESTDIR)$(INSTALL_INCLUDE_DIR)/priv" 2>$(NULL)
	-$(RMDIR) "$(DESTDIR)$(INSTALL_INCLUDE_DIR)" 2>$(NULL)
	-$(RMDIR) "$(DESTDIR)$(INSTALL_CMAKE_DIR)" 2>$(NULL)
	
	@echo "Uninstallation complete!"
	@echo "========================================"

# ==================== CLEANING ====================
clean_build:
	@echo "Cleaning build directory..."
	$(RMDIR) "$(BUILD_DIR)" 2>$(NULL)

clean_gen:
	@echo "Cleaning generated files..."
	$(RMDIR) "$(GEN_DIR)" "$(LOG_DIR)" "$(DOC_DIR)" 2>$(NULL)

clean: clean_build clean_gen
	@echo "All cleaned!"

# ==================== INFO ====================
info:
	@echo "========================================"
	@echo "$(PROJECT_NAME) Configuration"
	@echo "========================================"
	@echo "Project:        $(PROJECT_NAME) v$(PROJECT_VERSION)"
	@echo "Namespace:      $(PROJECT_NAMESPACE)"
	@echo "OS:             $(HOST_OS)"
	@echo "Compiler:       $(CC)"
	@echo "Flags:          $(CFLAGS)"
	@echo ""
	@echo "Generator:      $(if $(GEN_TYPE),$(GEN_TYPE),none)"
	@echo "Generator cmd:  $(if $(GEN_COMMAND),$(GEN_COMMAND),not available)"
	@echo ""
	@echo "Directories:"
	@echo "		Source:       $(SRC_DIR)"
	@echo "		Build:        $(BUILD_DIR)"
	@echo "		Generated:    $(GEN_DIR)"
	@echo "		Documentation: $(DOC_DIR)"
	@echo ""
	@echo "Options:"
	@echo "		VECTOR_CHECK_ON:       $(VECTOR_CHECK_ON)"
	@echo "		VECTOR_LITE:           $(VECTOR_LITE)"
	@echo "		VECTOR_OPTIMIZE_SIZE:  $(VECTOR_OPTIMIZE_SIZE)"
	@echo "		VECTOR_SMALL_MEMORY:   $(VECTOR_SMALL_MEMORY)"
	@echo "		VECTOR_RUN_GENERATOR:  $(VECTOR_RUN_GENERATOR)"
	@echo "		VECTOR_BUILD_TESTS:    $(VECTOR_BUILD_TESTS)"
	@echo "========================================"

# ==================== HELP ====================
help:
	@echo "========================================"
	@echo "$(PROJECT_NAME) Makefile"
	@echo "========================================"
	@echo "Targets:"
	@echo "		all			- Build the library (default)"
	@echo "		generate	- Run type generator"
	@echo "		test		- Build and run tests"
	@echo "		install		- Install library"
	@echo "		uninstall	- Remove installed files"
	@echo "		clean_build	- Remove build files"
	@echo "		clean_gen	- Remove generated files"
	@echo "		clean		- Remove all generated and built files"
	@echo "		info		- Show configuration"
	@echo "		help		- Show this help"
	@echo ""
	@echo "Options (set with VAR=value):"
	@echo "		VECTOR_CHECK_ON=ON			- Enable runtime checks"
	@echo "		VECTOR_LITE=ON				- Build lite version"
	@echo "		VECTOR_OPTIMIZE_SIZE=ON		- Optimize for size"
	@echo "		VECTOR_SMALL_MEMORY=ON		- Optimize for small memory"
	@echo "		VECTOR_RUN_GENERATOR=OFF	- Disable automatic generation"
	@echo "		VECTOR_BUILD_TESTS=ON		- Enable tests"
	@echo "		prefix=/path				- Installation prefix"
	@echo "		DESTDIR=/path				- Staging directory"
	@echo "========================================"

# ==================== DEFAULT TARGET ====================
.DEFAULT_GOAL := all