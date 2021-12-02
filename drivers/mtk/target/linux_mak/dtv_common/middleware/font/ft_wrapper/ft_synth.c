#include <ft2build.h>
#include FT_SYNTHESIS_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_TYPES_H
#include "ft_bitmap.h"
#include "ft_synth.h"
#include "os/inc/x_os.h"

#ifndef FT_GLYPH_OWN_BITMAP
#define FT_GLYPH_OWN_BITMAP  0x1
#endif

#ifndef FE_DISABLE_EDGE_EFFECT_SUPPORT

#ifndef FT_GlyphSlot_InternalRec
typedef struct  FT_Slot_InternalRec_
{
void*           loader;//pointer to FT_GlyphLoaderRec_*   
FT_UInt         flags;
FT_Bool         glyph_transformed;
FT_Matrix       glyph_matrix;
FT_Vector       glyph_delta;
void*           glyph_hints;

} FT_GlyphSlot_InternalRec;
#endif
/*
     Strength of edge effect
  */
  static FT_Pos     Shadow_DispX = 4;   /* x-axis displacement of shadow effect */
  static FT_Pos     Shadow_DispY = 1;   /* y-axis displacement of shadow effect */
  static FT_Pos     Raise_DispX = 3;    /* x-axis displacement of raise and depress effect */
  static FT_Pos     Raise_DispY = 1;    /* x-axis displacement of raise and depress effect */
  static FT_Pos     Uniform_Thick = 1;  /* thickness of uniform effect */
  static FT_Pos     Outline_Thick = 1;  /* thickness of outline effect */

