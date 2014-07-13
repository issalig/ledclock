/*
reset 4017
*/
void reset_4017(){
  digitalWrite(resetPin4017,HIGH);
  delayMicroseconds(5);
  digitalWrite(resetPin4017,LOW);
}

/*
send pulse on clock pin
*/
void clock_4017(){
  digitalWrite(clockPin4017, HIGH);  // Move to next row and repeat for all eight rows
  //delayMicroseconds(5);
  digitalWrite(clockPin4017, LOW);
}

/*
set row performing or
*/
void set_or_row(byte row, byte val){
  byte x;
  for (x = 0; x < MATRIX_COLS; x++)
    if (val & (1<<x))
      raster[row][MATRIX_COLS-x-1]=1;
}


/*
fill a row
*/
void fill_row(int row, int val){
  int x;
  for (x = 0; x < MATRIX_COLS; x++)
    raster[row][x]=val;
}

/*
fill a column
*/
void fill_col(int col, int val){
  int y;
  for (y = 0; y < MATRIX_ROWS; y++)
    raster[y][col]=val;
}

/*
fill matrix
*/
void fill_matrix(int val){
  int x,y;
  for (y = 0; y < MATRIX_ROWS; y++) 
    for (x = 0; x < MATRIX_COLS; x++)
      raster[y][x]=val;
}

/*
transpose matrix
*/
void trans_matrix(){
  int x,y;
  char val;
  char raster2[MATRIX_ROWS][MATRIX_ROWS];
  for (y = 0; y < MATRIX_ROWS; y++) 
    for (x = y+1; x < MATRIX_ROWS; x++){
        val=raster[y][x];
        raster[y][x]=raster[x][y];
        raster[x][y]=val;            
    }
}

/*
square
*/
void fill_square(int radius, int val){
  int x,y;
  for (y = 0; y < MATRIX_ROWS; y++) 
    for (x = 0; x < MATRIX_COLS; x++)
      if ((y >= radius) && ( y < (MATRIX_COLS - radius)) &&
          (x >= radius) && ( x < (MATRIX_ROWS - radius)))
        raster[y][x]=val;
}

/* 
sqaure animation
*/
void square_animation(){
  for(int repeat=0;repeat<3;repeat++){
  for(int radius=0;radius<3;radius++){    
  fill_matrix(0);
    fill_square(radius, 1);

      draw_matrix_595(300);

  }
  for(int radius=2;radius>=0;radius--){    
  fill_matrix(1);
    fill_square(radius, 0);
      draw_matrix_595(300);

  }
  }
  fill_matrix(0);
}

/*
makes random matrix
*/
void random_matrix(){
  int x,y;
  
  for (y = 0; y < MATRIX_ROWS; y++) 
    for (x = 0; x < MATRIX_COLS; x++)
      raster[y][x]=random(0,2 );
  
}

/*
fill a column
*/
void set_led_number(int number){
  int x,y;
  
  if (number > 0) {
    y = number % MATRIX_COLS;
    x = number - y * MATRIX_COLS;
  }
  
  raster[y][x]=1;
}


/*
 draws matrix fora period of time
*/
void draw_matrix(int wait){
  int x,y;
  unsigned long time1;
  
  time1=millis();
  
  while (millis() < time1 + wait){
  
  //draw leds
  //pulse_clock();
  
  //rows
  for (y = 0; y < MATRIX_ROWS; y++) {
    //columns
    for (x = 0; x < MATRIX_COLS; x++)
      if (raster[y][x]) digitalWrite(colPins[x], HIGH);  // Light appropriate column LEDs

    delayMicroseconds(300);  // Wait a bit, or else they'll be too dim

    for (x = 0; x < MATRIX_COLS; x++)
      digitalWrite(colPins[x], LOW);  // Turn off all row

    clock_4017();
  }
  reset_4017();

  }
}

void shiftOut2(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, int val, uint8_t bits = 8, uint8_t del = 0)
{
  uint8_t i;
  for (i = 0; i < bits; i++)  {
    if (bitOrder == LSBFIRST)
      digitalWrite(dataPin, !!(val & (1 << i)));
    else    
      digitalWrite(dataPin, !!(val & (1 << ((bits - 1 - i)))));
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(del);
    digitalWrite(clockPin, LOW);            
  }
}

/*
 draws matrix for a period of time using 595 for columns
*/
void draw_matrix_595(int wait){
  int x,y;
  unsigned long time1;
  
  time1=millis();
  
  while (millis() < time1 + wait){
  
  //draw leds
  //pulse_clock();
  
  //rows
  for (y = 0; y < MATRIX_ROWS; y++) {
        
    //columns
    int row = 0;
    for (x = 0; x < MATRIX_COLS; x++)
       //if (raster[y][x]) bitSet(row,x);
       bitWrite(row, x, raster[y][x]);
       //row = row + (raster[y][x] << x);

    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin595, LOW);
    // shift out the bits:
    shiftOut(dataPin595, clockPin595, MSBFIRST, row);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin595, HIGH);

    delayMicroseconds(300);  // Wait a bit, or else they'll be too dim 300 ok

    for (x = 0; x < MATRIX_COLS; x++)
      digitalWrite(colPins[x], LOW);  // Turn off all row

    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin595, LOW);
    // shift out the bits:
    shiftOut(dataPin595, clockPin595, MSBFIRST, 0);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin595, HIGH);

    //clock and go to next row
    clock_4017();
  }
  //reset to go for first row
  reset_4017();

  }
}

/*
 draws matrix for a period of time
*/
void draw_matrix_7219(int wait){
  int x,y;
  unsigned long time1;
  
  lc.clearDisplay(0);
    
  //rows
  for (y = 0; y < MATRIX_ROWS; y++) {
    //columns
    for (x = 0; x < MATRIX_COLS; x++)
      if (raster[y][x]) {
          lc.setLed(0,y,x,true);
      }
  }

  time1=millis();  
  while (millis() < time1 + wait);

}

