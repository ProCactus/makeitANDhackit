#include <LiquidCrystal.h>




LiquidCrystal lcd(8,9,4,5,6,7);
// Variables to Keep ( DO NOT DELETE THESE )
const int RelayPin = 13;
const int ButtonPressPin = A1;
const int AnalogMic = A0;

// ^^^^^^^^^^^^^ (DO NOT DELETE ABOVE) ^^^^^^^^^^

int SychicThought[5];                 //  Experimental Machine Thoughts, *#* Try to use only 1
int StrobeMode = 2;                   //  *!* Sets Flash Condition Profiles.
                                      //  *!* 1 = Normal (Live Mode)
                                      //  *!* 2 = Guess Locked (Sets the strobe to Guess Mode) 

int tMicIn;
const int MaxMicArray = 5;            //  *!* Sets the Microphone Array Size.
int MicArray[MaxMicArray];
int MicArrayTick = 1;

const int MaxTriggerRate = 200;       //  *!* Sets the Maximum Re-Trigger in milliseconds. Less means Faster.
int FlashTrigger = 500;               // Pre-Adjusts the Auto Mic Tracking Variable . *** This might not be needed.
int FlashOnTime = 30;                 // Sets the On-Time for the Stobe in milli seconds.
int FlashFade;
int ButtonState;
int TrigSensitivity;
int BeatsPerMin;





const int MaxArraySize =15;             //  *!* Sets the Maximum Array for BPM related Arrays (BeatsArray and BPMMatches).
int BPMMatchesLimit = 15;               //  *!*  Sets the Maximum Values in the BPMMatches Array
int BeatTick;                           //  1 to MaxArraySize
int BeatCheckTick;                      //  1 to MaxArraySize
int BeatCheckTick2;                     //  This is a test variable. It may or may not be really needed.
                                        //  I think the code would be more efficient if not needed.
int ArraySwipeCount;
int BPMLocked;
int BeatsArray[MaxArraySize];
int BPMMatches[MaxArraySize];


int BPMGuessmS;
long TempoMillis;
long TempoMillisOld;


long CurrentMillis;
long LastFlash = 0;
long NextFlash = 0;

int lastButtonState = LOW;
int Button;
long lastDebounceTime = 0;

long TempLong;


void setup() {
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);

  //Serial.begin(115200);
  lcd.begin(16, 2);
// lcd.print("hello, world!");
// lcd.setCursor(0, 0);
// lcd.noDisplay();
// lcd.display();
// lcd.autoscroll();
// lcd.noAutoscroll();
// lcd.clear();

  TrigSensitivity = 8;                        //  *!* Pre-Sets the Sensitivity
  BPMLocked = 297;
  
      lcd.setCursor(4, 0);
      lcd.print ("   ");
      lcd.setCursor(0, 0);
      lcd.print ("BPM=");
      lcd.print (BeatsPerMin);
      lcd.setCursor(0, 1);
      lcd.print ("Sence=");
      lcd.print(TrigSensitivity);
      lcd.setCursor(8,0);
      lcd.print ("No Lock");

  CurrentMillis = millis();

}

