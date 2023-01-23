# Based on the excellent article at:
# https://latedev.wordpress.com/2014/11/08/generic-makefiles-with-gcc-and-gnu-make/

# Variables:

CC     := gcc
LINKER := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic
DEBUG  := -ggdb

TARGET := cog
SRCDIR := src
INCDIR := include
OBJDIR := obj

SRCFILES := $(wildcard $(SRCDIR)/*.c)
OBJFILES := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))
DEPFILES := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.dat,$(SRCFILES))

# Recipes:

all: $(TARGET)

reset: clean $(TARGET)

$(TARGET): $(OBJFILES)
	$(LINKER) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(DEBUG) -I $(INCDIR) -c $< -o $@

$(OBJDIR)/%.dat: $(SRCDIR)/%.c
	$(CC) -I $(INCDIR) -MM $< > $@

-include $(DEPFILES)

clean:
	$(RM) $(TARGET) $(OBJDIR)/*

setup:
	mkdir $(SRCDIR) $(OBJDIR) $(INCDIR)
