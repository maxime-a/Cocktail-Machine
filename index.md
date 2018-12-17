## The Coktail Machine

This machine was designed and created with two class mate for my highschool final exam, moreover we have participated to engineering contest and we have won two prices.

<iframe width="560" height="315" src="https://www.youtube.com/embed/7zr9PUWKxgU" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
French version :
<iframe width="560" height="315" src="https://www.youtube.com/embed/ZRQ8ewBXXp0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Working principle

The Cocktail Machine is connected via wifi with your smartphone, with the android application you can enter what recipes you want for your cocktails they will be stored in a database.
To order a Cocktail you just have to hit the vocal recognition button and say the name of your cocktail. 
The application will search the name in the database and send to the machine the recipes. 
After, the machine pump the liquids from the bootles in the fridge to the shaker. The liquids will be mixed and a glass will drop to put your cocktail in. The machine will light green , it will mean that your cocktail will be done.
The apllication on the smartphone will estimate your alcohool level in your blood and warn you if go over the limits to drive.

## Code for the microcontroller ESP32 (wifi communications)

![Code_ESP32](\Code_ESP32.ino)

````C++
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>  //https://github.com/bbx10/WebServer_tng



WebServer server ( 80 );

const char* ssid     = "ssid";
const char* password = "password";



void setup() 
{
  Serial.begin(9600);

  connectToWifi();

  beginServer();
}

void loop() {
 
 server.handleClient();
 

 delay(1000);
 
}

void connectToWifi()
{
  WiFi.enableSTA(true);
  
  delay(2000);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);        
    }
  
}

void beginServer()
{
  server.on ( "/", handleRoot );
  server.begin();
}

void handleRoot(){ 
  if ( server.hasArg("cocktail") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }  
}


void handleSubmit() {

  String cocktail;
  cocktail = server.arg("cocktail"); 
  Serial.println(cocktail);    
  server.send ( 200, "text/html", getPage() );
}

String getPage(){
  String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='60'/>";
  page += "<title>ESP32 WebServer - educ8s.tv</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>";
  page += "</head><body><h1>ESP32 Cocktail WebServer</h1>";
  page += "<form action='/' method='POST'>";
  page += "<ul><li>Cocktail";
  page += "";
  page += "<INPUT type='text' name='cocktail'>";
  page += "<INPUT type='submit' value='Submit'>";

  page += "</body></html>";
  return page;
}
```


## Code for the Arduino Uno

```C++
//constantes des broches
const int POMPE[6] = {8,9,10,11,12,13};//liste des pompe doseuses

const int pompe=3; //PWM

const int led=2;

const int motDistrib=4;
const int motMel=5;//PWM

const int ev=6;

const int captG=7;

//variable dosage
char buff;
char tiret='-';
char retour='\n';
char recu[10];
String consigne;
String buff2;

int val=0;
int i;
int retourner = 0;
boolean memoire = LOW;

//variables capteur
int etatCaptG = LOW;
int memCaptG = LOW;

//variable de temps
long tempsMel = millis();
long tempsDistrib = millis();
long tempsPompe = millis();
long tempsFinal = millis();

//variable en fonction nombre bouteille
int doses[5];
int memDose[5];
long TEMPS[5];
boolean ETATDOSE[5];
boolean DOSER[5];
const boolean TYPE[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, LOW};//HIGH = pompe peristatique
                                                      //LOW = pompe pour poisson                                                   
//variable étape
boolean liquide = LOW;
boolean melanger = LOW;
boolean gobelet = LOW;
boolean final = LOW;
boolean rincage = LOW;

boolean etatFinal = LOW;
boolean etatPomp = LOW;
boolean etatDistrib = LOW;
boolean etatMel = LOW;



