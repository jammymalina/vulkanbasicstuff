TARGET   = vulkanapp 

CC       = gcc
# compiling flags here
# CFLAGS = -std=c11 -flto -O3 -march=native 
CFLAGS = -std=c11 -Wall -g3

LINKER   = gcc -o
# linking flags here
LFLAGS   = -flto -O3 -march=native -lm -ldl -lyaml -lSDL2 -lX11 -lX11-xcb -lxcb

GLSL_CC = glslc
GLSL_FLAGS =

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SHADER_DIR = shaders
SHADER_SRC_DIR = $(SRCDIR)/$(SHADER_DIR)
SHADER_OBJ_DIR = $(BINDIR)/$(SHADER_DIR)

SOURCES  := $(wildcard $(SRCDIR)/*.c $(SRCDIR)/**/*.c)

SHADER_SOURCES := $(wildcard $(SHADER_SRC_DIR)/basic/*.vert $(SHADER_SRC_DIR)/basic/*.frag)

INCLUDE_DIRS := 
LIB_DIRS     := 

OBJECTS         := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
SHADER_OBJECTS  := $(SHADER_SOURCES:$(SHADER_SRC_DIR)/%=$(SHADER_OBJ_DIR)/%.svm)

rm       = rm -rf

DEFINES := -DDEBUG_LOG -DPROD_LOG -DERROR_LOG -DVK_USE_PLATFORM_XCB_KHR

default: $(BINDIR)/$(TARGET)
all: default

$(BINDIR)/$(TARGET): $(OBJECTS) $(SHADER_OBJECTS)
	@mkdir -p $(BINDIR)
	@$(rm) $(BINDIR)/config
	@cp -r config/ $(BINDIR)/
	@$(LINKER) $@ $(LIB_DIRS) $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@export SDL_VIDEODRIVER=x11
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(SHADER_OBJECTS): $(SHADER_OBJ_DIR)/%.svm : $(SHADER_SRC_DIR)/%
	@mkdir -p $(SHADER_OBJ_DIR)/basic
	@$(GLSL_CC) $(GLSL_FLAGS) $< -o $@
	@echo "Compiled "$<" successfully!"


.PHONEY: clean
clean:
	@$(rm) $(OBJDIR)
	@$(rm) $(SHADER_OBJ_DIR)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
