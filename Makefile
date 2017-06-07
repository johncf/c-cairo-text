PKGS = harfbuzz cairo-ft freetype2

CFLAGS = `pkg-config --cflags $(PKGS)`
LDFLAGS = `pkg-config --libs $(PKGS)` -lm

all: hello-harfbuzz

%: %.c
	$(CC) -std=c11 -o $@ $^ $(CFLAGS) $(LDFLAGS)
