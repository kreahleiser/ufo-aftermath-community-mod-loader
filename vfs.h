#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int vfs_extract_file(const char *archive, const char *inner_path, const char *dest_dir, char *err, int err_n);
int vfs_pack_dir(const char *src_dir, const char *out_archive, char *err, int err_n);

#ifdef __cplusplus
}
#endif
