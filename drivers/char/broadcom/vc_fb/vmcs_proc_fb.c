/*****************************************************************************
 * Copyright 2011 Broadcom Corporation.  All rights reserved.
 *
 * Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2, available at
 * http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
 *
 * Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a
 * license other than the GPL, without Broadcom's express prior written
 * consent.
 *****************************************************************************/

#include <linux/uaccess.h>
#include <vc_fb_defs.h>
#include "vc_fb.h"

static int z_order_read_proc( char *buffer,
                             char **start,
                             off_t off,
                             int count,
                             int *eof,
                             void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   int len = 0;

   (void)start;
   (void)off;
   (void)count;
   (void)eof;

   if ( scrn_info != NULL )
      len += sprintf(buffer + len, "%u\n", (unsigned int) scrn_info->z_order );
   return len;
}

static int pixfmt_read_proc( char *buffer,
                            char **start,
                            off_t off,
                            int count,
                            int *eof,
                            void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   int len = 0;

   (void)start;
   (void)off;
   (void)count;
   (void)eof;

   if ( scrn_info != NULL )
      len += sprintf( buffer + len, "%u\n", 
                      (unsigned int) scrn_info->pixel_fmt );
   return len;
}

static int colspace_read_proc( char *buffer,
                              char **start,
                              off_t off,
                              int count,
                              int *eof,
                              void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   int len = 0;

   (void)start;
   (void)off;
   (void)count;
   (void)eof;

   if ( scrn_info != NULL )
      len += sprintf( buffer + len, "%u\n",
                      (unsigned int) scrn_info->color_space );
   return len;
}

static int buffers_read_proc( char *buffer,
                              char **start,
                              off_t off,
                              int count,
                              int *eof,
                              void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   int len = 0;

   (void)start;
   (void)off;
   (void)count;
   (void)eof;

   if ( scrn_info != NULL )
      len += sprintf( buffer + len, "%u\n",
                      (unsigned int) scrn_info->buffers );
   return len;
}

static int scale_read_proc( char *buffer,
                           char **start,
                           off_t off,
                           int count,
                           int *eof,
                           void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   int len = 0;

   (void)start;
   (void)off;
   (void)count;
   (void)eof;

   if ( scrn_info != NULL )
      len += sprintf (buffer + len, "%u\n", (unsigned int) scrn_info->scale );
   return len;
}

static int res_override_read_proc( char *buffer,
                                  char **start,
                                  off_t off,
                                  int count,
                                  int *eof,
                                  void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   int len = 0;

   (void)start;
   (void)off;
   (void)count;
   (void)eof;

   if ( scrn_info != NULL )
      len += sprintf( buffer + len,
                     "%ux%u\n",
                     (unsigned int) scrn_info->width_override,
                     (unsigned int) scrn_info->height_override );
   return len;
}

static int keep_resource_read_proc( char *buffer,
                                   char **start,
                                   off_t off,
                                   int count,
                                   int *eof,
                                   void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   int len = 0;

   (void)start;
   (void)off;
   (void)count;
   (void)eof;

   if ( scrn_info != NULL )
      len += sprintf( buffer + len, "%u\n",
                      (unsigned int) scrn_info->keep_resource );

   return len;
}

static int bpp_override_read_proc( char *buffer,
                                  char **start,
                                  off_t off,
                                  int count,
                                  int *eof,
                                  void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   int len = 0;

   (void)start;
   (void)off;
   (void)count;
   (void)eof;

   if ( scrn_info != NULL )
      len += sprintf( buffer + len, "%u\n",
                     (unsigned int) scrn_info->bpp_override );
   return len;
}

static int alpha_per_pixel_read_proc( char *buffer,
                                     char **start,
                                     off_t off,
                                     int count,
                                     int *eof,
                                     void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   int len = 0;

   (void)start;
   (void)off;
   (void)count;
   (void)eof;

   if ( scrn_info != NULL )
      len += sprintf( buffer + len, "%u\n",
                     (unsigned int) scrn_info->alpha_per_pixel );
   return len;
}

