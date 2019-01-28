//#include<msp430.h>
//#SoftwareSerial SIM800(4, 5);
#define gpsSerial Serial
int incomingByte = 0;
int i=58;
#define trigPin 8
#define echoPin 12
//#define OpnSIM 7
//#define OpnHC 6
#define RLY1 RED_LED  // P1.6 выход или светодиод желтый
int dum=0;

//float distance;
  /*int conversion;*/
  int duration;
  boolean on=true;
  String volt="p";
 
  
void setup() {
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  /*pinMode(LED, OUTPUT); 
  digitalWrite(LED, HIGH);*/
  pinMode(RLY1, OUTPUT);
  digitalWrite(RLY1, HIGH);
  delay(2000);
  //volt.reserve(200);
  gpsSerial.begin(9600);
    
}
int isOK() {
  if (incomingByte == 'O') {
    incomingByte = gpsSerial.read();
    if (incomingByte == 'K') {
      return 1;
    }
  }
  return 0;
}
void loop() {
   boolean ok=false;
   
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  //digitalWrite(P1_6, LOW);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //distance = duration*0.017;
  if(on)
  {
    on=false;
    
  do {
    do {
      //digitalWrite(P1_6, HIGH);
      gpsSerial.println("AT");
      //digitalWrite(P1_6, LOW);
      delay(500);
    } 
    while (!gpsSerial.available());
    incomingByte = gpsSerial.read();
    while(gpsSerial.available() && incomingByte == 255)
      incomingByte = gpsSerial.read();
    while(gpsSerial.available()) {
      if (isOK())
        ok = true;
      else
        incomingByte = gpsSerial.read();
    }
  } 
  while (!ok);
    connecting();
  }
  
 if (++i == 60){
  digitalWrite(RLY1, HIGH);
  delay(20);
  sendData();
  i=0;
 }
 else{
  delay(10000);
  }   
  delay(20);
 
}
void connecting()
{
  //digitalWrite(P1_6, HIGH);
delay(500);
  //Serial.println("AT");
  //mydelay();  delay(2000);
  gpsSerial.println("AT+IPR=9600");
  mydelay();
  gpsSerial.println("AT+GSMBUSY=1");//запрет входящих звонков
  delay(50);
  gpsSerial.println("ATE0");//Выключение режима эхо
  delay(50);/*
  gpsSerial.println("ATV0");//Формат ответа модуля - только ответ
  delay(50);*/
  gpsSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  mydelay();//*/delay(1000);
  gpsSerial.println("AT+SAPBR=3,1,\"APN\",\"internet.tele2.ru\"");
  mydelay();//delay(200);
  gpsSerial.println("AT+SAPBR=3,1,\"USER\",\"tele2\"");
  mydelay();//delay(1000);
  gpsSerial.println("AT+SAPBR=3,1,\"PWD\",\"tele2\"");
  mydelay();//delay(2000);
  gpsSerial.println("AT+SAPBR=1,1");
  digitalWrite(RLY1, LOW); 
  mydelay();//delay(1000);
  gpsSerial.println("AT+HTTPINIT");
  mydelay();//delay(2000);
  gpsSerial.println("AT+HTTPPARA=\"CID\",1");
  mydelay();
}


void sendData(){
  
  delay(1000);//было 500
  gpsSerial.print("AT+HTTPPARA=\"URL\",\azurewebsites.net/index?ID=");
  gpsSerial.print("9006001042");
  gpsSerial.print("&sp=");
  gpsSerial.print(duration);
  gpsSerial.print("&temp=");
  gpsSerial.println("24");
  while (!gpsSerial.available());
  
  delay(1000);
  
  gpsSerial.println("AT+HTTPACTION=1");
  digitalWrite(RLY1, LOW);
  while (!gpsSerial.available());
  delay(200);//было 20
 
}
void mydelay()
{
  while(!gpsSerial.available());
  delay(1000);
  while(gpsSerial.available()){dum=gpsSerial.read();}
}
/*  Обработчики прерываний  */
#pragma vector = TIMERA0_VECTOR  
__interrupt void CCR0_ISR(void) {
        if (++i == 12) {
                P1OUT ^= RLY1;
                i=0;
        }
} // CCR0_ISR

