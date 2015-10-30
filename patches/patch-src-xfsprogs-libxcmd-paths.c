$NetBSD$

Port to NetBSD.

--- src/xfsprogs/libxcmd/paths.c
+++ src/xfsprogs/libxcmd/paths.c
@@ -240,7 +240,7 @@ static int
 fs_table_initialise_mounts(
 	char		*path)
 {
-	struct statfs	*stats;
+	struct statvfs	*stats;
 	char		*dir, *fsname, *fslog, *fsrt;
 	int		i, count, error, found;
 
