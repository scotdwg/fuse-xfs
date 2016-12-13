VERSION=0.2.1

BUILD=$(TOPLEVEL)/build
SRC=$(TOPLEVEL)/src
BINS=$(BUILD)/bin
LIBS=$(BUILD)/lib
OBJECTS=$(BUILD)/obj

CC=cc
FUSE_CFLAGS=-D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26
FUSE_LDFLAGS=-L/usr/local/lib -lfuse -Bstatic
FUSE_INCLUDES=-I /usr/local/include/osxfuse
XFSUTILS_INCLUDES=-I $(SRC)/xfsutil
XFS_INCLUDES=-I $(SRC)/xfsprogs/include -I/usr/local/include

PROGRAMS=xfs-cli xfs-rcopy fuse-xfs mkfs.xfs
PROGRAMS:=$(addprefix $(BINS)/, $(PROGRAMS))

DMG=$(BUILD)/fuse-xfs-$(VERSION).dmg
