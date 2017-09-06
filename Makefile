.PHONY: clean help

APPS	:= userprog pagemap
APP	:= userprog
PAGEMAP	:= pagemap

IDIR	:= include
CC	:= gcc
CFLAGS	:= -g -I$(IDIR) -O3 -Wall -Werror -std=gnu99
#LIBS	:= -lpthread -lnuma -ljemalloc
ODIR	:= ./

_OBJ	:= userprog.o
OBJ	:= $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ_PM	:= pagemap.o
OBJ_PM	:= $(patsubst %,$(ODIR)/%,$(_OBJ_PM))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.s: %.c $(DEPS)
	$(CC) -S -o $@ $< $(CFLAGS)


$(APP): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(PAGEMAP): $(OBJ_PM)
	$(CC) -o $@ $^ $(LIBS)


all: $(APPS)

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.s $(APPS)


help:
	@echo "usage: make <target>"
	@echo ""
	@echo "targets: $(APP)"
	@echo ""