static int alpha_read_proc( char *buffer,
                           char **start,
                           off_t off,
                           int count,
                           int *eof,
                           void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   int len = 0;

   (void)start;
   (void)off;
   (void)count;
   (void)eof;

   if ( scrn_info != NULL )
      len += sprintf( buffer + len, "%u\n", (unsigned int) scrn_info->alpha );
   return len;
}
// ---- End of /proc entry read Functions -------------------------------------

// ---- /proc entry write Functions -------------------------------------------
static int z_order_write_proc( struct file *file,
                              const char __user *buffer,
                              unsigned long count,
                              void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   unsigned char kbuf[PROC_WRITE_BUF_SIZE+1];
   int ret;

   (void)file;

   memset( kbuf, 0, PROC_WRITE_BUF_SIZE+1 );
   if ( count >= PROC_WRITE_BUF_SIZE )
   {
      count = PROC_WRITE_BUF_SIZE;
   }

   if ( copy_from_user( kbuf, buffer, count ) != 0 )
   {
      LOG_ERR( "[%s]: failed to copy-from-user", __func__ );
      ret = -EFAULT;
      goto out;
   }
   kbuf[ count - 1 ] = 0;
   ret = count;

   if ( scrn_info != NULL )
   {
      if (scrn_info->instance != NULL)
      {
         LOG_ERR( "z order change from here is not allowed fb is opened\n" );
         ret = -EINVAL;
         goto out;
      }
      scrn_info->z_order = simple_strtoul( kbuf, NULL, 10 );
   }

   goto out;

out:
   return ret;
}

static int pixfmt_write_proc( struct file *file,
                             const char __user *buffer,
                             unsigned long count,
                             void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   unsigned char kbuf[PROC_WRITE_BUF_SIZE+1];
   int ret;

   (void)file;

   memset( kbuf, 0, PROC_WRITE_BUF_SIZE+1 );
   if ( count >= PROC_WRITE_BUF_SIZE )
   {
      count = PROC_WRITE_BUF_SIZE;
   }

   if ( copy_from_user( kbuf, buffer, count ) != 0 )
   {
      LOG_ERR( "[%s]: failed to copy-from-user", __func__ );

      ret = -EFAULT;
      goto out;
   }
   kbuf[ count - 1 ] = 0;
   ret = count;

   if ( scrn_info != NULL )
   {
      uint16_t input = simple_strtoul( kbuf, NULL, 10 );
      if (scrn_info->instance != NULL)
      {
         LOG_ERR( "pixel fmt change from here is not allowed fb is opened\n" );
         ret = -EINVAL;
         goto out;
      }
      if ( (FB_FORMAT_MIN < input && input < FB_FORMAT_MAX) ) {
         scrn_info->pixel_fmt = input;
         if (input == FB_FORMAT_YUVNV12) {
            FB_RECT_T *src_rect;
            FB_RECT_T *dst_rect;

            src_rect = (FB_RECT_T *)&scrn_info->fb_info.var.reserved[0];
            dst_rect = (FB_RECT_T *)&scrn_info->fb_info.var.reserved[2];
            src_rect->x = src_rect->y = dst_rect->x = dst_rect->y = 0;
            src_rect->w = dst_rect->w = 1;
            src_rect->h = dst_rect->h = 1;
         }
      } else
      {
         LOG_ERR( "Invalid pixel format value use 1-7" );
         ret = -EFAULT;
         goto out;
      }
   }
   goto out;

out:
   return ret;
}

static int colspace_write_proc( struct file *file,
                               const char __user *buffer,
                               unsigned long count,
                               void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   unsigned char kbuf[PROC_WRITE_BUF_SIZE+1];
   int ret;

   (void)file;

   memset( kbuf, 0, PROC_WRITE_BUF_SIZE+1 );
   if ( count >= PROC_WRITE_BUF_SIZE )
   {
      count = PROC_WRITE_BUF_SIZE;
   }

   if ( copy_from_user( kbuf, buffer, count ) != 0 )
   {
      LOG_ERR( "[%s]: failed to copy-from-user", __func__ );

      ret = -EFAULT;
      goto out;
   }
   kbuf[ count - 1 ] = 0;
   ret = count;

   if ( scrn_info != NULL )
   {
      uint16_t input = simple_strtoul( kbuf, NULL, 10 );
      if ( (input  < CSC_ITUR_BT_601_5 ) || (input > CSC_JPEG_JIF) )
      {
         LOG_ERR( "%s: invalid color space value (use 1-3)", __func__ );
      }
      else
         scrn_info->color_space = input;
   }
   goto out;

out:
   return ret;
}

