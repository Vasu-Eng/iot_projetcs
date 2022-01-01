
#include <Arduino.h>
#include <NewPing.h>
#include <Servo.h>
#include <RH_ASK.h>
#include <SPI.h> 

// In dev branch 

//defining the hardware pins
#define TRIGGER_PIN  4
#define ECHO_PIN     4
#define MAX_DISTANCE 400
#define led 12
#define button 10
#define buzzer 5                                                                                       
//defalut Rx pin 11 

//Crearting the objects
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;

//_____ variable 
float duration, distance;  
int pos=90, d = 20, counter=0,COUNT=1, angle_no,IR;
unsigned int feedback,Time;
float Reading[5],Record[5],IR_status[5],path;
int Angle[5] = {90,120,150,60,30};
//motor
int IN3 = 7;
int IN4 = 6;
int ENB = A5;
int Buzzer(int b);
void setup() {
  myservo.attach(2);
Serial.begin(9600);
pinMode(buzzer,OUTPUT);
pinMode(led,OUTPUT);
pinMode(IN3,OUTPUT);//motor
pinMode(IN4,OUTPUT);
pinMode(3,INPUT);// IR 
Buzzer(2);
}


//costom funtions 
int Forward(int val);
int Backward(int val);
int Stop();
int D_Reading();
int decision();
int Search();
int ServoActionControl();
int trap();
int Continue();


void loop() {
Serial.println( "<--------------------CHIKU in Starting and ready to run-------------------- -----> ");
Serial.println("------Serach()---------");
 Search();// search and store the UL sensor reading
Serial.println("---------Decision-----------");
 decision();//Main complex funtion which is reponsible for all the decision making
Serial.print("The desired distance : ");
Serial.println(path);
 ServoActionControl(); //mchanisum for movement of robo
 Stop();
 Continue();

 //this is complesory part of robo to make the arry reuseable for next alltration
   for(int x=0;x<5;x++){           
    Reading[x]=0;
    Record[x]=0;
    IR_status[x]=0; 
   }
   counter=0,feedback =0;
   Serial.println(" ");
   Serial.println("---------------------done-------------------"); 
   //feedback
   //if(COUNT%2 == 0)Time=0;//even
   COUNT++;
}
 

 // ------------------------------------------------------------------->FUNCTIONS DEFINATION<------------------------------------------

int Forward(int val){// 100,155,255
  // motor
   // 0 <= val <= 255
 
  analogWrite(ENB,val);
 digitalWrite(IN3,HIGH);
 digitalWrite(IN4,LOW); 
 return 0;  
}
int Backward(int val){
  analogWrite(ENB,val);
 digitalWrite(IN4,HIGH);
 digitalWrite(IN3,LOW);
 return 0;
}
int Stop(){
  
 analogWrite(ENB,LOW);
 digitalWrite(IN3,LOW);
 digitalWrite(IN4,LOW);  
  return 0;
}
int Buzzer(int b){
  if (b == 1){
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  delay(100);
 digitalWrite(buzzer,HIGH);
  delay(100);
 digitalWrite(buzzer,LOW);
  }
  if( b==2){
  digitalWrite(buzzer,HIGH);
  delay(300);
  digitalWrite(buzzer,LOW);
  delay(300);
 digitalWrite(buzzer,HIGH);
  delay(300);
 digitalWrite(buzzer,LOW);
    
  }
  return 0;
}
int  D_Reading(){
  //---->starting Ultra Sonic sensor 
  duration = sonar.ping_median(10); 
  distance = (duration / 2) * 0.0343;
   delay(5);// very nacessry to perform all the calculations
  // Send results to Serial Monitor
  //Serial.print("Distance = ");
  if (distance >= 400 ){
    distance = 400;
    }
 else if(distance <= 2) {
    distance = 2;
  }
 IR = digitalRead(3);
Serial.print("counter variable  :  ULT sensor  :  IR sensor ");
Serial.print(" ------ ");
Serial.print(counter);
Serial.print("  :  ");
Serial.print(distance);
Serial.print("  :  ");
Reading[counter]= distance;
Record[counter] = distance;
IR_status[counter] = IR;
Serial.println(IR);
counter++;
return 0;
 }
 


