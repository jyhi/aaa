# Name of the final executable
export BINNAME = aaa

# Tools
export CC = gcc
export LD = $(CC)
export AR = ar
export RM = rm

# Flags to be passed
export CFLAGS   = -std=c99 -pipe -fstack-protector-all
export CPPFLAGS =
export LDFLAGS  =
export ARFLAGS  = rcs
export RMFLAGS  = -f

# Turn on all warnings
# ... except clang, who yells at glib...
# ifeq ($(CC), clang)
# CFLAGS += -Weverything -Wno-documentation -Wno-documentation-pedantic -Wno-reserved-id-macro -Wno-used-but-marked-unused -Wno-padded
# else
# CFLAGS += -Wall -Wextra -Wpedantic
# endif

# Generate optimized code for release, otherwise for debug
ifdef RELEASE
CFLAGS   += -O2 -flto
CPPFLAGS += -DNDEBUG -D_FORTIFY_SOURCE=2
LDFLAGS  += -Wl,-O1,--sort-common,--as-needed -flto
else
CFLAGS += -g
endif

# Packages (dependencies) used in link time
PKGS = gtk+-3.0 gio-2.0 json-glib-1.0 libsodium

# Insert dependency flags
LDFLAGS += $(shell pkg-config --libs $(PKGS))

# Resource directory and source directory
RESDIR = res
SRCDIR = src

.PHONY: all clean $(RESDIR) $(SRCDIR)

all: $(BINNAME)

clean:
	for dir in $(RESDIR) $(SRCDIR); do \
		$(MAKE) clean -C $$dir; \
	done
	$(RM) $(RMFLAGS) $(BINNAME)

$(RESDIR):
	$(MAKE) -C $@

$(SRCDIR):
	$(MAKE) -C $@

$(BINNAME): $(RESDIR) $(SRCDIR)
# FIXME:
# Archive of the resource bundle(s) is not used, since if the archive is used
# then GTK will be weirdly not able to locate the resource.
# $(LD) $(LDFLAGS) -o $@ $(foreach dir,$(RESDIR) $(SRCDIR),$(dir)/lib.a)
	$(LD) $(LDFLAGS) -o $@ $(RESDIR)/aaa-resources.o $(SRCDIR)/lib.a
