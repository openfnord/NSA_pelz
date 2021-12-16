#include <stdlib.h>
#include <stdio.h>

#include "util.h"
#include "pelz_log.h"
#include "pelz_key_loaders.h"
#include "pelz_request_handler.h"

int pelz_load_key_from_file(char *filename, charbuf * key)
{
  size_t key_len;

  if (filename == NULL)
  {
    pelz_log(LOG_ERR, "No filename provided.");
    return 1;
  }

  unsigned char tmp_key[MAX_KEY_LEN];
  FILE *key_file_handle = NULL;

  key_file_handle = fopen(filename, "r");
  if (key_file_handle == NULL)
  {
    pelz_log(LOG_ERR, "Failed to open key file %s", filename);
    return 1;
  }

  key_len = fread(tmp_key, sizeof(char), MAX_KEY_LEN, key_file_handle);

  // If we've not reached EOF something has probably gone wrong.
  if ((key_len == 0) || (!feof(key_file_handle)))
  {
    pelz_log(LOG_ERR, "Error: Failed to fully read key file.");
    secure_memset(tmp_key, 0, key_len);
    fclose(key_file_handle);
    return 1;
  }
  fclose(key_file_handle);

  *key = new_charbuf(key_len);
  if (key->len == 0)
  {
    pelz_log(LOG_ERR, "Error: Failed to allocate memory for key.");
    return 1;
  }
  memcpy(key->chars, tmp_key, key->len);
  secure_memset(tmp_key, 0, key_len);
  return 0;
}
