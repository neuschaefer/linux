#include <ft2build.h>
#include FT_BITMAP_H
#include "ft_bitmap.h"
#include "os/inc/x_os.h"

#ifndef LINUX_TURNKEY_SOLUTION
#include FT_INTERNAL_OBJECTS_H
#endif

#ifndef FE_DISABLE_EDGE_EFFECT_SUPPORT

  /* documentation is in ftbitmap.h */
 /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  _FT_Bitmap_Copy( FT_Library        library,
                  const FT_Bitmap  *source,
                  FT_Bitmap        *target,
                  FT_Int            buf_count)
  {
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch  = source->pitch;
    FT_ULong   size;
    
    if ( source == target )
      return FT_Err_Ok;

    if ( source->buffer == NULL )
    {
      *target = *source;

      return FT_Err_Ok;
    }
    
    if ( pitch < 0 )
      pitch = -pitch;
    size = (FT_ULong)( pitch * source->rows * buf_count );

    if ( target->buffer )
    {
      FT_Int    target_pitch = target->pitch;
      FT_ULong  target_size;


      if ( target_pitch < 0  )
        target_pitch = -target_pitch;
      target_size = (FT_ULong)( target_pitch * target->rows* buf_count );   

      if ( target_size != size )
        {
            target->buffer = (unsigned char*)x_mem_realloc(target->buffer,target_size);
            x_memset(target->buffer, 0, target_size);
        }
    }
    else
        {
            target->buffer = (unsigned char*)x_mem_alloc(size);
            x_memset(target->buffer, 0, size);
        }

    if ( !error )
    {
      unsigned char *p;


      p = target->buffer;
      *target = *source;
      target->buffer = p;

      x_memcpy( target->buffer, source->buffer, size );
    }

    return error;
  }



  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  _FT_Bitmap_Convert( FT_Library        library,
                     const FT_Bitmap  *source,
                     FT_Bitmap        *target,
                     FT_Int            alignment,
                     FT_Int            buf_count)
  {
    FT_Error   error = FT_Err_Ok;
    FT_Int	  buf_idx;


    if ( !library )
      return FT_Err_Invalid_Library_Handle;


    switch ( source->pixel_mode )
    {
    case FT_PIXEL_MODE_MONO:
    case FT_PIXEL_MODE_GRAY:
    case FT_PIXEL_MODE_GRAY2:
    case FT_PIXEL_MODE_GRAY4:
      {
        FT_Int   pad;
        FT_Long  old_size;


        old_size = target->rows * target->pitch * buf_count;
        if ( old_size < 0 )
          old_size = -old_size;

        target->pixel_mode = FT_PIXEL_MODE_GRAY;
        target->rows       = source->rows;
        target->width      = source->width;

        pad = 0;
        if ( alignment > 0 )
        {
          pad = source->width % alignment;
          if ( pad != 0 )
            pad = alignment - pad;
        }

        target->pitch = source->width + pad;

        if ( target->rows * target->pitch  * buf_count> old_size )
        {
            target->buffer = (unsigned char*)x_mem_realloc(target->buffer, target->rows * target->pitch * buf_count);
            
        }
        if (target->buffer == NULL)
        return FT_Err_Invalid_Argument;

        x_memset(target->buffer, 0, target->rows * target->pitch  * buf_count);
      }
      break;

    default:
      error = FT_Err_Invalid_Argument;
    }

    switch ( source->pixel_mode )
    {
    case FT_PIXEL_MODE_MONO:
      {
        FT_Byte*  s = source->buffer;
        FT_Byte*  t = target->buffer;
        FT_Int    i;


        target->num_grays = 2;

		for ( buf_idx = buf_count; buf_idx > 0; buf_idx-- )
		{
        for ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_Int    j;


          /* get the full bytes */
          for ( j = source->width >> 3; j > 0; j-- )
          {
            FT_Int  val = ss[0]; /* avoid a byte->int cast on each line */


            tt[0] = (FT_Byte)( ( val & 0x80 ) >> 7 );
            tt[1] = (FT_Byte)( ( val & 0x40 ) >> 6 );
            tt[2] = (FT_Byte)( ( val & 0x20 ) >> 5 );
            tt[3] = (FT_Byte)( ( val & 0x10 ) >> 4 );
            tt[4] = (FT_Byte)( ( val & 0x08 ) >> 3 );
            tt[5] = (FT_Byte)( ( val & 0x04 ) >> 2 );
            tt[6] = (FT_Byte)( ( val & 0x02 ) >> 1 );
            tt[7] = (FT_Byte)(   val & 0x01 );

            tt += 8;
            ss += 1;
          }

          /* get remaining pixels (if any) */
          j = source->width & 7;
          if ( j > 0 )
          {
            FT_Int  val = *ss;


            for ( ; j > 0; j-- )
            {
              tt[0] = (FT_Byte)( ( val & 0x80 ) >> 7);
              val <<= 1;
              tt   += 1;
            }
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      }
      break;


    case FT_PIXEL_MODE_GRAY:
      {
        FT_Int    width   = source->width;
        FT_Byte*  s       = source->buffer;
        FT_Byte*  t       = target->buffer;
        FT_Int    s_pitch = source->pitch;
        FT_Int    t_pitch = target->pitch;
        FT_Int    i;


        target->num_grays = 256;

		for ( buf_idx = buf_count; buf_idx > 0; buf_idx-- )
		{
        for ( i = source->rows; i > 0; i-- )
        {
          x_memcpy(t, s, width);

          s += s_pitch;
          t += t_pitch;
        }
        }
      }
      break;


    case FT_PIXEL_MODE_GRAY2:
      {
        FT_Byte*  s = source->buffer;
        FT_Byte*  t = target->buffer;
        FT_Int    i;


        target->num_grays = 4;
		for ( buf_idx = buf_count; buf_idx > 0; buf_idx-- )
		{
        for ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_Int    j;


          /* get the full bytes */
          for ( j = source->width >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = source->width & 3;
          if ( j > 0 )
          {
            FT_Int  val = ss[0];


            for ( ; j > 0; j-- )
            {
              tt[0]  = (FT_Byte)( ( val & 0xC0 ) >> 6 );
              val  <<= 2;
              tt    += 1;
            }
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      }
      break;


    case FT_PIXEL_MODE_GRAY4:
      {
        FT_Byte*  s = source->buffer;
        FT_Byte*  t = target->buffer;
        FT_Int    i;


        target->num_grays = 16;
		for ( buf_idx = buf_count; buf_idx > 0; buf_idx-- )
		{
        for ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_Int    j;


          /* get the full bytes */
          for ( j = source->width >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          if ( source->width & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += source->pitch;
          t += target->pitch;
        }
        }
      }
      break;


    default:
      ;
    }

    return error;
  }



  static FT_Error
  ft_bitmap_assure_edge_buffer( FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels,
                           FT_UInt     xmove,
                           FT_UInt     ymove,
                           FT_Int      buf_count)
  {
    int             pitch;
    int             new_pitch;
    FT_UInt         ppb;
    FT_Int          i;
    FT_Int          j;
    unsigned char*  buffer = NULL;
    int             cpy_rows;


    pitch = bitmap->pitch;
    if ( pitch < 0 )
      pitch = -pitch;

    switch ( bitmap->pixel_mode )
    {
    case FT_PIXEL_MODE_MONO:
      ppb = 8;
      break;
    case FT_PIXEL_MODE_GRAY2:
      ppb = 4;
      break;
    case FT_PIXEL_MODE_GRAY4:
      ppb = 2;
      break;
    case FT_PIXEL_MODE_GRAY:
    case FT_PIXEL_MODE_LCD:
    case FT_PIXEL_MODE_LCD_V:
      ppb = 1;
      break;
    default:
      return FT_Err_Invalid_Glyph_Format;
    }

    new_pitch = ( bitmap->width + xpixels + ppb - 1 ) / ppb;
    cpy_rows =  bitmap->rows;

    buffer = (unsigned char*)x_mem_alloc((new_pitch * ( bitmap->rows + ypixels ) ) * 2);
    if ( buffer == NULL)
      return FT_Err_Invalid_Argument;
    x_memset(buffer, 0, (new_pitch * ( bitmap->rows + ypixels ) ) * 2);
    if ( bitmap->pitch > 0 )
    {
      for ( i = 0; i < bitmap->rows; i++ )
        x_memcpy( buffer + new_pitch * ( ymove + i ) + ( xmove / ppb ),
                     bitmap->buffer + pitch * i, pitch < new_pitch ? pitch : new_pitch );
      
      for ( i = bitmap->rows + ypixels ; i < cpy_rows; i++ )
      {
        x_memcpy( buffer + new_pitch * ( ymove + i ) + ( xmove / ppb ),
                     bitmap->buffer + pitch * (i - ypixels), pitch < new_pitch ? pitch : new_pitch );
      }
    }
    else
    {
      for ( i = 0; i < bitmap->rows; i++ )
        x_memcpy( buffer + new_pitch * ( ymove - ypixels + i ) + ( ( xmove - xpixels ) / ppb ),
                     bitmap->buffer + pitch * i, new_pitch );
      for ( i = bitmap->rows + ypixels ; i < cpy_rows; i++ )
      {
        x_memcpy( buffer + new_pitch * ( ymove - ypixels + i ) + ( ( xmove - xpixels ) / ppb ),
                     bitmap->buffer + pitch * (i - ypixels), new_pitch );
      }
    }

    if ( ( xmove / ppb ) * ppb != xmove )
    { /* Need shift */
      FT_Int          shift;

      shift = ( xmove - ( ( xmove / ppb ) * ppb ) ) * (8 / ppb);

      for ( i = 0; i < bitmap->rows + ypixels; i++ )
      {
        for ( j = new_pitch - 1; j > 0; j-- )
          buffer[new_pitch * i + j] = ( buffer[new_pitch * i + j - 1] << (8 - shift) ) |
                                      ( buffer[new_pitch * i + j] >> shift);
        buffer[new_pitch * i] >>= shift;
      }
    }

    x_mem_free( bitmap->buffer );
    bitmap->buffer = buffer;

    if ( bitmap->pitch < 0 )
      new_pitch = -new_pitch;

    bitmap->pitch = new_pitch;
    bitmap->rows += ypixels;

    return FT_Err_Ok;
  }



  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Shadow( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      dx,
                      FT_Pos      dy,
                      FT_Int      buf_count)
  {
    FT_Error        error;
    unsigned char*  p;
    unsigned char*  pedge;
    FT_Int          x, y, pitch;
    unsigned char*  buffer_edge_old = NULL;
    FT_Int          i;

    if ( !library )
      return FT_Err_Invalid_Library_Handle;

    if ( !bitmap || !bitmap->buffer )
      return FT_Err_Invalid_Argument;

    if ( (dx == 0) && (dy == 0) )
      return FT_Err_Ok;

    switch ( bitmap->pixel_mode )
    {
    case FT_PIXEL_MODE_GRAY2:
    case FT_PIXEL_MODE_GRAY4:
      {
        FT_Bitmap  tmp;
        FT_Int     align;


        if ( bitmap->pixel_mode == FT_PIXEL_MODE_GRAY2 )
          align = ( bitmap->width + dx + 3 ) / 4;
        else
          align = ( bitmap->width + dx + 1 ) / 2;

        FT_Bitmap_New( &tmp );
        error = _FT_Bitmap_Convert( library, bitmap, &tmp, align, buf_count );

        if ( error )
          return error;

        FT_Bitmap_Done( library, bitmap );
        *bitmap = tmp;
      }
      break;

    case FT_PIXEL_MODE_MONO:
      if ( dx > 8 )
        dx = 8;
      break;

    case FT_PIXEL_MODE_LCD:
      dx *= 3;
      break;

    case FT_PIXEL_MODE_LCD_V:
      dy *= 3;
      break;
    }
	
    error = ft_bitmap_assure_edge_buffer(bitmap, 
                (dx > 0)? dx: -dx, 
                (dy > 0)? dy: -dy, 
                (dx > 0)? 0: -dx, 
                (dy > 0)? 0: -dy,
                buf_count);
    if ( error )
      return error;

    pitch = bitmap->pitch;
    p = bitmap->buffer;

    pedge = p + (pitch * bitmap->rows);

    if (buf_count == 2)
    {		
        buffer_edge_old = (unsigned char*)x_mem_alloc(bitmap->pitch * (bitmap->rows));
		if ( buffer_edge_old == NULL)		     
			  return FT_Err_Invalid_Argument;
		x_memset(buffer_edge_old, 0, bitmap->pitch * (bitmap->rows));
		if ( bitmap->pitch > 0 )
		{
		  for ( i = 0; i < bitmap->rows; i++ )
			x_memcpy( buffer_edge_old + bitmap->pitch * i,
						 pedge + pitch * i, pitch);
		}
		else
		{
		  for ( i = 0; i < bitmap->rows; i++ )
			x_memcpy( buffer_edge_old +  bitmap->pitch * i,
						 bitmap->buffer + pitch * i, pitch );
		}
		
	}

    
    x_memset(pedge, 0, pitch * bitmap->rows);

    if ( bitmap->pixel_mode == FT_PIXEL_MODE_MONO )
    {
      for ( y = 0; y < bitmap->rows ; y++ )
      {
        for ( x = pitch - 1; x >= 0; x-- )
        {
          if ( (y + dy >= 0) && (y + dy < bitmap->rows) )
          {
            if ( dx > 0 )
            {
              pedge[dy * pitch + x] = (p[x] >> dx);
              if ( x > 0 )
                pedge[x] |= p[x - 1] << ( 8 - dx );
            }
            else
            {
              pedge[dy * pitch + x] = (p[x] << -dx);
              if ( x < pitch - 1 )
                pedge[x] |= p[x + 1] >> ( 8 + dx );
            }

            pedge[dy * pitch + x] ^= p[dy * pitch + x];
          }
        }

        p += pitch;
        pedge += pitch;
      }
    }
    else
    {
      for ( y = 0; y < bitmap->rows ; y++ )
      {
        for ( x = 0; x < pitch ; x++ )
        {
          if (p[x] && (bitmap->num_grays != p[dy * pitch + x + dx]))
          {
            if ((y + dy >= 0) && (y + dy < bitmap->rows) && 
                (x + dx >= 0) && (x + dx < pitch))
            {
              pedge[dy * pitch + x + dx] = 
                  p[x] * (bitmap->num_grays - p[dy * pitch + x + dx]) / bitmap->num_grays;
            }
          }
        }

        p += pitch;
        pedge += pitch;
      }
    }

	if (buf_count == 2)
	{
		p = bitmap->buffer;	
		pedge = p + (pitch * bitmap->rows);
	    for ( y = 0; y < bitmap->rows ; y++ )
	    {
		    for ( x = 0; x < pitch ; x++ )
		    {
		    	  pedge[x] |= buffer_edge_old[x];
	    	}
	    	buffer_edge_old += pitch;
	        pedge += pitch;

	    }
	}

    bitmap->width += (dx > 0)? dx: -dx;

	if (buffer_edge_old != NULL)
	{
		x_mem_free(buffer_edge_old);
	}
    return FT_Err_Ok;
  }



  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Raised( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      dx,
                      FT_Pos      dy,
                      FT_Int      buf_count)
  {
    FT_Error        error;
    unsigned char*  p;
    unsigned char*  pedge;
    FT_Int          dxa, dya;
    FT_Int          i, x, y, pitch;
	unsigned char*  buffer_edge_old = NULL;

    if ( !library )
      return FT_Err_Invalid_Library_Handle;

    if ( !bitmap || !bitmap->buffer )
      return FT_Err_Invalid_Argument;

    if ( (dx == 0) && (dy == 0) )
      return FT_Err_Ok;

    if ( dx < 0 )
      dxa = -dx;
    else
      dxa = dx;
    if ( dy < 0 )
      dya = -dy;
    else
      dya = dy;

    switch ( bitmap->pixel_mode )
    {
    case FT_PIXEL_MODE_GRAY2:
    case FT_PIXEL_MODE_GRAY4:
      {
        FT_Bitmap  tmp;
        FT_Int     align;


        if ( bitmap->pixel_mode == FT_PIXEL_MODE_GRAY2 )
          align = ( bitmap->width + dxa + 3 ) / 4;
        else
          align = ( bitmap->width + dxa + 1 ) / 2;

        FT_Bitmap_New( &tmp );
        error = _FT_Bitmap_Convert( library, bitmap, &tmp, align, buf_count);

        if ( error )
          return error;

        FT_Bitmap_Done( library, bitmap );
        *bitmap = tmp;
      }
      break;

    case FT_PIXEL_MODE_MONO:
      if ( dx > 8 )
        dx = 8;
      break;

    case FT_PIXEL_MODE_LCD:
      dx *= 3;
      break;

    case FT_PIXEL_MODE_LCD_V:
      dy *= 3;
      break;
    }
    error = ft_bitmap_assure_edge_buffer( bitmap, 
                (dx > 0)? dx: dxa, 
                (dy > 0)? dy: dya, 
                (dx > 0)? 0: dxa, 
                (dy > 0)? 0: dya,
                buf_count);
    if ( error )
      return error;

    pitch = bitmap->pitch;
    p = bitmap->buffer;

    pedge = p + (pitch * bitmap->rows);

	
    if (buf_count == 2)
    {
        buffer_edge_old = (unsigned char*)x_mem_alloc(bitmap->pitch * (bitmap->rows));
	   if ( buffer_edge_old == NULL)
			 return FT_Err_Invalid_Argument;
		x_memset(buffer_edge_old, 0, bitmap->pitch * (bitmap->rows));
	   
	   if ( bitmap->pitch > 0 )
	   {
		 for ( i = 0; i < bitmap->rows; i++ )
		   x_memcpy( buffer_edge_old + bitmap->pitch * i,
						pedge + pitch * i, pitch);
	   }
	   else
	   {
		 for ( i = 0; i < bitmap->rows; i++ )
		   x_memcpy( buffer_edge_old +  bitmap->pitch * i,
						bitmap->buffer + pitch * i, pitch );
	   }
    }
    
    x_memset(pedge, 0, pitch * bitmap->rows);

    if ( bitmap->pixel_mode == FT_PIXEL_MODE_MONO )
    {
      for ( y = 0; y < (bitmap->rows - dya) ; y++ )
      {
        for ( x = pitch - 1; x >= 0; x-- )
        {
          if ( dx > 0 )
          {
            for ( i = dx; i > 0; i-- )
            {
              FT_Int iy = dy * i / dx;
              FT_Int idx = iy * pitch + x;
  
              pedge[idx] |= (p[x] >> i);
              if ( x < pitch - 1 )
                pedge[idx + 1] |= p[x] << ( 8 - i );
            }
          }
          else
          {
            for ( i = 0; i < dxa; i++ )
            {
              FT_Int iy = dya * i / dxa;
              FT_Int idx = iy * pitch + x;
  
              pedge[idx] |= (p[dya * pitch + x] << i);
              if ( x > 0 )
                pedge[idx - 1] |= p[dya * pitch + x] >> ( 8 - i );
            }
          }
        }

        p += pitch;
        pedge += pitch;
      }

      p = bitmap->buffer;
      pedge = p + (pitch * bitmap->rows);
      for ( y = 0; y < bitmap->rows ; y++ )
      {
        for ( x = pitch - 1; x >= 0; x-- )
        {
          pedge[x] ^= p[x];
        }

        p += pitch;
        pedge += pitch;
      }
    }
    else
    {
      for ( y = 0; y < (bitmap->rows - dya) ; y++ )
      {
        for ( x = 0; x < (pitch - dxa) ; x++ )
        {
          if ((dx > 0) && p[x])
          {
            unsigned char gray = p[x];
            unsigned char gray2 = p[x] * 3 / 4;
  
            for ( i = dx; i > 0; i-- )
            {
              FT_Int iy = dy * i / dx;
              FT_Int idx = iy * pitch + x + i;
  
              if ((iy * dx) == (dy * i))
              {
                pedge[idx] |= gray;
              }
              else
              {
                pedge[idx] |= gray2;
                pedge[idx + pitch] |= gray2;
              }
            }
          }
          else if ((dx < 0) && p[dya * pitch + x + dxa])
          {
            unsigned char gray = p[dya * pitch + x + dxa];
            unsigned char gray2 = gray * 3 / 4;
  
            for ( i = 0; i < dxa; i++ )
            {
              FT_Int iy = dya * i / dxa;
              FT_Int idx = iy * pitch + x + i;
 
              if ((iy * dxa) == (dya * i))
              {
                pedge[idx] |= gray;
              }
              else
              {
                pedge[idx] |= gray2;
                pedge[idx + pitch] |= gray2;
              }
            }
          }
        }
  
        p += pitch;
        pedge += pitch;
      }

      p = bitmap->buffer;
      pedge = p + (pitch * bitmap->rows);
      for ( y = 0; y < bitmap->rows; y++ )
      {
        for ( x = 0; x < pitch; x++ )
        {
          if (pedge[x] && p[x])
          {
              pedge[x] = pedge[x] * (bitmap->num_grays - p[x]) / (bitmap->num_grays);
          }
        }
        p += pitch;
        pedge += pitch;
      }
    }

	if (buf_count == 2)
	{
		p = bitmap->buffer; 
		pedge = p + (pitch * bitmap->rows);
		for ( y = 0; y < bitmap->rows ; y++ )
		{
			for ( x = 0; x < pitch ; x++ )
			{
				  pedge[x] |= buffer_edge_old[x];
			}
			buffer_edge_old += pitch;
			pedge += pitch;

		}
	}


    bitmap->width += (dx > 0)? dx: dxa;

	if (buffer_edge_old != NULL)
	{
		x_mem_free(buffer_edge_old);
	}
    return FT_Err_Ok;
  }



  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Uniform( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      width,
                      FT_Int      buf_count)
  {
    FT_Error        error;
    unsigned char*  p;
    unsigned char*  q;
    unsigned char*  pedge;
    FT_Int          i, j, x, y, pitch;
    unsigned char*  buffer_edge_old = NULL;


    if ( !library )
      return FT_Err_Invalid_Library_Handle;

    if ( !bitmap || !bitmap->buffer )
      return FT_Err_Invalid_Argument;

    if ( width <= 0 )
      return FT_Err_Ok;


    switch ( bitmap->pixel_mode )
    {
    case FT_PIXEL_MODE_GRAY2:
    case FT_PIXEL_MODE_GRAY4:
      {
        FT_Bitmap  tmp;
        FT_Int     align;


        if ( bitmap->pixel_mode == FT_PIXEL_MODE_GRAY2 )
          align = ( bitmap->width + width + 3 ) / 4;
        else
          align = ( bitmap->width + width + 1 ) / 2;

        FT_Bitmap_New( &tmp );
        error = _FT_Bitmap_Convert( library, bitmap, &tmp, align, buf_count);

        if ( error )
          return error;

        FT_Bitmap_Done( library, bitmap );
        *bitmap = tmp;
      }
      break;

    case FT_PIXEL_MODE_MONO:
      if ( width > 8 )
        width = 8;
      break;

    case FT_PIXEL_MODE_LCD:
      width *= 3;
      break;

    case FT_PIXEL_MODE_LCD_V:
      width *= 3;
      break;
    }

    error = ft_bitmap_assure_edge_buffer( bitmap, width * 2, width * 2, width, width, buf_count);
    if ( error )
      return error;

    pitch = bitmap->pitch;
    p = bitmap->buffer;

    pedge = p + (pitch * bitmap->rows);

    if (buf_count == 2)
    {
        buffer_edge_old = (unsigned char*)x_mem_alloc(bitmap->pitch * (bitmap->rows));
	   if ( buffer_edge_old == NULL)
			 return FT_Err_Invalid_Argument;
		x_memset(buffer_edge_old, 0, bitmap->pitch * (bitmap->rows));
	   
	   if ( bitmap->pitch > 0 )
	   {
		 for ( i = 0; i < bitmap->rows; i++ )
		   x_memcpy( buffer_edge_old + bitmap->pitch * i,
						pedge + pitch * i, pitch);
	   }
	   else
	   {
		 for ( i = 0; i < bitmap->rows; i++ )
		   x_memcpy( buffer_edge_old +  bitmap->pitch * i,
						bitmap->buffer + pitch * i, pitch );
	   }
    }
    
    x_memset(pedge, 0, pitch * bitmap->rows);

    if ( bitmap->pixel_mode == FT_PIXEL_MODE_MONO )
    {
      for ( y = 0; y < bitmap->rows ; y++ )
      {
        /* Horizontally */
        for ( x = pitch - 1; x >= 0; x-- )
        {
          for ( i = 1; i <= width; i++ )
          {
            pedge[x] |= (p[x] >> i) | (p[x] << i);
            if ( x > 0 )
              pedge[x] |= p[x - 1] << ( 8 - i );
            if ( x < pitch - 1 )
              pedge[x] |= p[x + 1] >> ( 8 - i );
          }
        }
        /* Vertically */
        for ( x = 1; x <= width; x++ )
        {
          if ( y - x >= 0 )
          {
            q = p - pitch * x;
            for ( i = 0; i < pitch; i++ )
              pedge[i] |= q[i];
          }
          if ( y + x < bitmap->rows )
          {
            q = p + pitch * x;
            for ( i = 0; i < pitch; i++ )
              pedge[i] |= q[i];
          }
        }
        /* Take out the font body */
        for ( x = pitch - 1; x >= 0; x-- )
        {
          pedge[x] ^= p[x];
        }

        p += pitch;
        pedge += pitch;
      }
    }
    else
    {
      p += width * pitch + width;

      /* fill the value of each pixel to its surrounding edge pixels if it's larger;
         this ensures that each edge pixel gets the largest value */
      for ( y = 0; y < (bitmap->rows - (2 * width)) ; y++ )
      {
        for ( x = 0; x < (pitch - (2 * width)) ; x++ )
        {
          unsigned char gray = p[x];

          for ( i = 0; i <= (2 * width); i++ )
          {
            unsigned char *ptemp = &pedge[i * pitch + x];

            for ( j = 0; j <= (2 * width); j++ )
            {
              ptemp[j] |= gray;
            }
          }
        }

        p += pitch;
        pedge += pitch;
      }

      p = bitmap->buffer;
      pedge = p + (pitch * bitmap->rows);
      for ( y = 0; y < bitmap->rows; y++ )
      {
        for ( x = 0; x < pitch; x++ )
        {
          if (pedge[x] && p[x])
          {
            pedge[x] = pedge[x] * (bitmap->num_grays - p[x]) / (bitmap->num_grays);
          }
        }
        p += pitch;
        pedge += pitch;
      }
    }

    
	if (buf_count == 2)
	{
		p = bitmap->buffer; 
		pedge = p + (pitch * bitmap->rows);
		for ( y = 0; y < bitmap->rows ; y++ )
		{
			for ( x = 0; x < pitch ; x++ )
			{
				  pedge[x] |= buffer_edge_old[x];
			}
			buffer_edge_old += pitch;
			pedge += pitch;

		}
	}

    bitmap->width += 2 * width;

	if (buffer_edge_old != NULL)
	{
		x_mem_free(buffer_edge_old);
	}
    return FT_Err_Ok;
  }



  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Outline( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      width )
  {
    FT_Error        error;
    int             new_pitch;
    int             new_rows;
    FT_UInt         ppb;
    unsigned char*  buffer = NULL;
    unsigned char*  p;
    unsigned char*  p2;
    unsigned char*  pedge;
    FT_Int          i, j, x, y, pitch;
	
#ifndef LINUX_TURNKEY_SOLUTION
	FT_Memory memory;
#endif

    if ( !library )
      return FT_Err_Invalid_Library_Handle;

    if ( !bitmap || !bitmap->buffer )
      return FT_Err_Invalid_Argument;

    if ( width <= 0 )
      return FT_Err_Ok;
	
#ifndef LINUX_TURNKEY_SOLUTION
	memory = library->memory;
#endif


    switch ( bitmap->pixel_mode )
    {
    case FT_PIXEL_MODE_GRAY2:
    case FT_PIXEL_MODE_GRAY4:
      {
        FT_Bitmap  tmp;
        FT_Int     align;


        if ( bitmap->pixel_mode == FT_PIXEL_MODE_GRAY2 )
          align = ( bitmap->width + width + 3 ) / 4;
        else
          align = ( bitmap->width + width + 1 ) / 2;

        FT_Bitmap_New( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, align );

        if ( error )
          return error;

        FT_Bitmap_Done( library, bitmap );
        *bitmap = tmp;
      }
      break;

    case FT_PIXEL_MODE_MONO:
      if ( width > 8 )
        width = 8;
      break;

    case FT_PIXEL_MODE_LCD:
      width *= 3;
      break;

    case FT_PIXEL_MODE_LCD_V:
      width *= 3;
      break;
    }

    pitch = bitmap->pitch;
    if ( pitch < 0 )
      pitch = -pitch;

    switch ( bitmap->pixel_mode )
    {
    case FT_PIXEL_MODE_MONO:
      ppb = 8;
      break;
    case FT_PIXEL_MODE_GRAY2:
      ppb = 4;
      break;
    case FT_PIXEL_MODE_GRAY4:
      ppb = 2;
      break;
    case FT_PIXEL_MODE_GRAY:
    case FT_PIXEL_MODE_LCD:
    case FT_PIXEL_MODE_LCD_V:
      ppb = 1;
      break;
    default:
      return FT_Err_Invalid_Glyph_Format;
    }
    
    new_pitch = ( bitmap->width + 2 * width + ppb - 1 ) / ppb;
    new_rows = bitmap->rows + 2 * width;

#ifndef LINUX_TURNKEY_SOLUTION
    if (FT_ALLOC(buffer, new_pitch * new_rows))
		return FT_Err_Invalid_Argument;
#else
	buffer = (unsigned char*)x_mem_alloc(new_pitch * new_rows);
	if ( buffer == NULL)
		  return FT_Err_Invalid_Argument;

    x_memset(buffer, 0, new_pitch * new_rows);
#endif

    pitch = bitmap->pitch;
    p = bitmap->buffer;

    pedge = buffer;

    if ( bitmap->pixel_mode == FT_PIXEL_MODE_MONO )
    {
      p += (bitmap->rows - 1) * pitch;
      pedge += (bitmap->rows - 1) * new_pitch;

      for ( y = bitmap->rows - 1; y >= 0 ; y-- )
      {
        /* Horizontally */
        for ( x = pitch - 1; x >= 0; x-- )
        {
          for ( i = 0; i <= 2 * width; i++ )
          {
            pedge[x] |= (p[x] >> i);
            if ( x + 1 < new_pitch )
              pedge[x + 1] |= (p[x] << ( 8 - i ));
          }
        }
        /* Vertically */
        for ( x = 1; x <= 2 * width; x++ )
        {
          for ( i = 0; i < new_pitch; i++ )
          {
            pedge[x * new_pitch + i] |= pedge[i];
          }
        }

        p -= pitch;
        pedge -= new_pitch;
      }

      p = bitmap->buffer;
      pedge = buffer + width * new_pitch;
      for ( y = 0; y < bitmap->rows ; y++ )
      {
        for ( x = pitch - 1; x >= 0; x-- )
        {
          if ( x + 1 < new_pitch )
            pedge[x + 1] &= ~(p[x] << ( 8 - width ));
        }

        p += pitch;
        pedge += new_pitch;
      }
    }
    else
    {
      p2 = p;

      /* fill the value of each pixel to its surrounding edge pixels if it's larger;
         this ensures that each edge pixel gets the largest value */
      for ( y = 0; y < bitmap->rows ; y++ )
      {
        for ( x = 0; x < pitch ; x++ )
        {
          unsigned char gray = p[x];
  
          for ( i = 0; i <= (2 * width); i++ )
          {
            unsigned char *ptemp = &pedge[i * new_pitch + x];
  
            for ( j = 0; j <= (2 * width); j++ )
            {
              ptemp[j] |= gray;
            }
          }
        }

        /* cancel out the font body from edge effect - after the edge pixels get their
           largest value */
        if (y >= width)
        {
          for ( x = width; x < (pitch + width); x++ )
          {
            if (pedge[x] && p2[x - width])
            {
              pedge[x] = pedge[x] * (bitmap->num_grays - p2[x - width]) / (bitmap->num_grays);
            }
          }
          p2 += pitch;
        }
  
        p += pitch;
        pedge += new_pitch;
      }
    }
	
#ifndef LINUX_TURNKEY_SOLUTION
	FT_FREE( bitmap->buffer);
#else
    x_mem_free( bitmap->buffer );
#endif

    bitmap->buffer = buffer;

    bitmap->pitch = new_pitch;
    bitmap->rows = new_rows;
    bitmap->width += 2 * width;

    return FT_Err_Ok;
  }

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blurred( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      width,
                      FT_Int      buf_count)
  {
    int             new_pitch;
    int             new_rows;
    FT_UInt         ppb;
    unsigned char*  buffer = NULL;
    unsigned char*  p;
	unsigned char*  pedge;
	unsigned char*  src_pedge;
	unsigned char   alpha;
    FT_Int          i, j, x, y, pitch;
	unsigned char*  buffer_edge_old = NULL;
    unsigned char   *tmp_buf = NULL;


    if ( !library )
      return FT_Err_Invalid_Library_Handle;

    if ( !bitmap || !bitmap->buffer )
      return FT_Err_Invalid_Argument;

    if ( width <= 0 )
      return FT_Err_Ok;

    pitch = bitmap->pitch;
    if ( pitch < 0 )
      pitch = -pitch;

    switch ( bitmap->pixel_mode )
    {
    case FT_PIXEL_MODE_GRAY:
      ppb = 1;
      break;
    default:
      return FT_Err_Invalid_Glyph_Format;
    }
    
    new_pitch = ( bitmap->width + 2 * width + ppb - 1 ) / ppb;
    new_rows = bitmap->rows + 2 * width;

    buffer = (unsigned char*)x_mem_alloc(new_pitch * new_rows*2);
	  if ( buffer == NULL )
      return FT_Err_Invalid_Argument;
      
	  x_memset(buffer, 0, new_pitch * new_rows*2);

    tmp_buf = (unsigned char*)x_mem_alloc(new_pitch * new_rows);
    if ( tmp_buf == NULL )
    {
    	x_mem_free(buffer);
    	return FT_Err_Invalid_Argument;
    }
    x_memset(tmp_buf, 0, new_pitch * new_rows);


    pitch = bitmap->pitch;
    p = bitmap->buffer;
	  pedge=buffer + new_pitch* new_rows;

    if ( bitmap->pixel_mode == FT_PIXEL_MODE_GRAY)
    {
      #define     PIXEL(start, x, y, pitch)       (*((start) + (y) * (pitch) + (x)))

        /* step 1: copy to tmp_buf */
        for (y = 0; y < bitmap->rows; y++)
        {
            for (x = 0; x < pitch; x++)
            {
                PIXEL(tmp_buf, x + width, y + width, new_pitch) = PIXEL(p, x, y, pitch);
            }

        }

		 if (buf_count == 2)
		 {	
			 src_pedge = p + pitch * bitmap->rows;
		     buffer_edge_old = (unsigned char*)x_mem_alloc(new_pitch * new_rows);
			 if ( buffer_edge_old == NULL)
			 {
			 	x_mem_free(buffer);
				x_mem_free(tmp_buf);
			 	return FT_Err_Invalid_Argument;
			 }
		     x_memset(buffer_edge_old, 0, bitmap->pitch * (bitmap->rows));
			 
			 if ( bitmap->pitch > 0 )
			 {
			   for ( i = 0; i < bitmap->rows; i++ )
				 x_memcpy( buffer_edge_old + new_pitch * i,
							  src_pedge + pitch * i, pitch);
			 }
			 else
			 {
			   for ( i = 0; i < bitmap->rows; i++ )
				 x_memcpy( buffer_edge_old + new_pitch * i,
							  src_pedge +pitch * i, pitch );
			 }			
		  }
		 

        for (y = 0; y < new_rows; y++)
        {
            for (x = 0; x < new_pitch; x++)
            {
                FT_UInt      ui4_sum = 0;
                FT_UInt      ui4_tmp, ui4_count = 0;
                
				 for (i = -width ; i < width ; i++)
                {
					 if (y + i >= 0 && y + i < new_rows )
                    {
						 for (j = -width; j < width ; j++)
                    {
							 if (x + j >= 0 && x + j < new_pitch)
                        {
                            ui4_sum += PIXEL(tmp_buf, x + j, y + i, new_pitch);
                            ui4_count++;
                        }
                    }
                }
				 }

                ui4_tmp = ui4_sum / ((ui4_count != 0)? ui4_count: 1);

                if (x - width >= 0 && x - width < pitch &&
                    y - width >= 0 && y - width < bitmap->rows)
                {
                    ui4_tmp += PIXEL(p, x - width, y - width, pitch);
                }

                ui4_tmp = (ui4_tmp > 255)? 255: ui4_tmp;

                if (ui4_tmp < 31)
                {
                    ui4_tmp = 0;
                }
				 alpha =  PIXEL(tmp_buf, x, y, new_pitch);
				 if( 0 != alpha)
				 {
                PIXEL(buffer, x, y, new_pitch) = ui4_tmp;
					  PIXEL(pedge, x, y, new_pitch) = 0;
				 }
				 else
				 {
					  PIXEL(pedge, x, y, new_pitch) = ui4_tmp;
            }
        }
    }

		  
	  }


	if (buf_count == 2)
	{
		
		pedge = buffer + (new_pitch * new_rows);
		for ( y = 0; y <new_rows ; y++ )
		{
			for ( x = 0; x < new_pitch ; x++ )
			{
				  pedge[x] |= buffer_edge_old[x];
            }
			buffer_edge_old += pitch;
			pedge += pitch;

        }
    }

    x_mem_free( bitmap->buffer );
    x_mem_free( tmp_buf );
    bitmap->buffer = buffer;

    bitmap->pitch = new_pitch;
    bitmap->rows = new_rows;
    bitmap->width += 2 * width;

	if (buffer_edge_old)
	{
		x_mem_free(buffer_edge_old);
	}

    return FT_Err_Ok;
  }
  
  #endif
  
