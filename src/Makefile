# Vala compiler
VALAC = valac

# List of C source code files to generate from Vala code
VALASRCS = aaa-addcontact.c \
	aaa-winmain.c \
	app.c \
	main.c

# Object files
OBJS = aaa-addcontact.o \
	aaa-winmain.o \
	app.o \
	main.o

# Packages (dependencies) used in the project
PKGS = gtk+-3.0

# Insert dependency flags
CFLAGS += $(shell pkg-config --cflags $(PKGS))

# Same for vala
VALACFLAGS += $(foreach pkg,$(PKGS),--pkg $(pkg))

# XXX: Tell Vala about GResource
GRESOURCES = ../res/aaa-resources.gresource.xml
VALACFLAGS += --gresources $(GRESOURCES)

.PHONY: all clean valagen

all: lib.a

clean:
	$(RM) $(RMFLAGS) lib.a $(OBJS) $(VALASRCS)

valagen:
	$(VALAC) $(VALACFLAGS) -C $(VALASRCS:.c=.vala)

$(VALASRCS): valagen

lib.a: $(OBJS)
	$(AR) $(ARFLAGS) lib.a $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<