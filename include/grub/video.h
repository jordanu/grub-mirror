/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2006  Free Software Foundation, Inc.
 *
 *  GRUB is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef GRUB_VIDEO_HEADER
#define GRUB_VIDEO_HEADER	1

#include <grub/err.h>
#include <grub/symbol.h>
#include <grub/types.h>

typedef grub_uint32_t grub_video_color_t;

struct grub_video_render_target;

/* Defines used to describe video mode or rendering target.  */
#define GRUB_VIDEO_MODE_TYPE_ALPHA		0x00000008
#define GRUB_VIDEO_MODE_TYPE_DOUBLE_BUFFERED	0x00000004
#define GRUB_VIDEO_MODE_TYPE_INDEX_COLOR	0x00000002
#define GRUB_VIDEO_MODE_TYPE_RGB		0x00000001

/* Defines used to mask flags.  */
#define GRUB_VIDEO_MODE_TYPE_COLOR_MASK		0x00000003

/* Defines used to specify requested bit depth.  */
#define GRUB_VIDEO_MODE_TYPE_DEPTH_MASK		0x0000ff00
#define GRUB_VIDEO_MODE_TYPE_DEPTH_POS		8

/* Defined predefined render targets.  */
#define GRUB_VIDEO_RENDER_TARGET_DISPLAY	((struct grub_video_render_target *) 0)
#define GRUB_VIDEO_RENDER_TARGET_FRONT_BUFFER	((struct grub_video_render_target *) 0)
#define GRUB_VIDEO_RENDER_TARGET_BACK_BUFFER	((struct grub_video_render_target *) 1)

/* Defined blitting formats.  */
enum grub_video_blit_format
  {
    /* Follow exactly field & mask information.  */
    GRUB_VIDEO_BLIT_FORMAT_RGBA,
    /* Make optimization assumption.  */
    GRUB_VIDEO_BLIT_FORMAT_R8G8B8A8,
    /* Follow exactly field & mask information.  */
    GRUB_VIDEO_BLIT_FORMAT_RGB,
    /* Make optimization assumption.  */
    GRUB_VIDEO_BLIT_FORMAT_R8G8B8,
    /* When needed, decode color or just use value as is.  */
    GRUB_VIDEO_BLIT_FORMAT_INDEXCOLOR
  };

struct grub_video_mode_info
{
  /* Width of the screen.  */
  unsigned int width;
  
  /* Height of the screen.  */
  unsigned int height;

  /* Mode type bitmask.  Contains information like is it Index color or 
     RGB mode.  */
  unsigned int mode_type;

  /* Bits per pixel.  */
  unsigned int bpp;
  
  /* Bytes per pixel.  */
  unsigned int bytes_per_pixel;
  
  /* Pitch of one scanline.  How many bytes there are for scanline.  */
  unsigned int pitch;

  /* In index color mode, number of colors.  In RGB mode this is 256.  */
  unsigned int number_of_colors;

  /* Optimization hint how binary data is coded.  */
  enum grub_video_blit_format blit_format;

  /* How many bits are reserved for red color.  */
  unsigned int red_mask_size;

  /* What is location of red color bits.  In Index Color mode, this is 0.  */
  unsigned int red_field_pos;

  /* How many bits are reserved for green color.  */
  unsigned int green_mask_size;

  /* What is location of green color bits.  In Index Color mode, this is 0.  */
  unsigned int green_field_pos;

  /* How many bits are reserved for blue color.  */
  unsigned int blue_mask_size;

  /* What is location of blue color bits.  In Index Color mode, this is 0.  */
  unsigned int blue_field_pos;

  /* How many bits are reserved in color.  */
  unsigned int reserved_mask_size;

  /* What is location of reserved color bits.  In Index Color mode, 
     this is 0.  */
  unsigned int reserved_field_pos;
};

struct grub_video_render_target
{
  /* Copy of the screen's mode info structure, except that width, height and
     mode_type has been re-adjusted to requested render target settings.  */
  struct grub_video_mode_info mode_info;

  struct
  {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;	
  } viewport;

  /* Indicates wether the data has been allocated by us and must be freed 
     when render target is destroyed.  */
  int is_allocated;
  
  /* Pointer to data.  Can either be in video card memory or in local host's
     memory.  */
  void *data;
};

struct grub_video_palette_data
{
  grub_uint8_t r; /* Red color value (0-255).  */
  grub_uint8_t g; /* Green color value (0-255).  */
  grub_uint8_t b; /* Blue color value (0-255).  */
  grub_uint8_t a; /* Reserved bits value (0-255).  */
};

struct grub_font_glyph;
struct grub_video_bitmap;

struct grub_video_adapter
{
  /* The video adapter name.  */
  const char *name;

  /* Initialize the video adapter.  */
  grub_err_t (*init) (void);

  /* Clean up the video adapter.  */
  grub_err_t (*fini) (void);

  grub_err_t (*setup) (unsigned int width, 
		       unsigned int height,
		       unsigned int mode_type);

  grub_err_t (*get_info) (struct grub_video_mode_info *mode_info);
  
  grub_err_t (*set_palette) (unsigned int start,
			     unsigned int count,
			     struct grub_video_palette_data *palette_data);

  grub_err_t (*get_palette) (unsigned int start,
			     unsigned int count,
			     struct grub_video_palette_data *palette_data);

