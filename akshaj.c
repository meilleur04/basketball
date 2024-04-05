void plot_pixel(int x, int y, short int line_color);   // plots one pixel
void clear_screen();				       // clears whole screen
void draw_car(int offset_x, int offset_y, int colour);  // draws car
void draw_road(int, int, int, int); // Draws still road bg
void draw_object(short int array[], int offset_x, int offset_y, int width, int height);
void wait_for_vsync();
void clear_car(int offset_x, int offset_y);
void HexDisplay(int score);
void draw_car_flip(int offset_x, int offset_y, int colour);
void clear_car_flip(int offset_x, int offset_y);
void animate_lines(int t);
void draw_line(int pos_x, int pos_y);
void draw_sides(int t, volatile int* pixel_ctrl_ptr);
void animate_object(short int array[], int pos_x, int pos_y, int width, int height, int t);

short int border, base, shadow, window, window_shadow, headlight, taillight, drop_shadow;
short int police[1980];
int car[1980];
short int tree[4872];
short int money[888];
short int leaf[224];
short int grass[56];
int x_pos[4] = {76, 120, 166, 210};
int money_pos[2] = {190, 0};
int player_position[2] = {166, 170};
bool isScore;
int score = 0;
int car1_pos[2] = {0, 0};
int car2_pos[2] = {0, 0};
int pos = 1;
int pos_prev = 1;
bool car1 = false;
bool car2 = false;
bool money_bool = false;
int dy = 2;

int pixel_buffer_start; 
bool game_start = false;
volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
volatile int * PS2_ptr = (int *)0xFF200100;
int PS2_data, RVALID;
char byte1 = 0, byte2 = 0, byte3 = 0;
void game();

int main(void){
  pixel_buffer_start = *pixel_ctrl_ptr;
  	while (1)
    {
      clear_screen();
    while (!game_start)
    {
      PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
		RVALID = (PS2_data & 0x8000);	// extract the RVALID field
		if (RVALID != 0)
		{
			/* always save the last three bytes received */
			byte1 = byte2;
			byte2 = byte3;
			byte3 = PS2_data & 0xFF;
      if (byte2 == (char)0xF0)
    {
      if (byte3 == (char)0x29)
      {
        game_start = true;
      }
    }
		}
    }
    game();
    }
}