void setup()
{
  Serial.begin(9600);
  Serial.println("Arduino prêt");
  
  //dosage (depend nombre de bouteille) 
  for(int f=0; f<6; f++)
    pinMode(POMPE[f], OUTPUT);
    
  for(int h=0; h<6; h++)
    digitalWrite(POMPE[h], LOW);  

  //reste
  pinMode(pompe, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(motDistrib, OUTPUT);
  pinMode(motMel, OUTPUT);
  pinMode(ev, OUTPUT);
  pinMode(captG, INPUT);
  
  analogWrite(motMel, 0);
  analogWrite(pompe, 0);
  digitalWrite(led, HIGH);
  digitalWrite(motDistrib, LOW);
  digitalWrite(ev, LOW);
  
  //INNITIALISATION DES TABLEUX
  for (int u=0; u<5; u++)
    TEMPS[u] = millis();
  for (int t=0; t<5; t++)
    ETATDOSE[t] = LOW;
  for (int m=0; m<5; m++)
    DOSER[m] = LOW;
  for (int f=0; f<5; f++)
    memDose[f] = 0;
  for (int r=0; r<5; r++)
    doses[r] = 0;
  for (int e=0; e<5; e++)
    recu[e] = '\0';  
}

//------------Dosage-------------
void doser(long cl, int numero)
{

  if(ETATDOSE[numero] == LOW)
   {
     TEMPS[numero] = millis();
     ETATDOSE[numero] = HIGH;
     Serial.print("Etat dose : ");
     Serial.println(ETATDOSE[numero]);
     
     digitalWrite(POMPE[numero], HIGH);
     
     Serial.print("Dose HIGH :");
     Serial.println(ETATDOSE[numero]); 
   }
   Serial.println(DOSER[numero]);
   if(TYPE[numero])//si pompe peristatique
   {
     Serial.println("Perilsaltique");
     if((millis() - TEMPS[numero]) > (1000*10*cl) && ETATDOSE[numero] == HIGH)
     {
      Serial.print("If temps :");
      Serial.println(millis() - TEMPS[numero]);
       digitalWrite(POMPE[numero],LOW);
       Serial.println("Dose LOW");
       //ETATDOSE[numero] = LOW;
       DOSER[numero] = HIGH;
       Serial.print("Doser[numero] :");
       Serial.println(DOSER[numero]);
     }
   }
   else//si pompe modélisme
   {
     Serial.println("Normal");
     if((millis() - TEMPS[numero]) > (100*10*cl) && ETATDOSE[numero] == HIGH) //il faudra trouver la bonne conversion pour passer du temps aux volume en cl
     {
       digitalWrite(POMPE[numero],LOW);
       ETATDOSE[numero] = LOW;
       DOSER[numero] = HIGH;
     }
   } 
   Serial.println((1000*10*cl)-(millis()-TEMPS[numero]));
   Serial.print("Fin fct doser"); 
}



//------------Comparaison des tableaux-------------
int comparerTabl(int *recu)
{
  val = 1;
  for(int w=0; w<5; w++)//Si tout est doser
  {
    if(DOSER[w] != HIGH)//Si une boisson n'est pas doser
    {
      val = 0;
    }
  }
  for(int x=0; x<5; x++)
  {
    if(recu[x] == 66)//Si une boisson a 66 cl, cela est le mot STOP qui est dit, donc urgence
      val = 66;
    if(recu[x] == 55)//Si une boisson a 55 cl, commande specific inutile a ce jour
      val = 55;  
  }
  return val;
}



//------------RESET-------------
void reset()
{
liquide = LOW;
melanger = LOW;
gobelet = LOW;
final = LOW;
rincage = LOW;
etatFinal = LOW;
etatPomp = LOW;
etatDistrib = LOW;
etatMel = LOW;
etatCaptG = LOW;
memCaptG = LOW;
tempsMel = millis();
tempsDistrib = millis();
tempsPompe = millis();
tempsFinal = millis();
i = 0;
retourner = 0;
memoire = LOW;
for (int u=0; u<5; u++)
  TEMPS[u] = millis();
for (int t=0; t<5; t++)
  ETATDOSE[t] = LOW;
for (int m=0; m<5; m++)
  DOSER[m] = LOW;
for (int f=0; f<5; f++)
  memDose[f] = 0;
for (int r=0; r<5; r++)
  doses[r] = 0; 
for (int e=0; e<10; e++)
  recu[e] = '\0';  
  
analogWrite(motMel, 0);
analogWrite(pompe, 0);
digitalWrite(led, HIGH);
digitalWrite(motDistrib, LOW);
digitalWrite(ev, LOW);
for(int h=0; h<6; h++)
digitalWrite(POMPE[h], LOW);
}



void loop() 
{ 
  //reception des données téléphone
  i = 0;
  while (Serial.available() > 0)
  {
    buff = Serial.read();
    delay(5);
    
    if(buff != tiret && buff != retour)
    {
      recu[i] = buff;
      i++;
    }
    else
    {
      i++;
    }
  }
  
  //Parse
  for(int h=0;h<5;h++)
  {   
    buff2.concat(recu[2*h]);
    buff2.concat(recu[2*h+1]);
    doses[h] = buff2.toInt();
    Serial.print(memDose[h]);
    Serial.print("/");
    buff2="";
  }
  //-----
  Serial.print("\n");
  //Si on reçoit un message ou que la memoire est acvite
  if(recu[i] || memoire == HIGH)
  {
    retourner = comparerTabl(doses);//On compare pour savoir si on a finit de doser ou si on a une commande speciale
    
    if(liquide == LOW && rincage == LOW && final == LOW)//et que l'on est pas entrain de preparer un autre cocktail
    { 
      if(memoire == LOW)
      {
        memoire = HIGH;
        for(int p=0; p<5; p++)
        {
          memDose[p] = doses[p];//On enregistre le cocktail avant de RAZ, si commande spéciale pense a mettre a 0
        }
      }
      for(int j=0; j<5; j++)
      {
        doser(memDose[j], j); //On dose
        Serial.print("\n");
        Serial.print("Dosage : ");
        Serial.println(j);
      }
    }
    for(int g=0; g<10; g++)
    {   
      recu[g] = '\0';//remise à zéro de ce qu'on a recu
    }    
  }
  
  if(retourner == 1 || retourner == 66)//Si tous les liquides sont doser ou arret urgence
  {
    memoire = LOW;
    liquide = HIGH; //Dosage terminé
    for (int t=0; t<5; t++)
      ETATDOSE[t] = LOW;
    for (int m=0; m<5; m++)
      DOSER[m] = LOW;
      
    if(retourner == 66)//Arret D'urgence
      reset();
      
    retourner = 0;  
  }
  
  
  //-------2eme partie--------
  //melanger
  if (liquide == HIGH && melanger == LOW && etatMel ==LOW)
  {
    analogWrite(motMel, 21); //Vitesse moteur melangeur 1V
    tempsMel = millis();
    etatMel = HIGH;
  }
  if(millis()-tempsMel >= 5000 && liquide == HIGH && melanger == LOW)//temps melange
  {
    analogWrite(motMel, 0);
    melanger = HIGH;
    tempsMel = millis();
    etatMel = LOW;
  }   
  
  
  //lecture si présence gobelet
  gobelet = digitalRead(captG); 
  gobelet = !gobelet;
  
  
  // Distribution du gobelet
  if (liquide == HIGH && gobelet == LOW && etatDistrib == LOW && etatPomp == LOW)
  {
    etatDistrib = HIGH;
    tempsDistrib = millis();
    digitalWrite(motDistrib, HIGH);
  }  
  if(gobelet == HIGH)
  {
    digitalWrite(motDistrib, LOW);
    digitalWrite(led, HIGH);
    etatDistrib = LOW;
  }
  if(millis()- tempsDistrib >= 4000 && liquide == HIGH && gobelet == LOW)//temps si pas gobelet
  {
    digitalWrite(led, LOW);
    tempsDistrib = millis();
  }


  //pompage
  if(melanger == HIGH && gobelet == HIGH && etatPomp == LOW)
  {
    etatPomp = HIGH;
    analogWrite(pompe, 50);//vitesse versement dans le gobelet
    tempsPompe = millis();
  }
  if(millis()- tempsPompe > 4000 && rincage == LOW && liquide == HIGH && etatPomp == HIGH)//temps pour mettre dans le gobelet
  {
    liquide = LOW;
    melanger = LOW;
    etatPomp = LOW;
    analogWrite(pompe, 0);
    rincage = HIGH;
  }  
  
  
  //rincage
  if(rincage == HIGH)
  {
    digitalWrite(POMPE[5], HIGH);
    delay(2000);//temps de mise de l'eau pour rincer
    digitalWrite(POMPE[5], LOW);
    rincage = LOW;
    final = HIGH;
  }
  
  
  //final pompage
  if(final == HIGH && etatFinal == LOW && gobelet == LOW)
  {
    digitalWrite(pompe, HIGH);
    tempsFinal = millis();
    etatFinal = HIGH;
    
  }
  if(millis() - tempsFinal >= 2000 && etatFinal == HIGH)//temps pour vider la cuve
  {
    Serial.println("fin");
    digitalWrite(pompe, LOW);
    tempsFinal = millis();
    etatFinal = LOW;
    final = LOW;
  }
}
```

