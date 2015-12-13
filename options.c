#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "options.h"

hubicOptions* options = NULL;

bool scan_string(const char* optionname, const char* name, const char* value,
                 char** storage)
{
  if (strcasecmp(optionname, name)) return false;
  if (*storage) free(*storage);
  *storage = strdup(value);
  return true;
}

bool scan_int(const char* optionname, const char* name, const char* value,
              int* storage)
{
  if (strcasecmp(optionname, name)) return false;
  *storage = atoi(value);
  return true;
}

bool scan_long(const char* optionname, const char* name, const char* value,
               long* storage)
{
  if (strcasecmp(optionname, name)) return false;
  *storage = atol(value);
  return true;
}

bool scan_bool(const char* optionname, const char* name, const char* value,
               bool* storage)
{
  if (strcasecmp(optionname, name)) return false;
  *storage = !strcasecmp(value, "true");
  return true;
}

int parse_option(hubicOptions* options, const char* arg, int key,
                 void* outargs) //struct fuse_args
{

  char name[OPTION_SIZE];
  char value[OPTION_SIZE];
  sscanf(arg, " %[^\r\n= ] = %[^\r\n ]", &name, &value);

  //Special handling of debug
  if (!strcmp(arg, "-f") || !strcmp(arg, "-d") || !strcmp(arg, "debug"))
  {
    if (debug_level < DBG_LEVEL_NORM) debug_level = DBG_LEVEL_NORM;
    return 1;
  }

  bool found =
    scan_string("client_id", name, value, &options->client_id) ||
    scan_string("client_secret", name, value, &options->client_secret) ||
    scan_string("refresh_token", name, value, &options->refresh_token) ||
    scan_string("override_storage_url", name, value,
                &options->override_storage_url) ||
    scan_string("public_container", name, value, &options->public_container) ||
    scan_string("temp_dir", name, value, &options->temp_dir) ||
    scan_int("cache_timeout", name, value, &options->cache_timeout) ||
    scan_int("cache_statfs_timeout", name, value, &options->cache_statfs_timeout)
    ||
    scan_int("debug_level", name, value, &debug_level) ||
    scan_bool("verify_ssl", name, value, &options->verify_ssl) ||
    scan_bool("get_extended_metadata", name, value,
              &options->get_extended_metadata) ||
    scan_bool("curl_verbose", name, value, &options->curl_verbose) ||
    scan_bool("curl_progress_state", name, value, &options->curl_progress_state) ||
    scan_bool("enable_chown", name, value, &options->enable_chown) ||
    scan_bool("enable_chmod", name, value, &options->enable_chmod) ||
    scan_bool("enable_progressive_download", name, value,
              &options->enable_progressive_download) ||
    scan_bool("get_extended_metadata", name, value,
              &options->get_extended_metadata) ||
    scan_bool("preserve_metadatas", name, value,
              &options->preserve_metadatas) ||
    scan_long("segment_size", name, value, &options->segment_size) ||
    scan_long("segment_above", name, value, &options->segment_above);
  return !found;
}

hubicOptions* options_new()
{
  hubicOptions* opt = malloc(sizeof(hubicOptions));

  opt->client_id = NULL;
  opt->client_secret = NULL;
  opt->refresh_token = NULL;
  opt->temp_dir = NULL;
  opt->override_storage_url = NULL;
  opt->public_container = NULL;
  opt->cache_timeout = 600;
  opt->verify_ssl = 2;
  opt->get_extended_metadata = false;
  opt->preserve_metadatas = false;
  opt->curl_verbose = false;
  opt->cache_statfs_timeout = 0;
  opt->curl_progress_state = false;
  opt->enable_chown = false;
  opt->enable_chmod = false;
  opt->enable_progressive_upload = false;
  opt->enable_progressive_download = false;
  opt->segment_size = 1073741824;
  opt->segment_above = 2147483647;

  return opt;
}

void options_delete(hubicOptions* opt)
{
  free(opt->client_id);
  free(opt->client_secret);
  free(opt->refresh_token);
  free(opt->override_storage_url);
  free(opt->public_container);
  free(opt->temp_dir);
  free(opt);
}

void print_usage(const char* settings_filename)
{
  fprintf(stderr,
          "Unable to determine client_id, client_secret or refresh_token.\n\n");
  fprintf(stderr, "These can be set either as mount options or in "
          "a file named %s\n\n", settings_filename);
  fprintf(stderr, "  client_id=[App's id]\n");
  fprintf(stderr, "  client_secret=[App's secret]\n");
  fprintf(stderr, "  refresh_token=[Get it running hubic_token]\n");
  fprintf(stderr, "The following settings are optional:\n\n");
  fprintf(stderr,
          "  cache_timeout=[Seconds for directory caching, default 600]\n");
  fprintf(stderr, "  verify_ssl=[false to disable SSL cert verification]\n");
  fprintf(stderr,
          "  segment_size=[Size to use when creating DLOs, default 1073741824]\n");
  fprintf(stderr,
          "  segment_above=[File size at which to use segments, defult 2147483648]\n");
  fprintf(stderr,
          "  storage_url=[Storage URL for other tenant to view container]\n");
  fprintf(stderr,
          "  container=[Public container to view of tenant specified by storage_url]\n");
  fprintf(stderr, "  temp_dir=[Directory to store temp files]\n");
  fprintf(stderr,
          "  get_extended_metadata=[true to enable download of utime, chmod, chown, xattrs file attributes (but slower)]\n");
  fprintf(stderr, "  preserve_metadatas=[true to preserve unkown metadatas]\n");
  fprintf(stderr,
          "  curl_verbose=[true to debug info on curl requests (lots of output)]\n");
  fprintf(stderr,
          "  curl_progress_state=[true to enable progress callback enabled. Mostly used for debugging]\n");
  fprintf(stderr,
          "  cache_statfs_timeout=[number of seconds to cache requests to statfs (cloud statistics), 0 for no cache]\n");
  fprintf(stderr,
          "  debug_level=[0 to 2, 0 for minimal verbose debugging. No debug if -d or -f option is not provided.]\n");
  fprintf(stderr, "  enable_chmod=[true to enable chmod support on fuse]\n");
  fprintf(stderr, "  enable_chown=[true to enable chown support on fuse]\n");
}
