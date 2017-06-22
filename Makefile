TARGET   = vulkanapp 

CC       = gcc
# compiling flags here
CFLAGS = -std=c11 -flto -O3 -march=native

LINKER   = gcc -o
# linking flags here
LFLAGS   = -flto -O3 -march=native -lm -ldl

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
SOURCES  += $(wildcard $(SRCDIR)/vulkan_functions/*.c)
SOURCES  += $(wildcard $(SRCDIR)/vulkan_tools/*.c)

INCLUDES := $(wildcard $(SRCDIR)/*.h)
INCLUDES += $(wildcard $(SRCDIR)/vulkan_functions/*.h)
INCLUDES += $(wildcard $(SRCDIR)/vulkan_tools/*.h)

OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

rm       = rm -rf

default: $(BINDIR)/$(TARGET)
all: default

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR) $(OBJDIR)/vulkan_tools $(OBJDIR)/vulkan_functions
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	@$(rm) $(OBJDIR)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
