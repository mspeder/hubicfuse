#ifndef _OPTIONS_H
#define _OPTIONS_H

#include "utils.h"

#define OPTION_SIZE 1024

typedef struct hubicOptions
{
  char* client_id;
  char* client_secret;
  char* refresh_token;
  char* temp_dir;
  char* override_storage_url;
  char* public_container;
  int cache_timeout;
  int cache_statfs_timeout;
  bool verify_ssl;
  bool get_extended_metadata;
  bool preserve_metadatas;
  bool curl_verbose;
  bool curl_progress_state;
  bool enable_chown;
  bool enable_chmod;
  bool enable_progressive_upload;
  bool enable_progressive_download;
  long segment_size;
  long segment_above;
} hubicOptions;

extern hubicOptions* options;

hubicOptions* options_new();
void options_delete(hubicOptions* opt);

int parse_option(hubicOptions* options, const char* arg, int key,
                 void* outargs);
void print_usage(const char* settings_filename);

#endif
