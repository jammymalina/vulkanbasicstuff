TARGET   = vulkanapp 

CC       = gcc
# compiling flags here
# CFLAGS = -std=c11 -flto -O3 -march=native 
CFLAGS = -std=c11 -Wall -g3

LINKER   = gcc -o
# linking flags here
LFLAGS   = -flto -O3 -march=native -lm -ldl -lyaml -lSDL2 -lX11 -lX11-xcb -lxcb

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
SOURCES  += $(wildcard $(SRCDIR)/vulkan_functions/*.c)
SOURCES  += $(wildcard $(SRCDIR)/vulkan_tools/*.c)
SOURCES  += $(wildcard $(SRCDIR)/utils/*.c)
SOURCES  += $(wildcard $(SRCDIR)/window/*.c)

INCLUDES := $(wildcard $(SRCDIR)/*.h)
INCLUDES += $(wildcard $(SRCDIR)/vulkan_functions/*.h)
INCLUDES += $(wildcard $(SRCDIR)/vulkan_tools/*.h)
INCLUDES += $(wildcard $(SRCDIR)/vulkan_tools/*.h)
INCLUDES += $(wildcard $(SRCDIR)/window/*.h)

INCLUDE_DIRS := 
LIB_DIRS     := 

OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

rm       = rm -rf

DEFINES := -DDEBUG_LOG -DPROD_LOG -DERROR_LOG -DVK_USE_PLATFORM_XCB_KHR

default: $(BINDIR)/$(TARGET)
all: default

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@$(rm) $(BINDIR)/config
	@cp -r config/ $(BINDIR)/
	@$(LINKER) $@ $(LIB_DIRS) $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@export SDL_VIDEODRIVER=x11
	@mkdir -p $(OBJDIR) $(OBJDIR)/vulkan_tools $(OBJDIR)/vulkan_functions $(OBJDIR)/utils $(OBJDIR)/window
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