void game(){
  pixel_buffer_start = *pixel_ctrl_ptr;
  draw_road(0, 0, 320, 240);
	draw_object(tree, 10, 50, TREE_WIDTH, TREE_HEIGHT);
	draw_object(leaf, 50, 30, LEAF_WIDTH, LEAF_HEIGHT);
	draw_object(grass, 20, 150, GRASS_WIDTH, GRASS_HEIGHT);
    // draw_object(money, money_pos[0], money_pos[1], MONEY_WIDTH, MONEY_HEIGHT);
    int car_1_x = x_pos[rand() % 4];
    car1_pos[0] = car_1_x;
    car1_pos[1] = 0;
    player_position[0] = 166;
    player_position[1] = 170;
    money_bool = false;
    car2 = false;
    draw_car_flip(car1_pos[0], car1_pos[1], 5);
    car1 = true;
    score = 0;
    //set a timer of 0.2 sec
    HexDisplay(0);
    pos = 3;
    draw_object(police, player_position[0], player_position[1], CAR_WIDTH, CAR_HEIGHT);
    int i = 0;
    while (1)
    {
        if (score == 5)
        {
          dy = 3;
        } else if (score == 10)
        {
          dy = 4;
        } else if (score == 15)
        {
          game_start = false;
          break;
        }
        
        animate_lines(i);
        if (car1_pos[1] == 84 && money_bool == false)
        {
            printf("Money");
            money_bool = true;
            money_pos[0] = x_pos[rand() % 4];
            money_pos[1] = - MONEY_HEIGHT;
            draw_object(money, money_pos[0], money_pos[1], MONEY_WIDTH, MONEY_HEIGHT);
        }
        if (money_bool)
        {
            draw_road(money_pos[0], money_pos[1] - dy, money_pos[0] + MONEY_WIDTH, money_pos[1] + MONEY_HEIGHT);
            money_pos[1] = money_pos[1] + dy;
            draw_object(money, money_pos[0], money_pos[1], MONEY_WIDTH, MONEY_HEIGHT);
        }
    //   draw_road(money_pos[0], money_pos[1], money_pos[0] + MONEY_WIDTH, money_pos[1] + MONEY_HEIGHT);
    //   money_pos[1] = money_pos[1] + 1;
    //   draw_object(money, money_pos[0], money_pos[1], MONEY_WIDTH, MONEY_HEIGHT);
      if (money_bool && (money_pos[0] == player_position[0] && ((money_pos[1] + MONEY_HEIGHT) >= player_position[1] && money_pos[1] <= player_position[1] + CAR_HEIGHT)))
      {
        score += 1;
        HexDisplay(score);
        draw_road(money_pos[0], money_pos[1], money_pos[0] + MONEY_WIDTH, money_pos[1] + MONEY_HEIGHT);
        draw_object(police, player_position[0], player_position[1], CAR_WIDTH, CAR_HEIGHT);
        money_bool = false;
      }      
      if (money_bool && money_pos[1] > 240)
      {
        printf("Money missed\n");
        money_bool = false;
      }
      if (car1_pos[1] == 168 && car2 == false)
      {
        car2 = true;
        car2_pos[0] = x_pos[rand() % 4];
        while (car2_pos[0] == car1_pos[0])
        {
          car2_pos[0] = x_pos[rand() % 4];
        }
        car2_pos[1] =  - CAR_HEIGHT;
        draw_car_flip(car2_pos[0], car2_pos[1], 5);
      }
      if (car2)
      {
        clear_car_flip(car2_pos[0], car2_pos[1]);
        car2_pos[1] += dy;
        draw_car_flip(car2_pos[0], car2_pos[1], 5);
      }

      if (car2 && car2_pos[0] == player_position[0] && ((car2_pos[1] + CAR_HEIGHT) >= player_position[1] && car2_pos[1] <= player_position[1] + CAR_HEIGHT))
    {
      game_start = false;
      break;
    }
    if (car2_pos[1] > 240)
    {
      car2 = false;
    }
    
      
      
    draw_road(car1_pos[0], car1_pos[1] - dy, car1_pos[0] + CAR_WIDTH, car1_pos[1] + CAR_HEIGHT);
    car1_pos[1] = car1_pos[1] + dy;
    draw_car_flip(car1_pos[0], car1_pos[1], 5);
    if (car1_pos[0] == player_position[0] && ((car1_pos[1] + CAR_HEIGHT) >= player_position[1] && car1_pos[1] <= player_position[1] + CAR_HEIGHT))
    {
      game_start = false;
      break;
    }
    if (car1_pos[1] > 240)
    {
      printf("Car 1\n");
      car1_pos[0] = x_pos[rand() % 4];
      car1_pos[1] = - CAR_HEIGHT;
    }
    
    PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
			RVALID = (PS2_data & 0x8000);	// extract the RVALID field
			if (RVALID != 0){
				/* always save the last three bytes received */
				byte1 = byte2;
				byte2 = byte3;
				byte3 = PS2_data & 0xFF;
				if (byte2 == (char)0xF0){
      			if (byte3 == (char)0x1C && pos > 1){
					pos_prev = pos;
        			pos -= 1;
      			}
      			else if (byte3 == (char)0x23 && pos < 4){
					pos_prev = pos;
        			pos += 1;
      			}
			}
				switch (pos_prev){
					case 1:
						draw_road(76, 170, 76 + CAR_WIDTH, 170 + CAR_HEIGHT);
						break;
					case 2:
						draw_road(120, 170, 120 + CAR_WIDTH, 170 + CAR_HEIGHT);
						break;
					case 3:
						draw_road(166, 170, 166 + CAR_WIDTH, 170 + CAR_HEIGHT);
						break;
					default:
						draw_road(210, 170, 210 + CAR_WIDTH, 170 + CAR_HEIGHT);
						break;
				}
				
				switch (pos){
					case 1:
						draw_object(police, 76, 170, CAR_WIDTH, CAR_HEIGHT);
                        player_position[0] = 76;
						break;
					case 2:
						draw_object(police, 120, 170, CAR_WIDTH, CAR_HEIGHT);
                        player_position[0] = 120;
						break;
					case 3:
						draw_object(police, 166, 170, CAR_WIDTH, CAR_HEIGHT);
                        player_position[0] = 166;
						break;
					default:
						draw_object(police, 210, 170, CAR_WIDTH, CAR_HEIGHT);
                        player_position[0] = 210;
						break;
				}
                
                
    		}
        wait_for_vsync();  // swap front and back buffers on VGA vertical sync
      pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // new back buffer
        draw_sides(i, pixel_ctrl_ptr);
        i += 1;
        if (i >= SCREEN_HEIGHT)
        {
            i = 0;
        }        
    }
}