int trap(){
  Serial.println("CHIKU IS TRAPPED !!!");
  digitalWrite(led,HIGH);
  Buzzer(1);
  myservo.write(30);
  delay(5);
  Backward(255);
  delay(1000);
  Stop();
  for(int i=180;i>=90;i--){ 
  myservo.write(i);
  delay(25);
  Forward(255);
  }
  Stop(); 
  digitalWrite(led,LOW);
  return 0;
}

 int Search(){
 //Feedback
if(COUNT%2 != 0){//odd
  Time=millis();
}
else if(COUNT%2 == 0){//even
  feedback = millis() - Time;
  Serial.print("feedback Time : ");
  Serial.println(feedback);
}
if(feedback == 0 && COUNT >= 2)trap();
 
// code for Search the maximum distance 
    for(pos=90 ;pos<=150;pos+=30){
     myservo.write(pos);
     Serial.print("Search angle = ");
     Serial.print(pos);
     Serial.print(" / ");
     delay(20);
     D_Reading();// take rading and store it into the Record[] and Reading[] array and also incerese the counter variable by one in every attribute 
      //delay(200);
  }
    pos=60;
 for(pos=60;pos>=30;pos-=30){
    myservo.write(pos);
    Serial.print("Search angle =  ");
     Serial.print(pos);
     Serial.print(" / ");
     delay(20);
    D_Reading();
      delay(200);
     }
  pos = 90;
  myservo.write(pos); 
  delay(20);
  return 0;
 }

 

int ServoActionControl(){
//MATHAMATICAL PART
//INPUT -it takes the value from decision funtion ( which is stored in path variale) which is basically the derierd UL sensor dstance
  //action - find the corresponding angle of servo( a which is the cosserspoding  element no( angle_no)  of the Angle[] array  ) using RECORD array 
  //OUTPUT - It will give the the desired value of angle_no variable
  for(int i=0;i<5;i++){
  if(path == Record[i])angle_no = i; 
  }
  Serial.print(" Corresponding angle  :");
  Serial.println(Angle[angle_no]);

 //This part move the servo to the Corresponding angle
 Serial.println("---------Action--------");
if( Angle[angle_no] > 90){
  Serial.print("Left-Turn/");
  Serial.println(Angle[angle_no]);
  for( pos = Angle[angle_no];pos>=90;pos--){
  Forward(255);
  myservo.write(pos);          
  duration = sonar.ping_median(2); 
  distance = (duration / 2) * 0.0343;
   delay(2);// very nacessry to perform all the calculations
  // Send results to Serial Monitor
  //Serial.print("Distance = ");
 if (distance >= 400 )distance = 400;
 else if(distance <= 2)distance = 2;
//Serial.println(distance);
if( distance <= 10){
  Stop();
  pos=90;
  myservo.write(pos);
  return 0;
   }
        }
   }
else if( Angle[angle_no] < 90){
  Serial.print("Right-Turn");
  Serial.println(Angle[angle_no]);
   for( pos = Angle[angle_no];pos<=90;pos++){
  Forward(255);
  myservo.write(pos);             
    duration = sonar.ping_median(2); 
  distance = (duration / 2) * 0.0343;
   delay(2);// very nacessry to perform all the calculations
  // Send results to Serial Monitor
  //Serial.print("Distance = ");
  if (distance >= 400 ){
    distance = 400;
    }
 else if(distance <= 2) {
    distance = 2;
  }
//Serial.println(distance);
  if ( distance < 20.00){
    Stop();
    pos = 90;
    myservo.write(pos);
    break;
  }
        }
       
   } 
if(Angle[angle_no]==90){
  Continue();
}
   return 0;  
}

int Continue(){
  Serial.print("Forward/ULS-Read");
  digitalWrite(led,HIGH);
 for(int i=0;;i++){
 //IR_val = digitalRead(3);
  duration = sonar.ping_median(5); 
  distance = (duration / 2) * 0.0343;
   delay(5);// very nacessry to perform all the calculations
  // Send results to Serial Monitor
  //Serial.print("Distance = ");
//Serial.println(distance);
 
 if(distance <= 15.00 ){
  Stop();
  break;
 }
 Forward(255);
 }
Stop();
digitalWrite(led,LOW);
return 0;
}
  

