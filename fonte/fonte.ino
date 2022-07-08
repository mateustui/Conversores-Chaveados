#include <LiquidCrystal.h>
LiquidCrystal lcd(3, 2, 4, 5, 6, 7);
void setup()
{
  DDRB = 0xFF; //Pinos 9 (OC1A) e 10 (OC1B)
  TCCR1A = 0b10100010; //PWM não invertido nos pinos OC1A e OC1B
  TCCR1B = 0b00011001; //liga TC1, prescaler = 1
  ICR1 = 320; //valor máximo para contagem
  OCR1A = 0; //controle do ciclo ativo do PWM 0C1A
  OCR1B = 128  ;
  Serial.begin(115200);
  lcd.begin(16, 2);
}
int tensao = 0;
int corrente = 0;
float med_tens;
int i = 0;
float Ki=0.5;
float Kp=8.0;
float Sp=13.8;
float erro=0.0;
int acao=0.0;
float erro_int=0.0;
void loop() {


  //fast PWM, TOP = ICR1, OC1A e OC1B habilitados



  corrente = analogRead(A0);
  while (i < 64)
  {
    tensao = analogRead(A1);
    tensao = (tensao * 30.60) / (1024.0);
    med_tens += tensao;
    i++;
    delay(10);
  }
  med_tens /= 64.0;
  erro=Sp-med_tens;
  acao=140+erro*Kp+erro_int*Ki;
  erro_int+=erro;
  if(erro_int>100)
  {
    erro_int=100;
  }
  else if(erro_int<-100)
  {
    erro_int=-100;
  }
  if(acao>144)
  {
    acao=144;
  }
  else if(acao<125)
  {
    acao=125;
  }
  OCR1B=acao;
  lcd.setCursor(0, 0);
  lcd.print("Conv. chaveados");
  lcd.setCursor(0, 1);
  lcd.print(med_tens);
  lcd.setCursor(5, 1);
  lcd.print(" tensao");
  Serial.print("erro: ");
  Serial.print(erro);
  Serial.print(" acao: ");
  Serial.println(acao);
  delay(100);
  i = 0;
  med_tens = 0;
  /*_delay_us(500000);
    OCR1A++;
    if (OCR1A >= ICR1)
    {
    lcd.clear();
    OCR1A = 0;
    }
  */

}
