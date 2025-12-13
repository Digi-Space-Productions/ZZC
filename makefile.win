# Compiler and linker
CC	  := wcc386
LINK	:= wlink
WATCOM  := $(WATCOM_INSTALL)

# Directories
SRC_DIR := src
WIN_DIR := src/win
ZZT_DIR := src/libzzt2
OUT_DIR := buildWin

# Sources and objects
SRCS := $(wildcard $(SRC_DIR)/*.c) \
		$(wildcard $(WIN_DIR)/*.c) \
		$(wildcard $(ZZT_DIR)/*.c)

OBJS := $(patsubst %.c,$(OUT_DIR)/%.obj,$(notdir $(SRCS)))


# Output binary
TARGET := $(OUT_DIR)/ZZC.EXE

# Flags
CFLAGS := -q -za99 -I$(SRC_DIR) -I$(WIN_DIR) -I$(WATCOM)/h -I$(WATCOM)/h/nt -D_WINDOWS

# Default rule
all: $(TARGET)

# Link final executable with WLINK
$(TARGET): $(OBJS)
	@echo Linking $@
	$(LINK) SYSTEM NT OP q \
		NAME $@ \
		FILE { $^ } \
		LIBPATH $(WATCOM)/lib386/nt \
		LIBPATH $(WATCOM)/lib386 \
		LIBPATH $(WATCOM)/lib

# Compile each .c into .obj inside buildWin/
$(OUT_DIR)/%.obj: $(SRC_DIR)/%.c
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -fo=$@ $<

$(OUT_DIR)/%.obj: $(WIN_DIR)/%.c
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -fo=$@ $<

$(OUT_DIR)/%.obj: $(ZZT_DIR)/%.c
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -fo=$@ $<

# Clean
clean:
	rm -rf $(OUT_DIR)/*
	cp TOWN.ZZT $(OUT_DIR)/TOWN.ZZT

.PHONY: all clean
