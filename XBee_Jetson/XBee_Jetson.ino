int value = 0;
char string[16];

void setup() {
  // put your setup code here, to run once:
  
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
   sprintf(string, "{\"s\": %d}", value++);
   Serial1.print(string);

   delay(1000);

}
