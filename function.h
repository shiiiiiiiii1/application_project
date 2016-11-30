// for mode change   ---------------------------------------------------------------
void rotation() {
  int val_counterclockwise = digitalRead(encoder_counterclockwise_pin);
  int val_clockwise = digitalRead(encoder_clockwise_pin);
  int rotation_min = default_rotation;   // 1
  int rotation_max = 4;   // モード数

  if(val_counterclockwise == HIGH && val_clockwise == HIGH){   // 回してない時
    if(direction_rotation == 1){   // 一回前に右回りしてたら
      current_rotation += 1;
      if(current_rotation > rotation_max){
        current_rotation = rotation_max;
      }
    }
    if(direction_rotation == -1){   // 一回前に左回りしてたら
      current_rotation -= 1;
      if(current_rotation < rotation_min){
        current_rotation = rotation_min;
      }
    }
    direction_rotation = 0;   // direction_rotationの値に動かしてないことを保管
  } else {   // どっちかに回転したら
    if(val_counterclockwise == LOW){   // 右回りしたら
      direction_rotation = 1;   // direction_rotationの値に右回りしたことを保管
    }
    if(val_clockwise == HIGH){   // 左回りしたら
      direction_rotation = -1;   // direction_rotationの値に左回りしたことを保管
    }
  }
}
void mode_color_change(int current_rotation) {
  switch(current_rotation){
    case 0:   // 色なしの色
      analogWrite(cyan_pin, 255);
      analogWrite(magenta_pin, 255);
      analogWrite(yellow_pin, 255);
      break;
    case 1: // mode normal
      analogWrite(cyan_pin, 0);
      analogWrite(magenta_pin, 255);
      analogWrite(yellow_pin, 255);
      break;
    case 2: // mode sugoroku
      analogWrite(cyan_pin, 255);
      analogWrite(magenta_pin, 0);
      analogWrite(yellow_pin, 255);
      break;
    case 3: // mode art
      analogWrite(cyan_pin, 0);
      analogWrite(magenta_pin, 0);
      analogWrite(yellow_pin, 255);
      break;
    case 4: // mode bomb
      analogWrite(cyan_pin, 255);
      analogWrite(magenta_pin, 255);
      analogWrite(yellow_pin, 0);
      break;
    case 5: //PINK
      analogWrite(cyan_pin, 0);
      analogWrite(magenta_pin, 255);
      analogWrite(yellow_pin, 0);
      break;
    case 6: //SKY BLUE
      analogWrite(cyan_pin, 255);
      analogWrite(magenta_pin, 0);
      analogWrite(yellow_pin, 0);
      break;
    case 7: //WHITE
      analogWrite(cyan_pin, 0);
      analogWrite(magenta_pin, 0);
      analogWrite(yellow_pin, 0);
      break;
  }
}

// for all mode   ---------------------------------------------------------------
int acceleration_decision() {
  int z_old_val = analogRead(acceleration_z_analogpin);
  int z_difference = 0;
  int z_sum = 0;
  for(int i=0; i<2; i++){
    delay(50);
    z = analogRead(acceleration_z_analogpin);
    z_difference = abs(z_old_val - z);
    z_sum += z_difference;
    z_old_val = z;
  }
  return z_sum;
}
void acceleration_read() {
  x = analogRead(acceleration_x_analogpin);
  y = analogRead(acceleration_y_analogpin);
  z = analogRead(acceleration_z_analogpin);
  x = x<10 ? 0 : x;
  x = x>1013 ? 1023 : x;
  y = y<10 ? 0 : y;
  y = y>1013 ? 1023 : y;
  z = z<10 ? 0 : z;
  z = z>1013 ? 1023 : z;
}
void change_ledcolor(int r, int g, int b) {
  for(int i=0; i<num_leds; i++){
    rgbled.setPixelColor(i, rgbled.Color(r, g, b));
    rgbled.show();
  }
}
void flashing() {
  delay(1000);
  digitalWrite(flash_pin, HIGH);
  delay(1000);
  digitalWrite(flash_pin, LOW);
}

// for normal mode   ---------------------------------------------------------------
void return_loop_normal() {
  while(1){
    delay(50);
    acceleration_read();
    if(x<low_acceleration || high_acceleration<x || y<low_acceleration || high_acceleration<y){
      change_ledcolor(255, 255, 255);
      delay(200);
      break;
    }
  }
}

// for art mode   ---------------------------------------------------------------
int change_rgb(int current_H, int conversion_H){   // delay制御するかどうかまだ変更するかも
  int R, G, B;
  int while_finish_val = current_H+conversion_H;
  while(current_H < while_finish_val){
    if(current_H <= H_lap/3) {
      R = map(current_H, 0, H_lap/3, 255, 0);
      G = map(current_H, 0, H_lap/3, 0, 255);
      B = 0;
    }
    if(H_lap/3 < current_H && current_H <= H_lap*2/3){
      G = map(current_H, H_lap/3, H_lap*2/3, 255, 0);
      B = map(current_H, H_lap/3, H_lap*2/3, 0, 255);
      R = 0;
    }
    if(H_lap*2/3 < current_H){
      B = map(current_H, H_lap*2/3, H_lap, 255, 0);
      R = map(current_H, H_lap*2/3, H_lap, 0, 255);
      G= 0;
    }
    change_ledcolor(R, G, B);
    current_H++;
  }
  return current_H;
}


// for sugoroku mode   ---------------------------------------------------------------
void score_led(int total_score) {
  delay(500);
  total_score = total_score<=num_leds ? total_score : total_score-num_leds;
  if(total_score < num_leds){
    for(int i=0; i<total_score; i++){
      rgbled.setPixelColor(i, rgbled.Color(255, 255, 255));
      rgbled.show();
    }
    for(int i=total_score; i<num_leds; i++){
      rgbled.setPixelColor(i, rgbled.Color(0, 0, 0));
      rgbled.show();
    }
  }
  if(total_score == num_leds){
    flashing();
  }
}
void return_loop_sugoroku(int total_score) {
  while(1){
    delay(50);
    acceleration_read();
    if(x<low_acceleration || high_acceleration<x || y<low_acceleration || high_acceleration<y){
      score_led(total_score);
      delay(200);
      break;
    }
  }
}

// for bomb mode   ---------------------------------------------------------------
void warning_change_color(int current_score) {
  if(current_score < 25){
    int R, G, B;
    R = map(current_score, 0, 24, 0, 255);
    G = map(current_score, 0, 24, 255, 0);
    B = 0;
    change_ledcolor(R, G, B);
  }
  if(25 <= current_score){
    change_ledcolor(255, 0, 0);
  }
}
int return_loop_bomb(int current_score) {
  int max_score = 30;
  while(1){
    delay(50);
    acceleration_read();
    if(x<low_acceleration || high_acceleration<x || y<low_acceleration || high_acceleration<y){
      warning_change_color(current_score);
      if(current_score >= max_score){
        flashing();
        current_score = 0;
      }
      delay(200);
      return current_score;
    }
  }
}