  grub_err_t (*set_viewport) (unsigned int x,
			      unsigned int y,
			      unsigned int width,
			      unsigned int height);

  grub_err_t (*get_viewport) (unsigned int *x,
			      unsigned int *y,
			      unsigned int *width,
			      unsigned int *height);

  grub_video_color_t (*map_color) (grub_uint32_t color_name);
  
  grub_video_color_t (*map_rgb) (grub_uint8_t red,
			   	 grub_uint8_t green,
			   	 grub_uint8_t blue);

  grub_video_color_t (*map_rgba) (grub_uint8_t red,
				  grub_uint8_t green,
				  grub_uint8_t blue,
				  grub_uint8_t alpha);

  grub_err_t (*fill_rect) (grub_video_color_t color,
			   int x,
			   int y,
			   unsigned int width,
			   unsigned int height);

  grub_err_t (*blit_glyph) (struct grub_font_glyph *glyph,
			    grub_video_color_t color,
			    int x,
			    int y);

  grub_err_t (*blit_bitmap) (struct grub_video_bitmap *bitmap,
			     int x,
			     int y,
			     int offset_x,
			     int offset_y,
			     unsigned int width,
			     unsigned int height);

  grub_err_t (*blit_render_target) (struct grub_video_render_target *source,
				    int x,
				    int y,
				    int offset_x,
				    int offset_y,
				    unsigned int width,
				    unsigned int height);

  grub_err_t (*scroll) (grub_video_color_t color,
  			int dx,
  			int dy);

  grub_err_t (*swap_buffers) (void);

  grub_err_t (*create_render_target) (struct grub_video_render_target **result,
				      unsigned int width,
				      unsigned int height,
				      unsigned int mode_type);

  grub_err_t (*delete_render_target) (struct grub_video_render_target *target);

  grub_err_t (*set_active_render_target) (struct grub_video_render_target *target);

  /* The next video adapter.  */
  struct grub_video_adapter *next;
};
typedef struct grub_video_adapter *grub_video_adapter_t;

void EXPORT_FUNC(grub_video_register) (grub_video_adapter_t adapter);
void EXPORT_FUNC(grub_video_unregister) (grub_video_adapter_t adapter);
void EXPORT_FUNC(grub_video_iterate) (int (*hook) (grub_video_adapter_t adapter));

grub_err_t EXPORT_FUNC(grub_video_setup) (unsigned int width,
					  unsigned int height,
					  unsigned int mode_type);

grub_err_t EXPORT_FUNC(grub_video_restore) (void);

grub_err_t EXPORT_FUNC(grub_video_get_info) (struct grub_video_mode_info *mode_info);

enum grub_video_blit_format EXPORT_FUNC(grub_video_get_blit_format) (struct grub_video_mode_info *mode_info);

grub_err_t EXPORT_FUNC(grub_video_set_palette) (unsigned int start,
						unsigned int count,
						struct grub_video_palette_data *palette_data);

grub_err_t EXPORT_FUNC(grub_video_get_palette) (unsigned int start,
						unsigned int count,
						struct grub_video_palette_data *palette_data);

grub_err_t EXPORT_FUNC(grub_video_set_viewport) (unsigned int x,
						 unsigned int y,
						 unsigned int width,
						 unsigned int height);

grub_err_t EXPORT_FUNC(grub_video_get_viewport) (unsigned int *x,
						 unsigned int *y,
						 unsigned int *width,
						 unsigned int *height);

grub_video_color_t EXPORT_FUNC(grub_video_map_color) (grub_uint32_t color_name);

grub_video_color_t EXPORT_FUNC(grub_video_map_rgb) (grub_uint8_t red,
						    grub_uint8_t green,
						    grub_uint8_t blue);

grub_video_color_t EXPORT_FUNC(grub_video_map_rgba) (grub_uint8_t red,
						     grub_uint8_t green,
						     grub_uint8_t blue,
						     grub_uint8_t alpha);

grub_err_t EXPORT_FUNC(grub_video_fill_rect) (grub_video_color_t color,
					      int x,
					      int y,
					      unsigned int width,
					      unsigned int height);

grub_err_t EXPORT_FUNC(grub_video_blit_glyph) (struct grub_font_glyph *glyph,
					       grub_video_color_t color,
					       int x,
					       int y);

grub_err_t EXPORT_FUNC(grub_video_blit_bitmap) (struct grub_video_bitmap *bitmap,
						int x,
						int y,
						int offset_x,
						int offset_y,
						unsigned int width,
						unsigned int height);

grub_err_t EXPORT_FUNC(grub_video_blit_render_target) (struct grub_video_render_target *source,
						       int x,
						       int y,
						       int offset_x,
						       int offset_y,
						       unsigned int width,
						       unsigned int height);

grub_err_t EXPORT_FUNC(grub_video_scroll) (grub_video_color_t color,
					   int dx,
					   int dy);

grub_err_t EXPORT_FUNC(grub_video_swap_buffers) (void);

grub_err_t EXPORT_FUNC(grub_video_create_render_target) (struct grub_video_render_target **result,
							 unsigned int width,
							 unsigned int height,
							 unsigned int mode_type);

grub_err_t EXPORT_FUNC(grub_video_delete_render_target) (struct grub_video_render_target *target);

grub_err_t EXPORT_FUNC(grub_video_set_active_render_target) (struct grub_video_render_target *target);
					   
#endif /* ! GRUB_VIDEO_HEADER */