# Vala compiler
VALAC = valac

# List of C source code files to generate from Vala code
VALASRCS = aaa-addcontact.c \
	aaa-winmain.c \
	aaa-contactrow.c \
	aaa-peerinfo.c \
	aaa-chatui.c \
	aaa-daemon.c \
	aaa-socketservice.c \
	aaa-msgrow.c \
	app.c \
	main.c

# Object files
OBJS = aaa-addcontact.o \
	aaa-winmain.o \
	aaa-contactrow.o \
	aaa-peerinfo.o \
	aaa-chatui.o \
	aaa-daemon.o \
	aaa-socketservice.o \
	aaa-msgrow.o \
	aaa-configmgr.o \
	app.o \
	main.o

# Packages (dependencies) used in the project
PKGS = gtk+-3.0 json-glib-1.0 libsodium

# Insert dependency flags
CFLAGS += $(shell pkg-config --cflags $(PKGS))

# Same for vala
# NOTE: Excluding libsodium
VALACFLAGS += $(foreach pkg,$(filter-out libsodium, $(PKGS)),--pkg $(pkg))

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
