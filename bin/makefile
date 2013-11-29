include ../lib/config.mk

#CC += -faddress-sanitizer -fno-omit-frame-pointer
LDFLAGS := -L"../lib" -L. -lccv $(LDFLAGS)
CFLAGS := -O2 -Wall -I"../lib" $(CFLAGS)

TARGETS = bbffmt msermatch siftmatch bbfcreate bbfdetect swtcreate swtdetect swtdetect-dir dpmcreate dpmdetect convert tld icfcreate icfdetect icfoptimize cifar-10

all: libccv.a $(TARGETS)

clean:
	${MAKE} clean -C ../lib ; rm -f *.o $(TARGETS)

$(TARGETS): %: %.o libccv.a
	$(CC) -o $@ $< $(LDFLAGS)

libccv.a:
	${MAKE} -C ../lib

%.o: %.c ../lib/ccv.h
	$(CC) $< -o $@ -c $(CFLAGS) $(LDFLAGS) -L/usr/lib/atlas -L/usr/lib64/atlas