void loop() {

  ButtonState = analogRead(ButtonPressPin);
  Button = map(ButtonState, 0, 560, 0, 4);

//Serial.print(tMicIn);
//Serial.print(" ");
CurrentMillis = millis();




MicArrayTick = MicArrayTick +1;
if (MicArrayTick > MaxMicArray){
  MicArrayTick = 1;
}
MicArray[MicArrayTick] = tMicIn;








if (Button > 6 ){
  lastDebounceTime = CurrentMillis;
}
if (lastDebounceTime +100 < CurrentMillis){
  lastDebounceTime = CurrentMillis;
  if (ButtonState < 60) {
    //lcd.print ("Right ");
  }
  else if (ButtonState < 200) {
      TrigSensitivity = TrigSensitivity + 1;
      if (TrigSensitivity > 199) {
        TrigSensitivity=199;
      }
      lcd.setCursor(6, 1);
      lcd.print ("   ");
      lcd.setCursor(0, 1);
      lcd.print ("Sence=");
      lcd.print(TrigSensitivity);
  }
  else if (ButtonState < 400){
      TrigSensitivity = TrigSensitivity - 1;
      if (TrigSensitivity < 1 ) {
        TrigSensitivity=1;
      }
      lcd.setCursor(6, 1);
      lcd.print ("   ");
      lcd.setCursor(0, 1);
      lcd.print ("Sence=");
      lcd.print(TrigSensitivity);  
  }
  else if (ButtonState < 600){
    //lcd.print ("Left  ");
  }
  else if (ButtonState < 800){
    //for (BeatCheckTick = 1; BeatCheckTick < MaxArraySize; BeatCheckTick++){
    //  Serial.print (BeatsArray[BeatCheckTick]);
    //  Serial.print (" ");
    //  Serial.println (BPMMatches[BeatCheckTick]);
    //} 
    //Serial.println ("*~*");
  }
  
}









  tMicIn = analogRead(AnalogMic);               // Reads the Mic level into tMicIn
  if (tMicIn > FlashTrigger) {                  // Adjust The Trigger level depending on Mic Level 
    FlashTrigger = FlashTrigger +1;
  }
  if (tMicIn < FlashTrigger) {
    FlashTrigger = FlashTrigger -1;
  }

  CurrentMillis = CurrentMillis;
  if (StrobeMode == 2 ) {
    if (BPMLocked < 290 ){
      if (CurrentMillis > TempoMillis) {
        TempoMillis = CurrentMillis + 60000 / BPMLocked;
        TempoMillisOld = TempoMillis;
        TempLong = TempoMillis - CurrentMillis;
        lcd.setCursor(10,1);
        lcd.print("      ");
        lcd.setCursor(10,1);
        lcd.print(TempLong);
        digitalWrite(RelayPin,HIGH);                                       //  *** These digitalWrite's (LOW to HIGH) are with the Delay Obviously.
        delay(30);                                                        //  *** This Delay needs to be Removed in Future.
        digitalWrite(RelayPin,LOW);                                      //  *** Then I beleive this is a 100% State Machine.
        
      
      }
    }
  }
  
  if (CurrentMillis > NextFlash) {                                        // 
    if (tMicIn  > (FlashTrigger + TrigSensitivity)) {                     // Compare the Mic Level with the combined Trigger Level
      LastFlash = CurrentMillis - NextFlash + MaxTriggerRate;
      if (StrobeMode == 1){                                               //  Make it Flash!
        digitalWrite(RelayPin,HIGH);                                       //  *** These digitalWrite's (LOW to HIGH) are with the Delay Obviously.
        delay(30);                                                        //  *** This Delay needs to be Removed in Future.
        digitalWrite(RelayPin,LOW);                                      //  *** Then I beleive this is a 100% State Machine.
      }
      NextFlash = CurrentMillis + MaxTriggerRate;
      BeatsPerMin = 60000/LastFlash;
      
      BeatTick = BeatTick +1;
      if (BeatTick > MaxArraySize ) {
        BeatTick=1;
      }
      BeatsArray[BeatTick] = BeatsPerMin;
      lcd.setCursor(4, 0);
      lcd.print ("   ");
      lcd.setCursor(0, 0);
      lcd.print ("BPM=");
      lcd.print (BeatsPerMin);
      lcd.setCursor(6, 1);
      lcd.print ("   ");
      lcd.setCursor(0, 1);
      lcd.print ("Sence=");
      lcd.print(TrigSensitivity);
      
      for (BeatCheckTick = 1; BeatCheckTick < MaxArraySize; BeatCheckTick++){                     //  *?* Something fucked up here.
        
        
        if (BPMMatches[BeatCheckTick] > BPMMatchesLimit){               // *!* This sets the BPMMatches Limit. Effectivly changes how sure a BPM Lock is.
          BPMLocked = BeatsArray[BeatCheckTick];                        // This may need to be removed after a better BPM lock is made.
          BPMGuessmS = 60000 / BPMLocked;                               // Turns the BPM back into mS for Future BPM Prediction.
          
          TempoMillis = TempoMillisOld - (CurrentMillis + BPMGuessmS);
          BPMMatches[BeatCheckTick] = 0;
          //Serial.println ("*~*");
          lcd.setCursor(13,0);
          lcd.print ("   ");
          lcd.setCursor(8,0);
          lcd.print ("         ");
          lcd.setCursor(8,0);
          lcd.print ("Lock=");
          lcd.print (BPMLocked);
          //lcd.setCursor(10,1);
          //lcd.print (BPMGuessmS);
          
          // 
        }
        
        
        
        
        
        
        
        if (BeatsArray[BeatCheckTick] > 0 ){                                                      //  Make sure BPM is higher than 0.
          if (BeatTick != BeatCheckTick){                                                         //  Avoid Counting itself.
            if (BeatsArray[BeatTick]==BeatsArray[BeatCheckTick]){                                 //  Detect same value(BPM) in BeatsArray and BPMMatches.
              for (BeatCheckTick2 = 1; BeatCheckTick2 < MaxArraySize; BeatCheckTick2++){          //  *?* WTF!! Am I using BeatCheckTick2 ?
                if (BeatsArray[BeatCheckTick2] == BeatsArray[BeatCheckTick]){                     //  *?*
                  BPMMatches[BeatCheckTick2]++;
                    
                }
              }
            }
          }
        }

        
        
        
      }
//    * Any code here has atleast MaxTriggerRate (milliSeconds) ie. 200mS to complete
//    * Using this portion of code space will not affect any other opperations during the MaxTriggerRate timeframe.
//    * Yay !! :)







//    ^*^ Keep above this line ^*^ Keep above this line ^*^ Keep above this line ^*^ Keep above this line ^*^
//    Thanks :D
    }
  }
}
