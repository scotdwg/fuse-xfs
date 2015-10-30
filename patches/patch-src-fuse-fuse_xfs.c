$NetBSD$

Hi there!

--- src/fuse/fuse_xfs.c
+++ src/fuse/fuse_xfs.c
@@ -215,7 +215,7 @@ fuse_xfs_statfs(const char *path, struct statvfs *stbuf) {
     stbuf->f_ffree = mount->m_sb.sb_ifree + mount->m_maxicount - mount->m_sb.sb_icount;
     stbuf->f_favail = stbuf->f_ffree;
     stbuf->f_namemax = MAXNAMELEN;
-    stbuf->f_fsid = *((unsigned long*)mount->m_sb.sb_uuid);
+    stbuf->f_fsid = *((unsigned long*)&mount->m_sb.sb_uuid);
     log_debug("f_bsize=%ld\n", stbuf->f_bsize);
     log_debug("f_frsize=%ld\n", stbuf->f_frsize);
     log_debug("f_blocks=%d\n", stbuf->f_blocks);