int decision(){
   int val_IR;
 if( IR_status[0]== 0 && IR_status[1]== 0 && IR_status[2]== 0 && IR_status[3]== 0 && IR_status[4]== 0){
  myservo.write(90);
  Serial.print("Backward/IR");
  for( ;;){
    Backward(200);
    val_IR = digitalRead(3);
    if( val_IR == 1)break;
          }
         Stop();
     return 0;
       }
  // --------- if One/no object is detected-------
  //case : 0  --( no object is detected )
 else if( IR_status[0]== 1 && IR_status[1]== 1 && IR_status[2]== 1 && IR_status[3]== 1 && IR_status[4]== 1 ){
  
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
    }
  }
    path = Reading[0];
   return 0;
  }

  //case :1
 else if( IR_status[0]== 0 && IR_status[1]== 1 && IR_status[2]== 1 && IR_status[3]== 1 && IR_status[4]== 1 ){
  Reading[0] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
   return 0;
  } 

  //case : 2
  else if( IR_status[0]== 1 && IR_status[1]== 0 && IR_status[2]== 1 && IR_status[3]== 1 && IR_status[4]== 1 ){
  Reading[1] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
   path = Reading[0]; 
  return 0;
  }

  //case : 3
 else if( IR_status[0]== 1 && IR_status[1]== 1 && IR_status[2]== 0 && IR_status[3]== 1 && IR_status[4]== 1 ){
  Reading[2] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
  return 0;
  }
  //case : 4
  if( IR_status[0]== 1 && IR_status[1]== 1 && IR_status[2]== 1 && IR_status[3]== 0 && IR_status[4]== 1 ){
  Reading[3] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
   path = Reading[0]; 
  return 0;
  }
  //case  : 5
  else if( IR_status[0]== 1 && IR_status[1]== 1 && IR_status[2]== 1 && IR_status[3]== 1 && IR_status[4]== 0 ){
  Reading[4] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
    return 0;
  }

  //-----if two tied object  is detected--- 
  //case : 1
  else if( IR_status[0]== 0 && IR_status[1]== 0 && IR_status[2]== 1 && IR_status[3]== 1 && IR_status[4]== 1 ){
  Reading[0] = 0;
  Reading[1] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
   return 0;
  }
  //case :2
 else  if( IR_status[0]== 1 && IR_status[1]== 0 && IR_status[2]== 0 && IR_status[3]== 1 && IR_status[4]== 1 ){
  Reading[1] = 0;
  Reading[2] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
    return 0;
  }

  //case : 3
 else  if( IR_status[0]== 1 && IR_status[1]== 1 && IR_status[2]== 0 && IR_status[3]== 0 && IR_status[4]== 1 ){
  Reading[2] = 0;
  Reading[3] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
    return 0;
  }
  //case : 4
 else  if( IR_status[0]== 1 && IR_status[1]== 1 && IR_status[2]== 1 && IR_status[3]== 0 && IR_status[4]== 0 ){
  Reading[3] = 0;
  Reading[4] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
   return 0;
  }

  //-------if two distinct object is detected----
  //case : 1
 else  if( IR_status[0]== 0 && IR_status[1]== 1 && IR_status[2]== 0 && IR_status[3]== 1 && IR_status[4]== 1 ){
  Reading[0] = 0;
  Reading[2] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
  return 0;
  }
  //case: 2
 else  if( IR_status[0]== 0 && IR_status[1]== 1 && IR_status[2]== 1 && IR_status[3]== 0 && IR_status[4]== 1 ){
  Reading[0] = 0;
  Reading[3] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
  return 0;
  }
  //case : 3
 else  if( IR_status[0]== 0 && IR_status[1]== 1 && IR_status[2]== 1 && IR_status[3]== 1 && IR_status[4]== 0 ){
  Reading[0] = 0;
  Reading[4] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0];
  return 0;
  }
  //case : 4 
  else if( IR_status[0]== 1 && IR_status[1]== 0 && IR_status[2]== 1 && IR_status[3]== 0 && IR_status[4]== 1 ){
  Reading[0] = 0;
  Reading[3] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
   return 0;
  }
  //case :5
  else if( IR_status[0]== 1 && IR_status[1]== 0 && IR_status[2]== 1 && IR_status[3]== 1 && IR_status[4]== 0 ){
  Reading[1] = 0;
  Reading[4] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
  return 0;
  }
  //case : 6
 else  if( IR_status[0]== 1 && IR_status[1]== 0 && IR_status[2]== 0 && IR_status[3]== 1 && IR_status[4]== 0 ){
  Reading[2] = 0;
  Reading[4] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
    return 0;
 }

 //------- if three object is drtected---
 // CASE 1 : two tied and one distinct
    //case 1
  else if( IR_status[0]== 0 && IR_status[1]== 0 && IR_status[2]== 1 && IR_status[3]== 1 && IR_status[4]== 0 ){
  Reading[0] = 0;
  Reading[1] = 0;
  Reading[4] = 0;
  
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
    return 0;
  }
  //case : 2
  else  if( IR_status[0]== 1 && IR_status[1]== 0 && IR_status[2]== 0 && IR_status[3]== 1 && IR_status[4]== 0 ){
  Reading[1] = 0;
  Reading[2] = 0;
  Reading[4] = 0;
  
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
  return 0;
  }
  //case : 3
   else if( IR_status[0]== 0 && IR_status[1]== 1&& IR_status[2]== 0 && IR_status[3]== 0 && IR_status[4]== 0 ){
  Reading[0] = 0;
  Reading[2] = 0;
  Reading[3] = 0;
  
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0]; 
   return 0;
  }
  //case : 4
  else  if( IR_status[0]== 0 && IR_status[1]== 1  && IR_status[2]== 1 && IR_status[3]== 0 && IR_status[4]== 0 ){
  Reading[0] = 0;
  Reading[3] = 0;
  Reading[4] = 0;
  for(int i=0;i<5;i++){
    if(Reading[0]<Reading[i]){
      Reading[0] = Reading[i];
            }
     }
    path = Reading[0];
   return 0;
  }
  //CASE 2 : all three are distinct 
  //
  //case 1
  else  if( IR_status[0]== 0 && IR_status[1]== 1 && IR_status[2]== 0 && IR_status[3]== 1 && IR_status[4]== 0 ){
       path = Reading[1]; 
      return 0;
  }
  //CASE 3 : all three are tied 
  //case : 1 
   if( IR_status[0]== 0 && IR_status[1]== 0 && IR_status[2]== 0 && IR_status[3]== 1 && IR_status[4]== 1){
    path = Reading[4];
    return 0;
  }
  //case : 2
 else  if( IR_status[0]== 1 && IR_status[1]== 1 && IR_status[2]== 0 && IR_status[3]== 0 && IR_status[4]== 0 ){
  
    path = Reading[0]; 
   return 0;
  }
  //case : 3
  else if( IR_status[0]== 1 && IR_status[1]== 1 && IR_status[2]== 0 && IR_status[3]== 0 && IR_status[4]== 0 ){
  
    path = Reading[0]; 
    return 0;
  }

  //------ if 4 object is detected 
  //case : 1
  else if( IR_status[0]== 1 && IR_status[1]== 0 && IR_status[2]== 0 && IR_status[3]== 0 && IR_status[4]== 0 ){
    path = Reading[0]; 
    return 0;
  }
  //case : 2
 else  if( IR_status[0]== 0 && IR_status[1]== 1 && IR_status[2]== 0 && IR_status[3]== 0 && IR_status[4]== 0 ){
    path = Reading[1]; 
   return 0;
  }
  //case : 3
else  if( IR_status[0]== 0 && IR_status[1]== 0 && IR_status[2]== 1 && IR_status[3]== 0 && IR_status[4]== 0 ){
    path = Reading[2];
   return 0;
  }
  //case : 4
  else if( IR_status[0]== 0 && IR_status[1]== 0 && IR_status[2]== 0 && IR_status[3]== 1 && IR_status[4]== 0 ){
    path = Reading[3]; 
   return 0;
  }
  //case: 5
  else if( IR_status[0]== 0 && IR_status[1]== 0 && IR_status[2]== 0 && IR_status[3]== 0 && IR_status[4]== 1 ){
    path = Reading[4]; 
    return 0;
  }
  
  else {
    trap();
    return 0;
  }  
}

     //-- --------END-----------

 
  
