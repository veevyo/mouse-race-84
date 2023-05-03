#include <sys/util.h>
#include <graphx.h>
#include <keypadc.h>
#include <sys/timers.h>
#include <compression.h>
#include <ti/screen.h>
#include <fontlibc.h>
#include "gfx/gfx.h"
#include "fonts/fonts.h"

#define START_X_MOU ((GFX_LCD_WIDTH - mouse_width) / 2)
#define START_Y_MOU 210
#define START_X_CAT 200
#define START_Y_CAT 10
#define TIME (*(volatile uint32_t*)0xF30044)

gfx_UninitedSprite(background, mouse_width, mouse_height);
gfx_UninitedSprite(background1, cat_width, cat_height);

void DrawSprite(int x, int y, int sprite_type); //0 for mouse, 1 for cat
void input(int *y);
void moveCat(int *x, int *y);
int collisionCheck(int x1, int y1, int x2, int y2);
char * itoa(int num, char * buffer, int base);

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
	int score = 0;
	int lives = 3;

    	gfx_Begin();

    	gfx_SetPalette(global_palette, sizeof_global_palette, 0);
   	//gfx_FillScreen(1);
	//zx7_Decompress(gfx_vram, floor_compressed);
    	//gfx_SetTransparentColor(0);

   	gfx_SetDrawBuffer();

    	gfx_GetSprite(background, m_x, m_y);
    	gfx_GetSprite(background1, c_x, c_y);

    	DrawSprite(m_x, m_y,0);
    	DrawSprite(c_x, c_y,1);
	gfx_Sprite_NoClip(cheese, 150, 5);

	char buffer[20]; //different kind of buffer; for atoi
        fontlib_SetFont(test_font, 0);
        fontlib_SetWindow(300, 0, 10, 10);
        fontlib_SetCursorPosition(10, 10);
        fontlib_SetColors(0, 1);
	fontlib_DrawString("Lives: ");
        fontlib_DrawString(itoa(lives, buffer, 10));

	fontlib_SetFont(test_font, 0);
        fontlib_SetCursorPosition(250, 10);
        fontlib_SetColors(0, 1);
        fontlib_DrawString("Score: ");
        fontlib_DrawString(itoa(score, buffer, 10));

    	gfx_BlitBuffer();

    	do {
		input(&m_y);
		moveCat(&c_x, &c_y);
            	DrawSprite(m_x, m_y,0);
		DrawSprite(c_x, c_y, 1);
		if (collisionCheck(m_x, m_y, c_x, c_y)) {
			lives--;
			m_x = START_X_MOU;
			m_y = START_Y_MOU;
			fontlib_ClearWindow();
                        fontlib_SetCursorPosition(10, 10);
                        fontlib_DrawString("Lives: ");
                        fontlib_DrawString(itoa(lives, buffer, 10));
		}
		else if (collisionCheck(m_x, m_y, 150, 5)) {
			score++;
			m_x = START_X_MOU;
                        m_y = START_Y_MOU;
			fontlib_ClearWindow();
			fontlib_SetCursorPosition(250, 10);
			fontlib_DrawString("Score: ");
			fontlib_DrawString(itoa(score, buffer, 10));
		}
            	gfx_BlitBuffer();
    	} while (lives);

	if (!lives) {
		os_ClrLCD();
		zx7_Decompress(gfx_vram, floor_compressed);
		delay(2500);
	}

	if (kb_Data[6] == kb_Clear) {
    		gfx_End();
    		return 0;
	}
}

void DrawSprite(int x, int y, int sprite_type)
{
	if (sprite_type == 0) {
    		static int oldX = START_X_MOU;
    		static int oldY = START_Y_MOU;
    		gfx_Sprite(background, oldX, oldY);

    		gfx_GetSprite(background, x, y);

    		gfx_TransparentSprite(mouse, x, y);

    		oldX = x;
   	 	oldY = y;
	}
	else {
		static int oldX = START_X_CAT;
                static int oldY = START_Y_CAT;
                gfx_Sprite(background1, oldX, oldY);

                gfx_GetSprite(background1, x, y);

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
	*x -= 7;
        if (*x < 5) {
        	*x = 300;
                *y = randInt(0, 200 - cat_height);
       	}
}

int inRange(int min, int max, int value) {
        if (min <= value && value <= max) {
                return 1;
        }
        return 0;
}

int collisionCheck(int x1, int y1, int x2, int y2) {
	delay(1);
        if (inRange(0, 30, abs(x1 - x2)) && inRange(0, 30, abs(y1 - y2))) {
                return 1;
        }
        return 0;
}
void reverseString(char str[], int length) {
        int start = 0;
        int end = length - 1;
        while (start < end) {
                char temp = str[start];
                str[start] = str[end];
                str[end] = temp;
                end--;
                start++;
        }
}
char* itoa(int num, char* str, int base)
{
        int i = 0;
        int isNegative = 0;
        if (num == 0) {
                str[i++] = '0';
                str[i] = '\0';
                return str;
        }
        if (num < 0 && base == 10) {
                isNegative = 1;
                num = -num;
        }
        while (num != 0) {
                int rem = num % base;
                str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
                num = num / base;
        }
        if (isNegative) {
                str[i++] = '-';
        }
        str[i] = '\0';
        reverseString(str, i);

        return str;
}