void draw_object(short int array[], int offset_x, int offset_y, int width, int height){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
      		if  (j + offset_x >= 0 && j + offset_x < SCREEN_WIDTH && i + offset_y >= 0 && i + offset_y < SCREEN_HEIGHT){
        		if (array[j + (width*i)] != 0x0000){
          			plot_pixel(j + offset_x, i + offset_y, array[j + (width*i)]);
        		}
      		}
		}
	}
}

void wait_for_vsync() {
  volatile int *pixel_ctrl_ptr = (int *)0xff203020;  // base address
  int status;
  *pixel_ctrl_ptr = 1;  // start the synchronization process
  // write 1 into front buffer address register
  status = *(pixel_ctrl_ptr + 3);  // read the status register
  while ((status & 0x01) != 0){
    // polling loop waiting for S bit to go to 0
    status = *(pixel_ctrl_ptr + 3);
  }
}

void set_colour(int colour){
  border = 0x0040;
  drop_shadow = 0x62c8;
  if (colour == 1){
    base = 0xff9d;
    shadow = 0xbd16;
    window = 0x6ac7;
    window_shadow = 0x3202;
    headlight = 0xfd60;
    taillight = 0xaaa0;
  }
  else if (colour == 2){
    base = 0xfcb0;
    shadow = 0xc2ce;
    window = 0x71a3;
    window_shadow = 0x50c1;
    headlight = 0xfeef;
    taillight = 0xfcb0;
  }
  else if (colour == 3){
    base = 0xcd9e;
    shadow = 0x7bda;
    window = 0x420c;
    window_shadow = 0x2928;
    headlight = 0xfeef;
    taillight = 0xfcb0;
  }
  else if (colour == 4){
    base = 0xfe14;
    shadow = 0xac73;
    window = 0x728d;
    window_shadow = 0x618c;
    headlight = 0xffff;
    taillight = 0xb2cb;
  }
  else if (colour == 5){
    base = 0x4d32;
    shadow = 0x338e;
    window = 0x21e6;
    window_shadow = 0x1124;
    headlight = 0xfeef;
    taillight = 0xfcb0;
  } 
  else if (colour == 6)
  {
    base = 0x8c0f;
    shadow = 0x8c0f;
    window = 0x8c0f;
    window_shadow = 0x8c0f;
    headlight = 0x8c0f;
    taillight = 0x8c0f;

  }
  
}

void draw_car(int offset_x, int offset_y, int colour){
  	set_colour(colour);

	for (int i = 0; i < CAR_HEIGHT; i++){
		for (int j = 0; j < CAR_WIDTH; j++){
      		if  (j + offset_x >= 0 && j + offset_x < SCREEN_WIDTH && i + offset_y >= 0 && i + offset_y < SCREEN_HEIGHT){
        		switch (car[j + (CAR_WIDTH*i)]){
          			case 1:
            			plot_pixel(j + offset_x, i + offset_y, border);
            			break;
         			case 2:
            			plot_pixel(j + offset_x, i + offset_y, base);
            			break;
          			case 3:
            			plot_pixel(j + offset_x, i + offset_y, shadow);
            			break;
          			case 4:
            			plot_pixel(j + offset_x, i + offset_y, window);
            			break;
          			case 5:
            			plot_pixel(j + offset_x, i + offset_y, window_shadow);
            			break;
          			case 6:
            			plot_pixel(j + offset_x, i + offset_y, headlight);
            			break;
          			case 7:
            			plot_pixel(j + offset_x, i + offset_y, taillight);
            			break;
          			case 8:
            			plot_pixel(j + offset_x, i + offset_y, drop_shadow);
           				break;
          			default:
            			break;
        		}
      		}
		}
	}
}