static int buffers_write_proc( struct file *file,
                               const char __user *buffer,
                               unsigned long count,
                               void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   unsigned char kbuf[PROC_WRITE_BUF_SIZE+1];
   int ret;

   (void)file;

   memset( kbuf, 0, PROC_WRITE_BUF_SIZE+1 );
   if ( count >= PROC_WRITE_BUF_SIZE )
   {
      count = PROC_WRITE_BUF_SIZE;
   }

   if ( copy_from_user( kbuf, buffer, count ) != 0 )
   {
      LOG_ERR( "[%s]: failed to copy-from-user", __func__ );

      ret = -EFAULT;
      goto out;
   }
   kbuf[ count - 1 ] = 0;
   ret = count;

   if ( scrn_info != NULL )
   {
      uint16_t input = simple_strtoul( kbuf, NULL, 10 );
      if ( (input  < 1 ) || (input > CONFIG_FB_VC_NUM_FRAMES ) )
      {
         LOG_ERR( "%s: invalid buffers value (use 1 or %d)", __func__, CONFIG_FB_VC_NUM_FRAMES );
      }
      else
         scrn_info->buffers = input;
   }
   goto out;

out:
   return ret;
}

static int scale_write_proc( struct file *file,
                            const char __user *buffer,
                            unsigned long count,
                            void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   unsigned char kbuf[PROC_WRITE_BUF_SIZE+1];
   int ret;

   (void)file;

   memset( kbuf, 0, PROC_WRITE_BUF_SIZE+1 );
   if ( count >= PROC_WRITE_BUF_SIZE )
   {
      count = PROC_WRITE_BUF_SIZE;
   }

   if ( copy_from_user( kbuf, buffer, count ) != 0 )
   {
      LOG_ERR( "[%s]: failed to copy-from-user", __func__ );

      ret = -EFAULT;
      goto out;
   }
   kbuf[ count - 1 ] = 0;
   ret = count;

   if ( scrn_info != NULL )
   {
      scrn_info->scale = simple_strtoul( kbuf, NULL, 10 );
   }
   goto out;

out:
   return ret;
}

static int res_override_write_proc( struct file *file,
                                   const char __user *buffer,
                                   unsigned long count,
                                   void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   unsigned char kbuf[PROC_WRITE_BUF_SIZE+1];
   int ret;

   (void)file;

   memset( kbuf, 0, PROC_WRITE_BUF_SIZE+1 );
   if ( count >= PROC_WRITE_BUF_SIZE )
   {
      count = PROC_WRITE_BUF_SIZE;
   }

   if ( copy_from_user( kbuf, buffer, count ) != 0 )
   {
      LOG_ERR( "[%s]: failed to copy-from-user", __func__ );

      ret = -EFAULT;
      goto out;
   }
   kbuf[ count - 1 ] = 0;
   ret = count;

   if ( scrn_info != NULL )
   {
      FB_RECT_T *src_rect;
      FB_RECT_T *dst_rect;
      uint32_t width, height;

      if (scrn_info->instance != NULL)
      {
         LOG_ERR( "Resolution change from here is not allowed fb is opened\n" );
         ret = -EINVAL;
         goto out;
      }

      if ( sscanf(kbuf, "%ux%u", &width, &height ) != 2 )
      {
         LOG_ERR( "%s: invalid override resolution", __func__ );
         ret = -EINVAL;
         goto out;
      }
      scrn_info->width_override = width;
      scrn_info->height_override = height;

      src_rect = (FB_RECT_T *)&scrn_info->fb_info.var.reserved[0];
      dst_rect = (FB_RECT_T *)&scrn_info->fb_info.var.reserved[2];
      src_rect->x = src_rect->y = dst_rect->x = dst_rect->y = 0;
      src_rect->w = dst_rect->w = width;
      src_rect->h = dst_rect->h = height;
   }
out:
   return ret;
}

