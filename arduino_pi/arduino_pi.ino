#include "pi_arduino.h". 
//modified on march 13th. This is the final version. no new chnages to this project.
// woould be used as reference version for future development. Refer git README.md for more info
void setup() {
  // initialize digital pin led_pin as an output.
  pinMode(D4, OUTPUT);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer

// put your setup code here, to run once:
  Serial.begin(BAUD_RATE);

// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
 
  lcd.setRGB(colorR, colorG, colorB);
 
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("Welcome, PI!!!");
  lcd.setCursor(0,1);
  lcd.print("Configuring..!");
  delay(1000);
    
  lcd.clear();                
  lcd.home();
  Serial.write('M');
  /*do{
  Serial.write("This is test String to node red\n");
  delay(1000);
  }while(1);
  */   
 
  //Push Button and Buzzer configuration
  pinMode(button, INPUT); //set button as an INPUT device
  pinMode(buzzer, OUTPUT);   //set LED as an OUTPUT device

 //synchronize the PI and Arduino  
  do{
      lcd.setCursor(0,0);
      lcd.print("Sent A to PIOrg");  //sending init char 'A' to PI
      lcd.setCursor(0,1);
      lcd.print("Wait for P.");
      delay(1000);   
      Serial.write('A');
      
   }while(Serial.read()!='P');
   
   lcd.setCursor(0,0);
   lcd.print("Received P from PI");
   lcd.setCursor(0,1);
   lcd.print("Displaying Info!!!");
   delay(1000);

}

void loop() {
 
    btn = digitalRead(button); //read the status of the button
    digitalWrite(buzzer, btn);
    delay(1);

  //enable DATA_FROM_PI in pi_arduino.h
  #if DATA_FROM_PI  
  // put your main code here, to run repeatedly:
  if (Serial.available()){
   //the menu_char for each option from PI is received here.    
    menu_char=Serial.read(); 
  #if SERIAL_DEBUG_PRINTS
    Serial.print("Rxd data from PI=");
    Serial.print(menu_char);
    Serial.print("\n"); 
  #endif
   perform_action(menu_char,btn);  //the main action is performed here.
   
  }
  #endif
  
  #if RGB_LCD_TEST
    perform_action(LCD_DISPLAY);
    delay(100);
  #endif
      
  #if DISPLAY_TIME_IN_LOOP
  if (Serial.available())
    if (Serial.read()=='0'){
        display_time();
    }
  #endif
  
  #if DISPLAY_TEMP
     if (Serial.available())
      if (Serial.read()=='T')
        display_temperature(btn);
  #endif
  send_sensor_data_to_PI();
  delay(5000);
}

void led_display(char bit_state)
{
  lcd_display("Led Turned",bit_state?"ON":"OFF",2,6);
  if (bit_state){
      digitalWrite(D4, HIGH);   // turn the LED on (HIGH is the voltage level)
      led_next_state=LED_OFF;
  }
  else{
    digitalWrite(D4, LOW);    // turn the LED off by making the voltage LOW
    led_next_state=LED_ON;
  }
  

}

void lcd_display(char *line1str,char *line2str,int row1_col,int row2_col)
{
  lcd.clear();                
  lcd.home();
  lcd.setCursor(row1_col,ROW_0);
  lcd.print(line1str);
  lcd.setCursor(row2_col,ROW_1);
  lcd.print(line2str);
}


void display_temperature(int btn)
{
    
    int a = analogRead(pinTempSensor);
    float R = 1023.0/a-1.0;
    R = R0*R;
    float celsius = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
    float Farenheit= (celsius * (9/5))+32;

  if (btn){
    dtostrf(celsius,7,2,line2Buff);          
    sprintf (line2Buff,"%s %c",line2Buff,'C');
    
    lcd_display("Temperature  ",line2Buff,2,3);
    delay(1000);
  }
  else
  {
    dtostrf(Farenheit,7,2,line2Buff);          
    sprintf (line2Buff,"%s %c",line2Buff,'F');
    lcd_display("Temperature  ",line2Buff,2,3);
    delay(1000);
  }
}

