$NetBSD$

Yeah, yeah...

--- src/Make.inc
+++ src/Make.inc
@@ -6,14 +6,13 @@ BINS=$(BUILD)/bin
 LIBS=$(BUILD)/lib
 OBJECTS=$(BUILD)/obj
 
-CC=gcc-4.2
+CC=cc
 FUSE_CFLAGS=-D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26
-FUSE_LDFLAGS=-losxfuse
-FUSE_INCLUDES=-I /usr/local/include/osxfuse
+FUSE_LDLIBS=-lrefuse -lpuffs -lpthread
 XFSUTILS_INCLUDES=-I $(SRC)/xfsutil
 XFS_INCLUDES=-I $(SRC)/xfsprogs/include
 
-PROGRAMS=xfs-cli xfs-rcopy fuse-xfs mkfs.xfs
+PROGRAMS=fuse-xfs mkfs.xfs
 PROGRAMS:=$(addprefix $(BINS)/, $(PROGRAMS))
 
 DMG=$(BUILD)/fuse-xfs-$(VERSION).dmg