static int keep_resource_write_proc( struct file *file,
                                    const char __user *buffer,
                                    unsigned long count,
                                    void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   unsigned char kbuf[PROC_WRITE_BUF_SIZE+1];
   int ret;

   (void)file;

   memset( kbuf, 0, PROC_WRITE_BUF_SIZE+1 );
   if ( count >= PROC_WRITE_BUF_SIZE )
   {
      count = PROC_WRITE_BUF_SIZE;
   }

   if ( copy_from_user( kbuf, buffer, count ) != 0 )
   {
      LOG_ERR( "[%s]: failed to copy-from-user", __func__ );

      ret = -EFAULT;
      goto out;
   }
   kbuf[ count - 1 ] = 0;
   ret = count;

   if ( scrn_info != NULL )
   {
      scrn_info->keep_resource = !!( simple_strtoul( kbuf, NULL, 10 ) );
   }
   goto out;

out:
   return ret;
}

static int bpp_override_write_proc( struct file *file,
                                   const char __user *buffer,
                                   unsigned long count,
                                   void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   unsigned char kbuf[PROC_WRITE_BUF_SIZE+1];
   int ret;

   (void)file;

   memset( kbuf, 0, PROC_WRITE_BUF_SIZE+1 );
   if ( count >= PROC_WRITE_BUF_SIZE )
   {
      count = PROC_WRITE_BUF_SIZE;
   }

   if ( copy_from_user( kbuf, buffer, count ) != 0 )
   {
      LOG_ERR( "[%s]: failed to copy-from-user", __func__ );
      ret = -EFAULT;
      goto out;
   }
   kbuf[ count - 1 ] = 0;
   ret = count;

   if ( scrn_info != NULL )
   {
      uint32_t input;
      if (scrn_info->instance != NULL)
      {
         LOG_ERR( "bpp change from here is not allowed when fb is opened\n" );
         ret = -EINVAL;
         goto out;
      }
      input = simple_strtoul( kbuf, NULL, 10 );

      if ((input != 0 ) && (input != 16 ) && (input != 32 ) && (input != 24) )
      {
         LOG_ERR( "%s: invalid bits per pixel override value", __func__ );
         ret = -EINVAL;
         goto out;
      }
      scrn_info->bpp_override = input;
   }
out:
   return ret;
}

static int alpha_per_pixel_write_proc( struct file *file,
                                      const char __user *buffer,
                                      unsigned long count,
                                      void *data )
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   unsigned char kbuf[PROC_WRITE_BUF_SIZE+1];
   int ret;

   (void)file;

   memset( kbuf, 0, PROC_WRITE_BUF_SIZE+1 );
   if ( count >= PROC_WRITE_BUF_SIZE )
   {
      count = PROC_WRITE_BUF_SIZE;
   }

   if ( copy_from_user( kbuf, buffer, count ) != 0 )
   {
      LOG_ERR( "[%s]: failed to copy-from-user", __func__ );
      ret = -EFAULT;
      goto out;
   }
   kbuf[ count - 1 ] = 0;
   ret = count;

   if ( scrn_info != NULL )
      scrn_info->alpha_per_pixel = !!( simple_strtoul( kbuf, NULL, 10 ) );
out:
   return ret;
}

static int alpha_write_proc( struct file *file,
                            const char __user *buffer,
                            unsigned long count,
                            void *data)
{
   SCRN_INFO_T *scrn_info = (SCRN_INFO_T *) data;
   unsigned char kbuf[PROC_WRITE_BUF_SIZE+1];
   int ret;

   (void)file;

   memset( kbuf, 0, PROC_WRITE_BUF_SIZE+1 );
   if ( count >= PROC_WRITE_BUF_SIZE )
      count = PROC_WRITE_BUF_SIZE;

   if ( copy_from_user( kbuf,
      buffer,
      count ) != 0 )
   {
      LOG_ERR( "[%s]: failed to copy-from-user", __func__ );
      ret = -EFAULT;
      goto out;
   }
   kbuf[ count - 1 ] = 0;
   ret = count;

   if ( scrn_info != NULL )
   {
      scrn_info->alpha = simple_strtoul( kbuf, NULL, 10 );
      if ( scrn_info->alpha > 255 ) scrn_info->alpha = 255;
   }
   goto out;

out:
   return ret;
}

