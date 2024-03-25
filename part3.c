#include <stdlib.h>

volatile int pixel_buffer_start; // global variable
short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];

void wait_for_vsync() {
    volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
    register int status;

    *pixel_ctrl_ptr = 1;

    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0) {
        status = *(pixel_ctrl_ptr + 3);
    }
}

void clear_screen() {
    int x, y;
    for (y = 0; y < 240; y++) {
        for (x = 0; x < 320; x++) {
            Buffer2[y][x] = 0; // clear screen by setting all pixels to black
        }
    }
}

// Function to plot a pixel at a given position with a given color
void plot_pixel(int x, int y, short int line_color)
{
    volatile short int *one_pixel_address;

    one_pixel_address = (volatile short int *)(pixel_buffer_start + (y << 10) + (x << 1));

    *one_pixel_address = line_color;
}

void draw_box(int x, int y, short int color) {
    int i, j;
    for (i = x; i < x + 10; i++) {
        for (j = y; j < y + 10; j++) {
            plot_pixel(i, j, color); // draw a box at (x, y) with specified color
        }
    }
}

int main(void) {
    volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
    int i, x, y, dx, dy;

    *(pixel_ctrl_ptr + 1) = (int)&Buffer1; // set front pixel buffer to Buffer1
    wait_for_vsync(); // swap front and back buffers
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // initialize pointer to pixel buffer
    clear_screen(); // clear screen

    *(pixel_ctrl_ptr + 1) = (int)&Buffer2; // set back pixel buffer to Buffer2
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // initialize pointer to pixel buffer
    clear_screen(); // clear screen

    while (1) {
        clear_screen(); // clear previous boxes
        for (i = 0; i < 8; i++) {
            // generate random coordinates for each box
            x = rand() % 311; // ensure boxes stay within bounds
            y = rand() % 231;
            dx = (rand() % 2 == 0) ? 1 : -1; // random direction for x
            dy = (rand() % 2 == 0) ? 1 : -1; // random direction for y
            
            // draw boxes at new positions
            draw_box(x, y, 0xFFFF); // draw white box
            
            // update positions for next iteration
            x += dx;
            y += dy;

            // handle bouncing off edges
            if (x <= 0 || x >= 310) {
                dx = -dx;
            }
            if (y <= 0 || y >= 230) {
                dy = -dy;
            }
        }
        wait_for_vsync(); // swap front and back buffers
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // initialize pointer to pixel buffer
    }

    return 0;
}
