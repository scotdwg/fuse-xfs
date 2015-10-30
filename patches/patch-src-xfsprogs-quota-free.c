$NetBSD$

Port to NetBSD.

--- src/xfsprogs/quota/free.c
+++ src/xfsprogs/quota/free.c
@@ -56,7 +56,7 @@ mount_free_space_data(
 {
 	struct xfs_fsop_counts	fscounts;
 	struct xfs_fsop_geom	fsgeo;
-	struct statfs		st;
+	struct statvfs		st;
 	__uint64_t		logsize, count, free;
 	int			fd;
 