int vc_fb_create_per_scrn_proc_entries(SCRN_INFO_T *scrn_info, 
                                       struct proc_dir_entry* global_proc_entry)
{
   int ret;
   char fb_cfg_dir_entry_name[10];
   LOG_DBG( "fb=%u: ..Start", scrn_info->fb_num );

   // First create a '<x>' proc directory under 'vc-fb'
   sprintf(fb_cfg_dir_entry_name, "%d_%s", 
           scrn_info->scrn, scrn_info->fb_info.fix.id );
   scrn_info->proc_interface.fb_cfg_directory 
                        = proc_mkdir(fb_cfg_dir_entry_name, global_proc_entry );

   if ( scrn_info->proc_interface.fb_cfg_directory == NULL )
   {
      LOG_ERR( "fb=%u: failed to create proc directory entry", 
                scrn_info->fb_num );
      ret = -EPERM;
      goto out;
   }

   // Now create all the proc entries for modifiable parameters
   scrn_info->proc_interface.alpha_cfg_entry
            = create_proc_entry( "alpha", 0664,
                                 scrn_info->proc_interface.fb_cfg_directory );
   if ( scrn_info->proc_interface.alpha_cfg_entry == NULL )
   {
      LOG_ERR( "fb=%u: failed to create alpha entry",scrn_info->fb_num );
      ret = -EPERM;
      goto err_remove_fb_cfg_directory;
   }
   else
   {
      scrn_info->proc_interface.alpha_cfg_entry->data = (void *)scrn_info;
      scrn_info->proc_interface.alpha_cfg_entry->read_proc = &alpha_read_proc;
      scrn_info->proc_interface.alpha_cfg_entry->write_proc = &alpha_write_proc;
   }

   scrn_info->proc_interface.alpha_per_pixel_cfg_entry 
            = create_proc_entry( "alpha_per_pixel", 0664,
                                 scrn_info->proc_interface.fb_cfg_directory );
   if ( scrn_info->proc_interface.alpha_per_pixel_cfg_entry == NULL )
   {
      LOG_ERR( "fb=%u: failed to create alpha_per_pixel entry", 
               scrn_info->fb_num );
      ret = -EPERM;
      goto err_remove_alpha_cfg_entry;
   }
   else
   {
      scrn_info->proc_interface.alpha_per_pixel_cfg_entry->data 
                                                = (void *)scrn_info;
      scrn_info->proc_interface.alpha_per_pixel_cfg_entry->read_proc
                                                = &alpha_per_pixel_read_proc;
      scrn_info->proc_interface.alpha_per_pixel_cfg_entry->write_proc
                                                = &alpha_per_pixel_write_proc;
   }

   scrn_info->proc_interface.bpp_override_cfg_entry
            = create_proc_entry( "bpp_override", 0664,
                                 scrn_info->proc_interface.fb_cfg_directory );
   if ( scrn_info->proc_interface.bpp_override_cfg_entry == NULL )
   {
      LOG_ERR( "fb=%u: failed to create bpp_override entry", scrn_info->fb_num );
      ret = -EPERM;
      goto err_remove_alpha_per_pixel_cfg_entry;
   }
   else
   {
      scrn_info->proc_interface.bpp_override_cfg_entry->data
                                                   = (void *)scrn_info;
      scrn_info->proc_interface.bpp_override_cfg_entry->read_proc
                                                   = &bpp_override_read_proc;
      scrn_info->proc_interface.bpp_override_cfg_entry->write_proc
                                                   = &bpp_override_write_proc;
   }

   scrn_info->proc_interface.keep_resource_cfg_entry 
            = create_proc_entry( "keep_resource", 0664,
                                 scrn_info->proc_interface.fb_cfg_directory );
   if ( scrn_info->proc_interface.keep_resource_cfg_entry == NULL )
   {
      LOG_ERR( "fb=%u: failed to create keep_resource entry", 
               scrn_info->fb_num );
      ret = -EPERM;
      goto err_remove_bpp_override_cfg_entry;
   }
   else
   {
      scrn_info->proc_interface.keep_resource_cfg_entry->data
                                                   = (void *)scrn_info;
      scrn_info->proc_interface.keep_resource_cfg_entry->read_proc
                                                   = &keep_resource_read_proc;
      scrn_info->proc_interface.keep_resource_cfg_entry->write_proc
                                                   = &keep_resource_write_proc;
   }

