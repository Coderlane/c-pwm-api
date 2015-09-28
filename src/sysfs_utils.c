/**
 * @file sysfs_utils.c
 * @brief
 * @author Travis Lane
 * @version
 * @date 2015-09-22
 */

#include <fcntl.h>
#include <unistd.h>

#include "pwm_internal.h"
#include "pwm.h"


/**
 * The buffer size is the amount of data we will read/write out in
 * one operation at one time. The kernel says 4096, but nothing this
 * is doing is getting close to that.
 */
#define ATTR_BUFFER_SIZE 128
#define MAX_ATTR_BUFFER_SIZE 4096
#define INT_BUFFER_SIZE 32

/**
 * @brief Read a value from a sysfs attribute into a buffer.
 *
 * @param path The path to the sysfs attribute to read.
 * @param buff A buffer to read data into.
 * @param buff_size The size of the buffer provided.
 * @param out_len The length of data actually read.
 *
 * @return A statusp code
 */
int
sysfs_read_attr_str(const char *path, char *buff, size_t buff_size,
                    ssize_t *out_len)
{
  int fd, result;
  ssize_t result_len;

  fd = open(path, O_RDONLY);
  if (fd < 0) {
    result = USP_FILE_ERROR;
    goto out;
  }

  result_len = read(fd, buff, buff_size);
  if (result_len < 0) {
    result = USP_IO_ERROR;
    goto out;
  }

  /* Always make the last character NULL */
  buff[result_len] = '\0';
  *out_len = result_len;
  result = USP_OK;

out:
  if (fd >= 0)
    close(fd);

  return result;
}

/**
 * @brief Write a new value for a sysfs attribute from a buffer.
 *
 * @param path The path to the sysfs attribute to write.
 * @param data The data to write.
 * @param len The length of the data to write.
 * @param out_len The actual lenght of data written.
 *
 * @return A statusp code.
 */
int
sysfs_write_attr_str(const char *path, const char *buff, size_t buff_len,
                     ssize_t *out_len)
{
  int fd, result;
  ssize_t result_len;

  assert(buff_len < MAX_ATTR_BUFFER_SIZE);

  fd = open(path, O_WRONLY);
  if (fd < 0) {
    result = USP_FILE_ERROR;
    goto out;
  }

  result_len = write(fd, buff, buff_len);
  if (result_len < 0) {
    result = USP_IO_ERROR;
    goto out;
  }

  *out_len = result_len;
  result = USP_OK;

out:
  if (fd >= 0)
    close(fd);

  return result;
}

/**
 * @brief Read a new integer value for a sysfs attribute from a file.
 *
 * @param path The path to the sysfs attribute to read.
 * @param data The data read.
 *
 * @return A statusp code.
 */
int
sysfs_read_attr_int(const char *path, int *data)
{
  int rc;
  ssize_t read_len;
  char buff[INT_BUFFER_SIZE];

  rc = sysfs_read_attr_str(path, buff, INT_BUFFER_SIZE, &read_len);
  if(rc != USP_OK)
    return rc;

  // TODO: Check read length.
  sscanf(buff, "%d", data);
  return USP_OK;
}

/**
 * @brief Write a new integer value ofr a sysfs attribute to a file.
 *
 * @param path The path to the sysfs attribute to write.
 * @param data The data to write.
 *
 * @return A statusp code.
 */
int
sysfs_write_attr_int(const char *path, int data)
{
  int rc;
  ssize_t write_len;
  char buff[INT_BUFFER_SIZE];

  snprintf(buff, INT_BUFFER_SIZE, "%d", data);
  rc = sysfs_write_attr_str(path, buff, INT_BUFFER_SIZE, &write_len);

  // TODO: Check write length.
  return rc;
}

/**
 * @brief Read a new float value for a sysfs attribute from a file.
 *
 * @param path The path to the sysfs attribute to read.
 * @param data The data read.
 *
 * @return A statusp code.
 */
int
sysfs_read_attr_float(const char *path, float *data)
{
  float rc;
  ssize_t read_len;
  char buff[INT_BUFFER_SIZE];

  rc = sysfs_read_attr_str(path, buff, INT_BUFFER_SIZE, &read_len);
  if(rc != USP_OK)
    return rc;

  // TODO: Check read length.
  sscanf(buff, "%f", data);

  return USP_OK;
}

/**
 * @brief Write a new float value ofr a sysfs attribute to a file.
 *
 * @param path The path to the sysfs attribute to write.
 * @param data The data to write.
 *
 * @return A statusp code.
 */
int
sysfs_write_attr_float(const char *path, float data)
{
  float rc;
  ssize_t write_len;
  char buff[INT_BUFFER_SIZE];

  snprintf(buff, INT_BUFFER_SIZE, "%f", data);
  rc = sysfs_write_attr_str(path, buff, INT_BUFFER_SIZE, &write_len);

  // TODO: Check write length.
  return rc;
}
