CFLAGS=`pkg-config --cflags feedback glib-2.0` -Wall -g
LDFLAGS=`pkg-config --libs feedback glib-2.0`

TARGETS=capi_autofeedback capi_feedback getdata

all: $(TARGETS)

$(TARGETS): % : %.c
	$(CC) -o $@ $@.c $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGETS)
