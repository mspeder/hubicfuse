#ifndef _CLOUDFSAPI_H
#define _CLOUDFSAPI_H

#include <curl/curl.h>
#include <curl/easy.h>
#include <time.h>
#include "utils.h"
#include "options.h"

#define BUFFER_INITIAL_SIZE 4096
#define MAX_HEADER_SIZE 8192

#define MAX_URL_SIZE (MAX_PATH_SIZE * 3)
#define USER_AGENT "CloudFuse"

typedef struct curl_slist curl_slist;

#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL 5
struct curl_progress {
  double lastruntime;
  CURL *curl;
};


void cloudfs_init(void);
void cloudfs_free(void);
void cloudfs_set_credentials(char *client_id, char *client_secret, char *refresh_token);
int cloudfs_connect(void);

struct segment_info
{
  FILE *fp;
  int part;
  long size;
  long segment_size;
  char *seg_base;
  const char *method;
};

int file_is_readable(const char* fname);
const char* get_file_mimetype (const char* filename);
int cloudfs_object_read_fp(const char* path, FILE* fp);
int cloudfs_object_write_fp(const char* path, FILE* fp);
int cloudfs_list_directory(const char* path, dir_entry**);
int cloudfs_delete_object(const char* path);
int cloudfs_copy_object(const char* src, const char* dst);
int cloudfs_create_symlink(const char* src, const char* dst);
int cloudfs_create_directory(const char* label);
int cloudfs_object_truncate(const char* path, off_t size);
off_t cloudfs_file_size(int fd);
int cloudfs_statfs(const char *path, struct statvfs *stat);
void cloudfs_verify_ssl(int dbg);
void get_file_metadata(dir_entry* de);
int cloudfs_update_meta(dir_entry* de);
#endif
