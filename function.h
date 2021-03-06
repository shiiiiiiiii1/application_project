// for mode change   ---------------------------------------------------------------
void rotation() {
  bool val_counterclockwise = digitalRead(ENCODER_COUNTERCLOCKWISE_PIN);
  bool val_clockwise = digitalRead(ENCODER_CLOCKWISE_PIN);
  int rotation_min = DEFAULT_ROTATION;   // 1
  int rotation_max = 4;   // モード数

  if(val_counterclockwise == 0 && val_clockwise == 1){   // 回してない時
    current_rotation += 1;
    Serial.println("right");
    if(current_rotation > rotation_max){
      current_rotation = rotation_max;
    }
    delay(100);
  }
  if(val_counterclockwise == 1 && val_clockwise == 0){   // どっちかに回転したら
    current_rotation -= 1;
    Serial.println("left");
    if(current_rotation < rotation_min){
      current_rotation = rotation_min;
    }
    delay(100);
  }
}
void mode_color_change(int current_rotation) {
  switch(current_rotation){
    case 0:   // 色なしの色
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
      break;
    case 1: // mode normal
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
      break;
    case 2: // mode sugoroku
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
      break;
    case 3: // mode art
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
      break;
    case 4: // mode bomb
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
      break;
  }
}

// for all mode   ---------------------------------------------------------------
int acceleration_decision() {
  int z_old_val = analogRead(ACCELERATION_Z_ANALOGPIN);
  int z_difference = 0;
  int z_sum = 0;
  for(int i=0; i<2; i++){
    delay(50);
    z = analogRead(ACCELERATION_Z_ANALOGPIN);
    z_difference = abs(z_old_val - z);
    z_sum += z_difference;
    z_old_val = z;
  }
  return z_sum;
}
void acceleration_read() {
  x = analogRead(ACCELERATION_X_ANALOGPIN);
  y = analogRead(ACCELERATION_Y_ANALOGPIN);
  z = analogRead(ACCELERATION_Z_ANALOGPIN);
  x = x<10 ? 0 : x;
  x = x>1013 ? 1023 : x;
  y = y<10 ? 0 : y;
  y = y>1013 ? 1023 : y;
  z = z<10 ? 0 : z;
  z = z>1013 ? 1023 : z;
}
void change_ledcolor(int r, int g, int b) {
  for(int i=0; i<NUM_LEDS; i++){
    rgbled.setPixelColor(i, rgbled.Color(r, g, b));
    rgbled.show();
  }
}
void flashing() {
  delay(1000);
  digitalWrite(FLASH_PIN, HIGH);
  delay(1000);
  digitalWrite(FLASH_PIN, LOW);
}

// for normal mode   ---------------------------------------------------------------
void return_loop_normal() {
  while(1){
    delay(50);
    acceleration_read();
    if(x<LOW_ACCELERATION || HIGH_ACCELERATION<x || y<LOW_ACCELERATION || HIGH_ACCELERATION<y){
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

    if( digitalRead(ENCODER_SWITCH_PIN) ){
      break;
    }

    if(current_H <= H_LAP/3) {
      R = map(current_H, 0, H_LAP/3, 255, 0);
      G = map(current_H, 0, H_LAP/3, 0, 255);
      B = 0;
    }
    if(H_LAP/3 < current_H && current_H <= H_LAP*2/3){
      G = map(current_H, H_LAP/3, H_LAP*2/3, 255, 0);
      B = map(current_H, H_LAP/3, H_LAP*2/3, 0, 255);
      R = 0;
    }
    if(H_LAP*2/3 < current_H){
      B = map(current_H, H_LAP*2/3, H_LAP, 255, 0);
      R = map(current_H, H_LAP*2/3, H_LAP, 0, 255);
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
  total_score = total_score<=NUM_LEDS ? total_score : total_score-NUM_LEDS;
  if(total_score < NUM_LEDS){
    for(int i=0; i<total_score; i++){
      rgbled.setPixelColor(i, rgbled.Color(255, 255, 255));
      rgbled.show();
    }
    for(int i=total_score; i<NUM_LEDS; i++){
      rgbled.setPixelColor(i, rgbled.Color(0, 0, 0));
      rgbled.show();
    }
  }
  if(total_score == NUM_LEDS){
    flashing();
  }
}
void return_loop_sugoroku(int total_score) {
  while(1){
    delay(50);

    if( digitalRead(ENCODER_SWITCH_PIN) ){
      break;
    }

    acceleration_read();
    if(x<LOW_ACCELERATION || HIGH_ACCELERATION<x || y<LOW_ACCELERATION || HIGH_ACCELERATION<y){
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

    if( digitalRead(ENCODER_SWITCH_PIN) ){
      break;
    }

    acceleration_read();
    if(x<LOW_ACCELERATION || HIGH_ACCELERATION<x || y<LOW_ACCELERATION || HIGH_ACCELERATION<y){
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