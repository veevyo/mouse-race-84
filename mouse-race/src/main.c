#include <sys/util.h>
#include <graphx.h>
#include <keypadc.h>
#include <sys/timers.h>
#include <ti/screen.h>
/* Include the sprite data */
#include "gfx/gfx.h"

#define START_X_MOU ((GFX_LCD_WIDTH - mouse_width) / 2)
#define START_Y_MOU 210
#define START_X_CAT 200
#define START_Y_CAT 10
#define TIME (*(volatile uint32_t*)0xF30044)

/* Create a buffer to store the background behind the sprite */
gfx_UninitedSprite(background, mouse_width, mouse_height);
gfx_UninitedSprite(background1, cat_width, cat_height);

void DrawSprite(int x, int y, int sprite_type); //0 for mouse, 1 for cat
void input(int *y);
void moveCat(int *x, int *y);
int collisionCheck(int x1, int y1, int x2, int y2);

int main(void)
{
	srandom(TIME);
    	int i;

    	background->width = mouse_width;
    	background->height = mouse_height;

	background1->width = cat_width;
	background1->height = cat_height;

    	int m_x = START_X_MOU;
    	int m_y = START_Y_MOU;
	int c_x = START_X_CAT;
	int c_y = START_Y_CAT;

    	gfx_Begin();

    	/* Set the palette for the sprites */
    	gfx_SetPalette(global_palette, sizeof_global_palette, 0);
   	gfx_FillScreen(1);
    	gfx_SetTransparentColor(0);

    	/* Draw to the offscreen buffer */
   	gfx_SetDrawBuffer();

    	/* Draw a bunch of random sprites to demonstrate the partial redraw */

    	/* Get the original background behind the sprite */
    	gfx_GetSprite(background, m_x, m_y);
    	gfx_GetSprite(background1, c_x, c_y);

    	/* Draw the main sprite */
    	DrawSprite(m_x, m_y,0);
    	DrawSprite(c_x, c_y,1);
	gfx_Sprite_NoClip(cheese, 150, 5);
    	/* Copy the buffer to the screen */
    	/* Same as gfx_Blit(gfx_buffer) */
    	gfx_BlitBuffer();

    	do {
		input(&m_y);
		moveCat(&c_x, &c_y);
            	DrawSprite(m_x, m_y,0);
		DrawSprite(c_x, c_y, 1);
		if (collisionCheck(m_x, m_y, c_x, c_y)) {
			m_x = START_X_MOU;
			m_y = START_Y_MOU;
		}

            	gfx_BlitBuffer();

    	} while (kb_Data[6] != kb_Clear);

    	/* End graphics drawing */
    	gfx_End();

    	return 0;
}

/* Function for drawing the main sprite */
void DrawSprite(int x, int y, int sprite_type)
{
	if (sprite_type == 0) {
    		static int oldX = START_X_MOU;
    		static int oldY = START_Y_MOU;

    		/* Render the original background */
    		gfx_Sprite(background, oldX, oldY);

    		/* Get the background behind the sprite */
    		gfx_GetSprite(background, x, y);

    		/* Render the sprite */
    		gfx_TransparentSprite(mouse, x, y);

    		oldX = x;
   	 	oldY = y;
	}
	else {
		static int oldX = START_X_CAT;
                static int oldY = START_Y_CAT;

                /* Render the original background */
                gfx_Sprite(background1, oldX, oldY);

                /* Get the background behind the sprite */
                gfx_GetSprite(background1, x, y);

                /* Render the sprite */
                gfx_TransparentSprite(cat, x, y);

                oldX = x;
                oldY = y;
	}

}

void input(int *y) {
	kb_key_t arrows;

        kb_Scan();

        arrows = kb_Data[7];

        if (arrows)
	{
            	if ((arrows & kb_Down) && *y <= 200) {
                	*y += 2;
            	}
            	if ((arrows & kb_Up)) {
               		*y -= 2;
        	}
	}
}

void moveCat(int *x, int *y) {
	*x -= 5;
        if (*x < 5) {
        	*x = 300;
                *y = randInt(0, 240 - cat_height);
       	}
}

int inRange(int min, int max, int value) {
        if (min <= value && value <= max) {
                return 1;
        }
        return 0;
}

int collisionCheck(int x1, int y1, int x2, int y2) {
        if (inRange(0, 30, abs(x1 - x2)) && inRange(0, 30, abs(y1 - y2))) {
                return 1;
        }
        return 0;
}
