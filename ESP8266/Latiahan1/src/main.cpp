int led[] = {12, 13, 15};

int delR = 3000;
int delK = 1000;
int delH = 2000;

int bawah = 1000;
int atas = 3000;

void setup()
{
  Serial.begin(9600);

  for (int i = 0; i <= 3; i++)
  {
    pinMode(led[i], OUTPUT);
  }
}

void loop()
{

  // int i =

  digitalWrite(led[0], HIGH);
  delay(delR);
  digitalWrite(led[0], LOW);
  delay(200);

  digitalWrite(led[1], HIGH);
  delay(delK);
  digitalWrite(led[1], LOW);
  delay(200);

  digitalWrite(led[2], HIGH);
  delay(delH);
  digitalWrite(led[2], LOW);
  delay(200);
}