   scrn_info->proc_interface.res_override_cfg_entry
            = create_proc_entry( "res_override", 0664,
                                 scrn_info->proc_interface.fb_cfg_directory );
   if ( scrn_info->proc_interface.res_override_cfg_entry == NULL )
   {
      LOG_ERR( "fb=%u: failed to create res_override entry",
               scrn_info->fb_num );
      ret = -EPERM;
      goto err_remove_keep_res_cfg_entry;
   }
   else
   {
      scrn_info->proc_interface.res_override_cfg_entry->data
                                                   = (void *)scrn_info;
      scrn_info->proc_interface.res_override_cfg_entry->read_proc
                                                   = &res_override_read_proc;
      scrn_info->proc_interface.res_override_cfg_entry->write_proc
                                                   = &res_override_write_proc;
   }

   scrn_info->proc_interface.scale_cfg_entry
            = create_proc_entry( "scale", 0664,
                                 scrn_info->proc_interface.fb_cfg_directory ); 
   if ( scrn_info->proc_interface.scale_cfg_entry == NULL )
   {
      LOG_ERR( "fb=%u: failed to create scale entry", scrn_info->fb_num );
      ret = -EPERM;
      goto err_remove_res_override_cfg_entry;
   }
   else
   {
      scrn_info->proc_interface.scale_cfg_entry->data = (void *)scrn_info;
      scrn_info->proc_interface.scale_cfg_entry->read_proc = &scale_read_proc;
      scrn_info->proc_interface.scale_cfg_entry->write_proc = &scale_write_proc;
   }

   scrn_info->proc_interface.z_order_cfg_entry
            = create_proc_entry( "z_order", 0664,
                                 scrn_info->proc_interface.fb_cfg_directory );
   if ( scrn_info->proc_interface.z_order_cfg_entry == NULL )
   {
      LOG_ERR( "fb=%u: failed to create z_order entry", scrn_info->fb_num );
      ret = -EPERM;
      goto err_remove_scale_cfg_entry;
   }
   else
   {
      scrn_info->proc_interface.z_order_cfg_entry->data
                                                   = (void *)scrn_info;
      scrn_info->proc_interface.z_order_cfg_entry->read_proc
                                                   = &z_order_read_proc;
      scrn_info->proc_interface.z_order_cfg_entry->write_proc
                                                   = &z_order_write_proc;
   }

   scrn_info->proc_interface.pixfmt_cfg_entry
            = create_proc_entry( "pixfmt", 0664,
                                 scrn_info->proc_interface.fb_cfg_directory );
   if ( scrn_info->proc_interface.pixfmt_cfg_entry == NULL )
   {
      LOG_ERR( "fb=%u: failed to create pixfmt entry", scrn_info->fb_num );
      ret = -EPERM;
      goto err_remove_zorder_cfg_entry;
   }
   else
   {
      scrn_info->proc_interface.pixfmt_cfg_entry->data = (void *)scrn_info;
      scrn_info->proc_interface.pixfmt_cfg_entry->read_proc = &pixfmt_read_proc;
      scrn_info->proc_interface.pixfmt_cfg_entry->write_proc
                                                         = &pixfmt_write_proc;
   }

   scrn_info->proc_interface.colspace_cfg_entry
            = create_proc_entry( "colspace", 0664,
                                 scrn_info->proc_interface.fb_cfg_directory );
   if ( scrn_info->proc_interface.colspace_cfg_entry == NULL )
   {
      LOG_ERR( "fb=%u: failed to create colspace entry", scrn_info->fb_num );
      ret = -EPERM;
      goto err_remove_pixfmt_cfg_entry;
   }
   else
   {
      scrn_info->proc_interface.colspace_cfg_entry->data = (void *)scrn_info;
      scrn_info->proc_interface.colspace_cfg_entry->read_proc
                                                         = &colspace_read_proc;
      scrn_info->proc_interface.colspace_cfg_entry->write_proc
                                                         = &colspace_write_proc;
   }

