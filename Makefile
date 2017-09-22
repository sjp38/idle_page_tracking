.PHONY: clean help

APPS	:= userprog pageidle
USRPRG	:= userprog
PAGEIDL	:= pageidle

IDIR	:= include
CC	:= gcc
CFLAGS	:= -g -I$(IDIR) -O3 -Wall -Werror -std=gnu99
#LIBS	:= -lpthread -lnuma -ljemalloc
ODIR	:= ./

_OBJ_UP	:= userprog.o
OBJ_UP	:= $(patsubst %,$(ODIR)/%,$(_OBJ_UP))

_OBJ_PI	:= pageidle.o
OBJ_PI	:= $(patsubst %,$(ODIR)/%,$(_OBJ_PI))


all: $(APPS)


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.s: %.c $(DEPS)
	$(CC) -S -o $@ $< $(CFLAGS)


$(USRPRG): $(OBJ_UP)
	$(CC) -o $@ $^ $(LIBS)

$(PAGEIDL): $(OBJ_PI)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.s $(APPS)


help:
	@echo "usage: make <target>"
	@echo ""
	@echo "targets: $(APPS)"
	@echo ""