void display_time(void)
{         
        int i=0;
        char *time_token[5];
        time_token[0]=(char *)malloc(3*sizeof(char));
        time_token[1]=(char *)malloc(3*sizeof(char));
        time_token[2]=(char *)malloc(2*sizeof(char));
        time_token[3]=(char *)malloc(8*sizeof(char));
        time_token[4]=(char *)malloc(4*sizeof(char));
        
          timeStr=Serial.readStringUntil('\n');
          
          delay(10);
          memset(line1Buff,0,LINE_BUFF_SIZE);
          memset(line2Buff,0,LINE_BUFF_SIZE);
          memset(time_token[0],0,(3*sizeof(char)));
          memset(time_token[1],0,(3*sizeof(char)));
          memset(time_token[2],0,(2*sizeof(char)));
          memset(time_token[3],0,(8*sizeof(char)));
          memset(time_token[4],0,(4*sizeof(char)));
          
          char* token=strtok((char *)timeStr.c_str(),(const char *)" ");        
          while (token!=NULL){
             strcpy(time_token[i++],token);    
             token=strtok(NULL,(const char *)" ");
           }
          
          sprintf(line1Buff,"%s %s %s %s ",time_token[0],time_token[1],time_token[2],time_token[4]);
          sprintf(line2Buff,"%s",time_token[3]);

          if ((\
              !strcmp("Sun",time_token[0])||\
              !strcmp("Mon",time_token[0])||\
              !strcmp("Tue",time_token[0])||\
              !strcmp("Wed",time_token[0])||\
              !strcmp("Thu",time_token[0])||\
              !strcmp("Fri",time_token[0])||\
              !strcmp("Sat",time_token[0]))\
                            &&\
              !strcmp("2018",time_token[4])||\
              !strcmp("2019",time_token[4])||\
              !strcmp("2020",time_token[4])||\
              !strcmp("2021",time_token[4])||\
              !strcmp("2022",time_token[4]))
        
          {
            lcd_display(line1Buff,line2Buff,0,3);
            delay(100);
          }
      
      for (i=0;i<5;i++)
          free(time_token[i]);
}

void sound_sensor(void)
{

    long sum = 0;
    for(int i=0; i<32; i++)
    {
        sum += analogRead(pinAdc);
    }

    sum >>= 5;
    sprintf(line1Buff,"%s","Sound Sensor");
    sprintf(line2Buff,"%ld",sum);
    lcd_display(line1Buff,line2Buff,2,6);
    delay(100);  
}

void ultrasonic_sensor(void)
{
  // Pass INC as a parameter to get the distance in inches
  int distance = ultrasonic.read();
  sprintf(line1Buff,"%s","Ultrasonic Sensor");
  sprintf(line2Buff,"%d %s",distance, "cms");
  lcd_display(line1Buff,line2Buff,0,4);
  delay(100);  
}

void light_sensor(void)
{
  long sum = 0;
  for(int i=0; i<32; i++)
     sum += analogRead(lightAdc);
   sum >>= 5;

   sprintf(line1Buff,"%s","Light Sensor");
   sprintf(line2Buff,"%ld",sum);
               
   lcd_display(line1Buff,line2Buff,2,6);
   delay(100);  
}

void LCD_Color_Change(int sec)
{
    
    static int counter=0;
    // Print a message to the LCD.
    lcd.setCursor(0,0);
    counter++;
    if (counter%sec==0)
      lcd.setRGB(colorR,0,0);
    if (counter%(2 * sec)==0)
      lcd.setRGB(0,colorG,0); 
    if (counter%(3* sec)==0)
      lcd.setRGB(0,0,colorB);  
    if (counter%(4 * sec)==0){
       counter=0;
       lcd.setRGB(colorR,colorG,colorB);  
    }
    delay(100);
}

void send_sensor_data_to_PI(void)
{
    int i=0;
    int light_sensor_data = 0,sound_sensor_data=0;
    
    int a = analogRead(pinTempSensor);
    float R = 1023.0/a-1.0;
    R = R0*R;
    float celsius = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
    float Farenheit= (celsius * (9/5))+32;

    dtostrf(celsius,7,2,line2Buff);
    dtostrf(Farenheit,7,2,line1Buff);
 
    lcd_display("Sending Data to","Rasberry pi...",0,0);
  
  //get sensor data
    for( i=0; i<32; i++)
       light_sensor_data += analogRead(lightAdc);
    

    light_sensor_data >>= 5;

    //get sound data
    for( i=0; i<32; i++)
       sound_sensor_data += analogRead(pinAdc);

    sound_sensor_data >>= 5;

    // Pass INC as a parameter to get the distance in inches
    int distance = ultrasonic.read();

    sprintf(serial_buffer,"%s F,%s C,%d L,%d dB,%d cms, %s \n",\
                          line1Buff,line2Buff,\
                          light_sensor_data,sound_sensor_data,distance,!led_next_state?"Led ON":"Led OFF");

    Serial.write(serial_buffer);
}

void perform_action(char menu_item,int btn)
{
  static int byteCount=0;
  
  switch (menu_item){
    
    case DISPLAY_TIME:
         display_time();
    break;

    case GET_TEMPARATURE:
        display_temperature(btn);
    break;

    case GET_SOUND_SENSOR_DATA:
      sound_sensor();
    break;
    
    case GET_LIGHT_SENSOR_DATA:
      light_sensor();
    break;

    case GET_ULTRASONIC_DATA:
      ultrasonic_sensor();
    break;

    case LED_DISPLAY:
      led_display(led_next_state);
    break;

    case SEND_SENSOR_DATA_TO_PI:
      send_sensor_data_to_PI();
    break;
   #if 0     
    //get text from PI to display
    case LCD_TEXT_DISPLAY:
        timeStr=Serial.readStringUntil('\n');
        delay(50);
    break;
    #endif
    
    default:
    
    break;
  }
}