void draw_road(int start_w, int start_h, int end_w, int end_h){
	for (int i = start_w; i < end_w; i++){
		for (int j = start_h; j < end_h; j++){
    		if  (i >= 0 && i < SCREEN_WIDTH && j >= 0 && j < SCREEN_HEIGHT){
        		if (i <= 72 || i > 248){
          			plot_pixel(i, j, 0xbd4c);
        		}
        		else if (i == 73 || i == 248){
          			plot_pixel(i, j, 0x5a89);
        		}
        		else if (i == 158 || i == 164){
          			plot_pixel(i, j, 0xf560);
        		}
        		else if (i > 158 && i < 164){
          			plot_pixel(i, j, 0xedef);
        		}
        		else {
          			plot_pixel(i, j, 0x8c0f);
        		}
      		}
		}
	}
}

void plot_pixel(int x, int y, short int line_color){
    volatile short int *one_pixel_address;
	
	one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
	
	*one_pixel_address = line_color;
}

void clear_screen(){
        for (int x = 0; x < 320; x++){
                for (int y = 0; y < 240; y++){
                        plot_pixel (x, y, 0);
				}
		}
}

void clear_car(int offset_x, int offset_y){
	for (int i = 0; i < CAR_HEIGHT; i++){
		for (int j = 0; j < CAR_WIDTH; j++){
      		if  (j + offset_x >= 0 && j + offset_x < SCREEN_WIDTH && i + offset_y >= 0 && i + offset_y < SCREEN_HEIGHT){
        		switch (car[j + (CAR_WIDTH*i)]){
          			case 1:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
         			case 2:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 3:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 4:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 5:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 6:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 7:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 8:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
           				break;
          			default:
            			break;
        		}
      		}
		}
	}
}