   scrn_info->proc_interface.buffers_cfg_entry
            = create_proc_entry( "buffers", 0664,
                                 scrn_info->proc_interface.fb_cfg_directory );
   if ( scrn_info->proc_interface.buffers_cfg_entry == NULL )
   {
      LOG_ERR( "fb=%u: failed to create buffers entry", scrn_info->fb_num );
      ret = -EPERM;
      goto err_remove_colspace_cfg_entry;
   }
   else
   {
      scrn_info->proc_interface.buffers_cfg_entry->data = (void *)scrn_info;
      scrn_info->proc_interface.buffers_cfg_entry->read_proc
                                                         = &buffers_read_proc;
      scrn_info->proc_interface.buffers_cfg_entry->write_proc
                                                         = &buffers_write_proc;
   }

   ret = 0;
   goto out;

err_remove_colspace_cfg_entry:
   remove_proc_entry( "colspace", scrn_info->proc_interface.fb_cfg_directory );

err_remove_pixfmt_cfg_entry:
   remove_proc_entry( "pixfmt", scrn_info->proc_interface.fb_cfg_directory );

err_remove_zorder_cfg_entry:
   remove_proc_entry( "z_order", scrn_info->proc_interface.fb_cfg_directory );

err_remove_scale_cfg_entry:
   remove_proc_entry( "scale", scrn_info->proc_interface.fb_cfg_directory );

err_remove_res_override_cfg_entry:
   remove_proc_entry( "res_override", 
                      scrn_info->proc_interface.fb_cfg_directory );

err_remove_keep_res_cfg_entry:
   remove_proc_entry( "keep_resource",
                      scrn_info->proc_interface.fb_cfg_directory );

err_remove_bpp_override_cfg_entry:
   remove_proc_entry( "bpp_override",
                      scrn_info->proc_interface.fb_cfg_directory );

err_remove_alpha_per_pixel_cfg_entry:
   remove_proc_entry( "alpha_per_pixel",
                      scrn_info->proc_interface.fb_cfg_directory );

err_remove_alpha_cfg_entry:
   remove_proc_entry( "alpha", scrn_info->proc_interface.fb_cfg_directory );

err_remove_fb_cfg_directory:
   remove_proc_entry( fb_cfg_dir_entry_name, global_proc_entry );

out:
   LOG_DBG( "fb=%u: ..End (ret=%d)", scrn_info->fb_num, ret );
   return ret;
}

void vc_fb_remove_per_scrn_proc_entries( SCRN_INFO_T *scrn_info, 
                                         struct proc_dir_entry* global_proc_entry )
{
   char fb_cfg_dir_entry_name[10];
   LOG_DBG( "fb=%u: ..Start", scrn_info->fb_num );

   // First create a '<x>' proc directory under 'vc-fb'
   sprintf(fb_cfg_dir_entry_name, "%d_%s", 
           scrn_info->scrn, scrn_info->fb_info.fix.id );

   remove_proc_entry( "alpha", scrn_info->proc_interface.fb_cfg_directory );
   remove_proc_entry( "alpha_per_pixel",
                      scrn_info->proc_interface.fb_cfg_directory );
   remove_proc_entry( "bpp_override", 
                      scrn_info->proc_interface.fb_cfg_directory );
   remove_proc_entry( "keep_resource",
                      scrn_info->proc_interface.fb_cfg_directory );
   remove_proc_entry( "res_override",
                      scrn_info->proc_interface.fb_cfg_directory );
   remove_proc_entry( "scale", scrn_info->proc_interface.fb_cfg_directory );
   remove_proc_entry( "z_order", scrn_info->proc_interface.fb_cfg_directory );
   remove_proc_entry( "pixfmt", scrn_info->proc_interface.fb_cfg_directory );
   remove_proc_entry( "colspace", scrn_info->proc_interface.fb_cfg_directory );
   remove_proc_entry( "buffers", scrn_info->proc_interface.fb_cfg_directory );

   remove_proc_entry( fb_cfg_dir_entry_name, global_proc_entry );

   LOG_DBG( "fb=%u: ..End", scrn_info->fb_num );
}
// ---- End of /proc entry write Functions ------------------------------------