FT_EXPORT_DEF( void )
  FT_GlyphSlot_ShadowRight( FT_GlyphSlot  slot,
                            FT_Bool       fg_double_buffer)
  {
    FT_Library  library = slot->library;
    FT_Error    error   = FT_Err_Ok;
    FT_Pos      dx, dy;
    FT_Int      buf_count = 1;

    /* some reasonable strength */
    dx = Shadow_DispX;
    dy = Shadow_DispY;
    if (fg_double_buffer)
    {
        buf_count = 2;
    }
    if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_Err_Unimplemented_Feature;
      return;   /* unsupported */
    }
    else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
    {
      /* slot must be bitmap-owner */
      if ( !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
      {
        FT_Bitmap  bitmap;

        FT_Bitmap_New( &bitmap );
        error = _FT_Bitmap_Copy( library, &slot->bitmap, &bitmap, buf_count);

        if ( !error )
        {
          slot->bitmap = bitmap;
          slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
        }
      }

      if ( !error )
        error = FT_Bitmap_Shadow( library, &slot->bitmap, dx, dy, buf_count);
    }
    else
      error = FT_Err_Invalid_Argument;

    /* modify the metrics accordingly */
    if ( !error )
    {
      slot->advance.x += dx;
      slot->advance.y += dy;

      slot->metrics.width        += dx;
      slot->metrics.height       += dy;
      slot->metrics.horiBearingY += dy;
      slot->metrics.horiAdvance  += dx;
      slot->metrics.vertBearingX -= dx / 2;
      slot->metrics.vertBearingY += dy;
      slot->metrics.vertAdvance  += dy;

      if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
        slot->bitmap_top += dy >> 6;
    }
  }


  FT_EXPORT_DEF( FT_Short )
  FT_GlyphSlot_ShadowRight_Width_Inc( void )
  {
    return (FT_Short) Shadow_DispX;
  }


  FT_EXPORT_DEF( void )
  FT_GlyphSlot_ShadowLeft( FT_GlyphSlot  slot,
                           FT_Bool       fg_double_buffer)
  {
    FT_Library  library = slot->library;
    FT_Error    error   = FT_Err_Ok;
    FT_Pos      dx, dy;
    FT_Int      buf_count = 1;
    /* some reasonable strength */
    dx = Shadow_DispX;
    dy = Shadow_DispY;

    if (fg_double_buffer)
    {
        buf_count = 2;
    }

    if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_Err_Unimplemented_Feature;
      return;   /* unsupported */
    }
    else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
    {
      /* slot must be bitmap-owner */
      if ( !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
      {
        FT_Bitmap  bitmap;

        FT_Bitmap_New( &bitmap );
        error = _FT_Bitmap_Copy( library, &slot->bitmap, &bitmap, buf_count );

        if ( !error )
        {
          slot->bitmap = bitmap;
          slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
        }
      }

      if ( !error )
        error = FT_Bitmap_Shadow( library, &slot->bitmap, -dx, dy, buf_count);
    }
    else
      error = FT_Err_Invalid_Argument;

    /* modify the metrics accordingly */
    if ( !error )
    {
      slot->advance.x += dx;
      slot->advance.y += dy;

      slot->metrics.width        += dx;
      slot->metrics.height       += dy;
      slot->metrics.horiBearingY += dy;
      slot->metrics.horiAdvance  += dx;
      slot->metrics.vertBearingX -= dx / 2;
      slot->metrics.vertBearingY += dy;
      slot->metrics.vertAdvance  += dy;

      if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
        slot->bitmap_top += dy >> 6;
    }
  }


  FT_EXPORT_DEF( FT_Short )
  FT_GlyphSlot_ShadowLeft_Width_Inc( void )
  {
    return (FT_Short) Shadow_DispX;
  }


  FT_EXPORT_DEF( void )
  FT_GlyphSlot_Depressed( FT_GlyphSlot  slot,
                          FT_Bool       fg_double_buffer)
  {
    FT_Library  library = slot->library;
    FT_Error    error   = FT_Err_Ok;
    FT_Pos      dx, dy;
    FT_Int      buf_count = 1;

    /* some reasonable strength */
    dx = Raise_DispX;
    dy = Raise_DispY;
    
    if (fg_double_buffer)
    {
        buf_count = 2;
    }

    if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_Err_Unimplemented_Feature;
      return;   /* unsupported */
    }
    else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
    {
      /* slot must be bitmap-owner */
      if ( !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
      {
        FT_Bitmap  bitmap;

        FT_Bitmap_New( &bitmap );
        error = _FT_Bitmap_Copy( library, &slot->bitmap, &bitmap, buf_count);

        if ( !error )
        {
          slot->bitmap = bitmap;
          slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
        }
      }

      if ( !error )
        error = FT_Bitmap_Raised( library, &slot->bitmap, -dx, -dy, buf_count);
    }
    else
      error = FT_Err_Invalid_Argument;

    /* modify the metrics accordingly */
    if ( !error )
    {
      slot->advance.x += dx;
      slot->advance.y += dy;

      slot->metrics.width        += dx;
      slot->metrics.height       += dy;
      slot->metrics.horiBearingY += dy;
      slot->metrics.horiAdvance  += dx;
      slot->metrics.vertBearingX -= dx / 2;
      slot->metrics.vertBearingY += dy;
      slot->metrics.vertAdvance  += dy;

      if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
        slot->bitmap_top += dy >> 6;
    }
  }


  FT_EXPORT_DEF( FT_Short )
  FT_GlyphSlot_Depressed_Width_Inc( void )
  {
    return (FT_Short) Raise_DispX;
  }


  FT_EXPORT_DEF( void )
  FT_GlyphSlot_Raised( FT_GlyphSlot  slot ,
                       FT_Bool       fg_double_buffer)
  {
    FT_Library  library = slot->library;
    FT_Error    error   = FT_Err_Ok;
    FT_Pos      dx, dy;
    FT_Int      buf_count = 1;
    /* some reasonable strength */
    dx = Raise_DispX;
    dy = Raise_DispY;
    
    if (fg_double_buffer)
    {
        buf_count = 2;
    }
    if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_Err_Unimplemented_Feature;
      return;   /* unsupported */
    }
    else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
    {
      /* slot must be bitmap-owner */
      if ( !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
      {
        FT_Bitmap  bitmap;

        FT_Bitmap_New( &bitmap );
        error = _FT_Bitmap_Copy( library, &slot->bitmap, &bitmap,buf_count );

        if ( !error )
        {
          slot->bitmap = bitmap;
          slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
        }
      }

      if ( !error )
        error = FT_Bitmap_Raised( library, &slot->bitmap, dx, dy,buf_count );
    }
    else
      error = FT_Err_Invalid_Argument;

    /* modify the metrics accordingly */
    if ( !error )
    {
      slot->advance.x += dx;
      slot->advance.y += dy;

      slot->metrics.width        += dx;
      slot->metrics.height       += dy;
      slot->metrics.horiBearingY += dy;
      slot->metrics.horiAdvance  += dx;
      slot->metrics.vertBearingX -= dx / 2;
      slot->metrics.vertBearingY += dy;
      slot->metrics.vertAdvance  += dy;

      if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
        slot->bitmap_top += dy >> 6;
    }
  }


  FT_EXPORT_DEF( FT_Short )
  FT_GlyphSlot_Raised_Width_Inc( void )
  {
    return (FT_Short) Raise_DispX;
  }

  FT_EXPORT_DEF( void )
  FT_GlyphSlot_Blurred( FT_GlyphSlot  slot , 
                        FT_UFWord     ui2_kernel_width,
                        FT_Bool       fg_double_buffer)
  {
    FT_Library  library = slot->library;
    FT_Error    error   = FT_Err_Ok;
    FT_Pos      width = ui2_kernel_width;
    FT_Int      buf_count = 1;

    width = width << 6;

    if (fg_double_buffer)
    {
        buf_count = 2;
    }

    if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_Err_Unimplemented_Feature;
      return;   /* unsupported */
    }
    else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
    {
      /* slot must be bitmap-owner */
      if ( !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
      {
        FT_Bitmap  bitmap;

        FT_Bitmap_New( &bitmap );
        error = _FT_Bitmap_Copy( library, &slot->bitmap, &bitmap, buf_count);

        if ( !error )
        {
          slot->bitmap = bitmap;
          slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
        }
      }

      if ( !error )
        error = FT_Bitmap_Blurred( library, &slot->bitmap, width >> 6, buf_count);
    }
    else
      error = FT_Err_Invalid_Argument;

    /* modify the metrics accordingly */
    if ( !error )
    {
      slot->advance.x += 2 * width;
      slot->advance.y += 2 * width;

      slot->metrics.width        += 2 * width;
      slot->metrics.height       += 2 * width;
      slot->metrics.horiBearingY += 2 * width;
      slot->metrics.horiAdvance  += 2 * width;
      slot->metrics.vertBearingX -= width;
      slot->metrics.vertBearingY += 2 * width;
      slot->metrics.vertAdvance  += 2 * width;
      /*slot->bitmap_left          += width >> 6;*/
      /*slot->bitmap_top           += width >> 6;*/

      if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
        slot->bitmap_top += (width >> 6);
    }
  }

  FT_EXPORT_DEF( void )
  _ft_glyph_uniform(FT_GlyphSlot  slot, 
                    FT_Pos        in_width,
                    FT_Bool       fg_double_buffer)
  {
    FT_Library  library = slot->library;
    FT_Error    error   = FT_Err_Ok;
    FT_Pos      width = in_width;
    FT_Int      buf_count = 1;

    if (fg_double_buffer)
    {
        buf_count = 2;
    }
    
    if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_Err_Unimplemented_Feature;
      return;   /* unsupported */
    }
    else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
    {
      /* slot must be bitmap-owner */
      if ( !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
      {
        FT_Bitmap  bitmap;

        FT_Bitmap_New( &bitmap );
        error = _FT_Bitmap_Copy( library, &slot->bitmap, &bitmap, buf_count );

        if ( !error )
        {
          slot->bitmap = bitmap;
          slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
        }
      }

      if ( !error )
        error = FT_Bitmap_Uniform( library, &slot->bitmap, width, buf_count);
    }
    else
      error = FT_Err_Invalid_Argument;

    /* modify the metrics accordingly */
    if ( !error )
    {
      slot->advance.x += 2 * width;
      slot->advance.y += 2 * width;

      slot->metrics.width        += 2 * width;
      slot->metrics.height       += 2 * width;
      slot->metrics.horiBearingY += 2 * width;
      slot->metrics.horiAdvance  += 2 * width;
      slot->metrics.vertBearingX -= width;
      slot->metrics.vertBearingY += 2 * width;
      slot->metrics.vertAdvance  += 2 * width;

      if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
        slot->bitmap_top += width >> (6 - 1);
    }
  }


  FT_EXPORT_DEF( void )
  FT_GlyphSlot_Uniform( FT_GlyphSlot  slot ,
                        FT_Bool       fg_double_buffer)
  {
    _ft_glyph_uniform(slot, Uniform_Thick, fg_double_buffer);
  }


  FT_EXPORT_DEF( void )
  FT_GlyphSlot_Custom_Uniform( FT_GlyphSlot  slot , 
                               FT_UFWord     ui2_uniform_width,
                               FT_Bool       fg_double_buffer)
  {
    _ft_glyph_uniform(slot, (FT_Pos)ui2_uniform_width, fg_double_buffer);
  }


  FT_EXPORT_DEF( FT_Short )
  FT_GlyphSlot_Uniform_Width_Inc( void )
  {
    return (FT_Short) 2 * Uniform_Thick;
  }


  FT_EXPORT_DEF( void )
  _ft_glyph_outline( FT_GlyphSlot  slot, 
                     FT_Pos        in_width)
  {
    FT_Library  library = slot->library;
    FT_Error    error   = FT_Err_Ok;
    FT_Pos      width = in_width;
    
    if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_Err_Unimplemented_Feature;
      return;   /* unsupported */
    }
    else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
    {
      /* slot must be bitmap-owner */
      if ( !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
      {
        FT_Bitmap  bitmap;

        FT_Bitmap_New( &bitmap );
        error = FT_Bitmap_Copy( library, &slot->bitmap, &bitmap);

        if ( !error )
        {
          slot->bitmap = bitmap;
          slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
        }
      }

      if ( !error )
        error = FT_Bitmap_Outline( library, &slot->bitmap, width);
    }
    else
      error = FT_Err_Invalid_Argument;

    /* modify the metrics accordingly */
    if ( !error )
    {
      slot->advance.x += 2 * width;
      slot->advance.y += 2 * width;

      slot->metrics.width        += 2 * width;
      slot->metrics.height       += 2 * width;
      slot->metrics.horiBearingY += 2 * width;
      slot->metrics.horiAdvance  += 2 * width;
      slot->metrics.vertBearingX -= width;
      slot->metrics.vertBearingY += 2 * width;
      slot->metrics.vertAdvance  += 2 * width;

      if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
        slot->bitmap_top += width >> (6 - 1);
    }
  }
  
  FT_EXPORT_DEF( void )
  FT_GlyphSlot_Outline( FT_GlyphSlot  slot)
  {
    _ft_glyph_outline(slot, Outline_Thick);
  }

  FT_EXPORT_DEF( void )
  FT_GlyphSlot_Custom_Outline( FT_GlyphSlot  slot, 
                               FT_UFWord     ui2_outline_width )
  {
    _ft_glyph_outline(slot, (FT_Pos)ui2_outline_width);
  }

  FT_EXPORT_DEF( FT_Short )
  FT_GlyphSlot_Outline_Width_Inc( void )
  {
    return (FT_Short) 2 * Outline_Thick;
  }
#endif
  
