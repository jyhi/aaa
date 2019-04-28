# Resource bundles to compile to
OBJS = aaa-resources.o

# Packages (dependencies) used
PKGS = gio-2.0

# Insert dependency flags
CFLAGS += $(shell pkg-config --cflags $(PKGS))

.PHONY: all clean

all: lib.a

clean:
	$(RM) $(RMFLAGS) lib.a $(OBJS) $(OBJS:.o=.c)

lib.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

### Rules for building resource bundles below ###

aaa-resources.c: aaa-resources.gresource.xml $(shell glib-compile-resources --generate-dependencies aaa-resources.gresource.xml)
	glib-compile-resources --generate-source $<
