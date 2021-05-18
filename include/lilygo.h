#ifndef _LILY_GO_H
#define _LILY_GO_H

#define PORTRAIT_MODE 0
#define LANDSCAPE_MODE 1

#define _FONT_LIGHT_   1
#define _FONT_NORMAL_  2
#define _FONT_BOLD_    3

extern float batt_voltage;

void EPD_print_text(int font_size, char *text, int x_pos, int y_pos, int style);
void EPD_draw_rect(int x_pos, int y_pos, int width, int heigth);
void EPD_init(int &width, int &height);
void EPD_enter();
void EPD_leave();
void EPD_power_off();
void EPD_orient(int orientation);
void EPD_draw_logo();

#endif