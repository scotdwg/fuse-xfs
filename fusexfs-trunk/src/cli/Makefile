TOPLEVEL=../..

include ../Make.inc

LDFLAGS=-lreadline

xfs-cli: $(BINS)/xfs-cli

xfs-rcopy: $(BINS)/xfs-rcopy

$(OBJECTS)/cli.o: cli.c $(SRC)/xfsutil/xfsutil.h
	$(CC) $(XFS_INCLUDES) $(XFSUTILS_INCLUDES) -o $(OBJECTS)/cli.o -c cli.c

$(OBJECTS)/rcopy.o: rcopy.c $(SRC)/xfsutil/xfsutil.h
	$(CC) $(XFS_INCLUDES) $(XFSUTILS_INCLUDES) -o $(OBJECTS)/rcopy.o -c rcopy.c

$(BINS)/xfs-cli: $(OBJECTS)/cli.o $(OBJECTS)/xfsutil.o $(LIBS)/libxfs.a
	$(CC) $(LDFLAGS) -o $(BINS)/xfs-cli $(OBJECTS)/cli.o $(OBJECTS)/xfsutil.o $(LIBS)/libxfs.a

$(BINS)/xfs-rcopy: $(OBJECTS)/rcopy.o $(OBJECTS)/xfsutil.o $(LIBS)/libxfs.a
	$(CC) -o $(BINS)/xfs-rcopy $(OBJECTS)/rcopy.o $(OBJECTS)/xfsutil.o $(LIBS)/libxfs.a

clean:

