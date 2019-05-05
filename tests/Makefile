.PHONY: all clean

# All tests should be listed here
TESTS = aaa-crypto

# Include path should include soruce directory
CFLAGS += -I../src

all: $(TESTS)
	for i in $(TESTS); do \
		./$$i; \
	done

clean:
	$(RM) $(RMFLAGS) $(TESTS)

# Individual tests put rules below

aaa-crypto:
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(shell pkg-config --cflags --libs glib-2.0 libsodium) -o $@ aaa-crypto.c ../src/aaa-crypto.c
