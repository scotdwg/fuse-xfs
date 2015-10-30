$NetBSD$

Hi there!

--- src/fuse/main.c
+++ src/fuse/main.c
@@ -105,7 +105,7 @@ int xfs_probe(struct fuse_xfs_options* opts) {
     return 1;
 }
 
-int main(int argc, char* argv[], char* envp[], char** exec_path) {
+int main(int argc, char* argv[], char* envp[]) {
     struct fuse_xfs_options opts;
     char *fuse_argv[256];
     char fsname[20];
@@ -144,12 +144,11 @@ int main(int argc, char* argv[], char* envp[], char** exec_path) {
     }
 
     if (opts.printuuid) {
-        for (i=0; i<16; i++) {
-            printf("%02x", opts.xfs_mount->m_sb.sb_uuid[i]);
-            if ((i==3) || (i==5) || (i==7) || (i==9))
-                printf("-");
-        }
-        printf("\n");
+	char *buf;
+	uint32_t st;
+	uuid_to_string(&opts.xfs_mount->m_sb.sb_uuid, &buf, &st);
+	printf("%s\n", buf);
+	free(buf);
         libxfs_umount(opts.xfs_mount);
         return 0;
     }
