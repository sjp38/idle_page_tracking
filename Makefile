.PHONY: clean help

APPS	:= userprog pageidle
APP	:= userprog
PAGEIDL	:= pageidle

IDIR	:= include
CC	:= gcc
CFLAGS	:= -g -I$(IDIR) -O3 -Wall -Werror -std=gnu99
#LIBS	:= -lpthread -lnuma -ljemalloc
ODIR	:= ./

_OBJ	:= userprog.o
OBJ	:= $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ_PI	:= pageidle.o
OBJ_PI	:= $(patsubst %,$(ODIR)/%,$(_OBJ_PI))


all: $(APPS)


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.s: %.c $(DEPS)
	$(CC) -S -o $@ $< $(CFLAGS)


$(APP): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(PAGEIDL): $(OBJ_PI)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.s $(APPS)


help:
	@echo "usage: make <target>"
	@echo ""
	@echo "targets: $(APP)"
	@echo ""
