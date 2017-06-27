TARGET   = vulkanapp 

CC       = gcc
# compiling flags here
CFLAGS = -std=c11 -flto -O3 -march=native

LINKER   = gcc -o
# linking flags here
LFLAGS   = -flto -O3 -march=native -lm -ldl -lyaml

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
SOURCES  += $(wildcard $(SRCDIR)/vulkan_functions/*.c)
SOURCES  += $(wildcard $(SRCDIR)/vulkan_tools/*.c)
SOURCES  += $(wildcard $(SRCDIR)/utils/*.c)

INCLUDES := $(wildcard $(SRCDIR)/*.h)
INCLUDES += $(wildcard $(SRCDIR)/vulkan_functions/*.h)
INCLUDES += $(wildcard $(SRCDIR)/vulkan_tools/*.h)
INCLUDES += $(wildcard $(SRCDIR)/vulkan_tools/*.h)

INCLUDE_DIRS := 
LIB_DIRS     := 

OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

rm       = rm -rf

# DEFINES := -DYAML_VERSION_MAJOR=0 -DYAML_VERSION_MINOR=1 -DYAML_VERSION_PATCH=7 -DYAML_VERSION_STRING=\"1.7\"
DEFINES := -DDEBUG_LOG -DPROD_LOG -DERROR_LOG

default: $(BINDIR)/$(TARGET)
all: default

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@$(rm) $(BINDIR)/config
	@cp -r config/ $(BINDIR)/
	@$(LINKER) $@ $(LIB_DIRS) $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR) $(OBJDIR)/vulkan_tools $(OBJDIR)/vulkan_functions $(OBJDIR)/utils
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	@$(rm) $(OBJDIR)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