void HexDisplay(int score){
  volatile int * HEX3_HEX0_ptr = (int *)0xFF200020;
  unsigned char hex_segs[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
  int tens_digit = score / 10;
  int ones_digit = score % 10;
  int display = hex_segs[tens_digit] << 8 | hex_segs[ones_digit];
  *(HEX3_HEX0_ptr) = display;
}

void draw_car_flip(int offset_x, int offset_y, int colour){
  	set_colour(colour);

	for (int i = 0; i < CAR_HEIGHT; i++){
		for (int j = 0; j < CAR_WIDTH; j++){
      		if  (j + offset_x >= 0 && j + offset_x < SCREEN_WIDTH && i + offset_y >= 0 && i + offset_y < SCREEN_HEIGHT){
        		switch (car[1979 - j - (CAR_WIDTH*i)]){
          			case 1:
            			plot_pixel(j + offset_x, i + offset_y, border);
            			break;
         			case 2:
            			plot_pixel(j + offset_x, i + offset_y, base);
            			break;
          			case 3:
            			plot_pixel(j + offset_x, i + offset_y, shadow);
            			break;
          			case 4:
            			plot_pixel(j + offset_x, i + offset_y, window);
            			break;
          			case 5:
            			plot_pixel(j + offset_x, i + offset_y, window_shadow);
            			break;
          			case 6:
            			plot_pixel(j + offset_x, i + offset_y, headlight);
            			break;
          			case 7:
            			plot_pixel(j + offset_x, i + offset_y, taillight);
            			break;
          			case 8:
            			plot_pixel(j + offset_x, i + offset_y, drop_shadow);
           				break;
          			default:
            			break;
        		}
      		}
		}
	}
}

void clear_car_flip(int offset_x, int offset_y){

	for (int i = 0; i < CAR_HEIGHT; i++){
		for (int j = 0; j < CAR_WIDTH; j++){
      		if  (j + offset_x >= 0 && j + offset_x < SCREEN_WIDTH && i + offset_y >= 0 && i + offset_y < SCREEN_HEIGHT){
        		switch (car[1979 - j - (CAR_WIDTH*i)]){
          			case 1:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
         			case 2:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 3:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 4:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 5:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 6:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 7:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
            			break;
          			case 8:
            			plot_pixel(j + offset_x, i + offset_y, 0x8c0f);
           				break;
          			default:
            			break;
        		}
      		}
		}
	}
}

void animate_lines(int t){
	int pos1 = 114;
	int pos2 = 204;
	int offset = 24;
	draw_road(pos1, 0, pos1 + 4, 240);
	draw_line(pos1, t);
	draw_line(pos1, t + offset);
	draw_line(pos1, t + offset*2);
	draw_line(pos1, t + offset*3);
	draw_line(pos1, t + offset*4);
	draw_line(pos1, t + offset*5);
	draw_line(pos1, t + offset*6);
	draw_line(pos1, t + offset*7);
	draw_line(pos1, t + offset*8);
	draw_line(pos1, t + offset*9);
	
	draw_road(pos2, 0, pos2 + 4, 240);
	draw_line(pos2, t);
	draw_line(pos2, t + offset);
	draw_line(pos2, t + offset*2);
	draw_line(pos2, t + offset*3);
	draw_line(pos2, t + offset*4);
	draw_line(pos2, t + offset*5);
	draw_line(pos2, t + offset*6);
	draw_line(pos2, t + offset*7);
	draw_line(pos2, t + offset*8);
	draw_line(pos2, t + offset*9);
}

void draw_line(int pos_x, int pos_y){
	for (int i = pos_x; i < pos_x + 4; i++){
		for (int j = pos_y; j < pos_y + 12; j++){
			plot_pixel(i, j%SCREEN_HEIGHT, 0xFFFF);
		}
	}
}

void draw_sides(int t, volatile int* pixel_ctrl_ptr){
	//draw_road(0, 0, 73, 240);
	//draw_road(249, 0, 321, 240);
	//wait_for_vsync();  
    //pixel_buffer_start = *(pixel_ctrl_ptr + 1);
	
	animate_object(tree, 10, 50, TREE_WIDTH, TREE_HEIGHT, t);
	//animate_object(tree, 15, 160, TREE_WIDTH, TREE_HEIGHT, t);
	//animate_object(tree, 255, 120, TREE_WIDTH, TREE_HEIGHT, t);
	//animate_object(tree, 260, 10, TREE_WIDTH, TREE_HEIGHT, t);
	animate_object(leaf, 50, 30, LEAF_WIDTH, LEAF_HEIGHT, t);
	animate_object(grass, 20, 150, GRASS_WIDTH, GRASS_HEIGHT, t);
	
	wait_for_vsync();  
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
}

void animate_object(short int array[], int pos_x, int pos_y, int width, int height, int t){
	// draw_road(pos_x, (pos_y + t)%SCREEN_HEIGHT - 1, pos_x + width, (pos_y + t)%SCREEN_HEIGHT + height - 1);
	if ((pos_y + t)%SCREEN_HEIGHT + height - 1 > SCREEN_HEIGHT){
		draw_road(pos_x, (pos_y + t)%SCREEN_HEIGHT - 1, pos_x + width, SCREEN_HEIGHT + height);
		draw_road(pos_x, 0, pos_x + width, (pos_y + t + height)%SCREEN_HEIGHT - 1);
	}
	else{
		draw_road(pos_x, (pos_y + t)%SCREEN_HEIGHT - 1, pos_x + width, (pos_y + t)%SCREEN_HEIGHT + height - 1);
	}
	draw_object(array, pos_x, (pos_y + t)%SCREEN_HEIGHT, width, height);
}