#ifndef mouse_include_file
#define mouse_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define mouse_width 41
#define mouse_height 41
#define mouse_size 1683
#define mouse ((gfx_sprite_t*)mouse_data)
extern unsigned char mouse_data[1683];

#ifdef __cplusplus
}
#endif

#endif
