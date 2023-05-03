#ifndef mouse_include_file
#define mouse_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define mouse_width 14
#define mouse_height 24
#define mouse_size 338
#define mouse ((gfx_sprite_t*)mouse_data)
extern unsigned char mouse_data[338];

#ifdef __cplusplus
}
#endif

#endif
