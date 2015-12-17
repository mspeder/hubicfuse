#ifndef _COMMONFS_H
#define _COMMONFS_H
#define FUSE_USE_VERSION 26
#include <fuse.h>
#include "utils.h"
#include "map.h"

#define MAX_PATH_SIZE (1024 + 256 + 3)
#define THREAD_NAMELEN 16

extern size_t file_buffer_size;

typedef struct metadataItem
{
  struct stringMapItem item;
  char* value;
  size_t size;
  char* encoded;
  size_t esize;
} metadataItem;

// metatdata support
#define HEADER_TEXT_META "X-Object-Meta-"
#define HEADER_TEXT_META_XATTR HEADER_TEXT_META "Xattr-"
#define HEADER_TEXT_MTIME HEADER_TEXT_META "Mtime"
#define HEADER_TEXT_ATIME HEADER_TEXT_META "Atime"
#define HEADER_TEXT_CTIME HEADER_TEXT_META "Ctime"
#define HEADER_TEXT_MTIME_DISPLAY HEADER_TEXT_MTIME "-Display"
#define HEADER_TEXT_ATIME_DISPLAY HEADER_TEXT_ATIME "-Display"
#define HEADER_TEXT_CTIME_DISPLAY HEADER_TEXT_CTIME "-Display"
#define HEADER_TEXT_CHMOD HEADER_TEXT_META "Chmod"
#define HEADER_TEXT_UID HEADER_TEXT_META "Uid"
#define HEADER_TEXT_GID HEADER_TEXT_META "Gid"
#define HEADER_TEXT_FILEPATH HEADER_TEXT_META "FilePath"
#define TEMP_FILE_NAME_FORMAT "%s/.cloudfuse_%s"
#define HUBIC_DATE_FORMAT "%Y-%m-%d %T."

//linked list with files in a directory
typedef struct dir_entry
{
  char *name;
  char *full_name;
  char *content_type;
  off_t size;
  time_t last_modified;
  // implement utimens
  struct timespec mtime;
  struct timespec ctime;
  struct timespec atime;
  char *md5sum; //interesting capability for rsync/scrub
  mode_t chmod;
  uid_t uid;
  gid_t gid;
  stringMap* xattrs;
  stringMap* raw_xattrs;
  stringMap* unknown_metas;
  bool issegmented;
  time_t accessed_in_cache;//todo: cache support based on access time
  bool metadata_downloaded;
  // end change
  int isdir;
  int islink;
  struct dir_entry *next;
} dir_entry;

// linked list with cached folder names
typedef struct dir_cache
{
  char *path;
  dir_entry *entries;
  time_t cached;
  //added cache support based on access time
  time_t accessed_in_cache;
  bool was_deleted;
  //end change
  struct dir_cache *next, *prev;
} dir_cache;

metadataItem* metadata_new();
metadataItem* metadata_new_encoded(const char* value);
metadataItem* metadata_new_xattr(const char* key, const char* value,
                                 size_t size);
metadataItem* metadata_new_xattr_encoded(const char* value, char** key);
void metadata_copy(const char* key, void* item, void* param);
void metadata_delete(void* item);

bool is_unknown_meta(const char* head);

time_t my_timegm(struct tm* tm);
time_t get_time_from_str_as_gmt(char* time_str);
time_t get_time_as_local(time_t time_t_val, char time_str[],
                         int char_buf_size);
int get_time_as_string(time_t time_t_val, long nsec, char* time_str,
                       int time_str_len);
time_t get_time_now();
int get_timespec_as_str(const struct timespec *times, char *time_str, int time_str_len);
char *str2md5(const char *str, int length);
int file_md5(FILE *file_handle, char *md5_file_str);
void debug_print_descriptor(struct fuse_file_info *info);
int get_safe_cache_file_path(const char *file_path, char *file_path_safe, char *temp_dir);
dir_entry *init_dir_entry();
void copy_dir_entry(dir_entry *src, dir_entry *dst);
dir_cache *new_cache(const char *path);
void dir_for(const char *path, char *dir);
void debug_list_cache_content();
void update_dir_cache(const char *path, off_t size, int isdir, int islink);
dir_entry *path_info(const char *path);
dir_entry *check_path_info(const char *path);
dir_entry * check_parent_folder_for_file(const char *path);
void dir_decache(const char *path);
void cloudfs_free_dir_list(dir_entry *dir_list);
extern int cloudfs_list_directory(const char *path, dir_entry **);
int caching_list_directory(const char *path, dir_entry **list);

#endif
