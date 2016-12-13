TOPLEVEL=..

include Make.inc

all: $(PROGRAMS)

$(LIBS):
	mkdir -p $(LIBS)

$(BINS):
	mkdir -p $(BINS)

$(OBJECTS):
	mkdir -p $(OBJECTS)

$(LIBS)/libxfs.a: $(LIBS) $(BINS)
	$(MAKE) -C xfsprogs
	for LIB in libdisk libxcmd libxlog libxfs; do\
		echo $$LIB;\
		ln -sf $(PWD)/xfsprogs/$$LIB/.libs/$$LIB.0.dylib $(LIBS)/;\
		ln -sf $(PWD)/xfsprogs/$$LIB/.libs/$$LIB.a $(LIBS)/;\
		ln -sf $(PWD)/xfsprogs/$$LIB/.libs/$$LIB.la $(LIBS)/;\
		ln -sf $(PWD)/xfsprogs/$$LIB/.libs/$$LIB.dylib $(LIBS)/;\
	done

$(BINS)/mkfs.xfs: $(LIBS)/libxfs.a $(BINS)
	cp $(PWD)/xfsprogs/mkfs/mkfs.xfs $(BINS)/

$(OBJECTS)/xfsutil.o: $(OBJECTS)
	$(MAKE) -C xfsutil

$(BINS)/xfs-cli: $(LIBS)/libxfs.a $(OBJECTS)/xfsutil.o $(BINS)
	$(MAKE) -C cli xfs-cli

$(BINS)/xfs-rcopy: $(LIBS)/libxfs.a $(OBJECTS)/xfsutil.o $(BINS)
	$(MAKE) -C cli xfs-rcopy

$(BINS)/fuse-xfs: $(LIBS)/libxfs.a $(OBJECTS)/xfsutil.o $(BINS)
	$(MAKE) -C fuse

$(BUILD)/fuse-xfs-$(VERSION).dmg: $(PROGRAMS)
	$(MAKE) -C macosx

clean:
	rm -rf $(BINS)/*
	rm -rf $(LIBS)/*
	rm -rf $(OBJECTS)/*
	$(MAKE) -C cli clean
	$(MAKE) -C fuse clean
	$(MAKE) -C xfsutil clean
	$(MAKE) -C xfsprogs clean
	$(MAKE) -C macosx clean
