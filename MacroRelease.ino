// Copyright The University of Auckland all rights reserved (2016)
//
//
// The following code provides a user the ability to act out a series of pre-programmed scripts stored on an SD card. In total 9 script slots are available with not real limit on size.
// A more detailed instruction for writing scripts will be provided in a README.txt file. This code is built for a Pro-Micro (32U4 5v/16MHz) board connected to a Micro SD card reader,
// and a proprietary button array. In essence, this provides the user with a macro keyboard which can, and must, be pre-programmed to run proprietary scrips. There is a limitation that
// only 1 script can be completed at a time with no interruptions accepted (So once a script starts, you cannot end it (unless you unplug the pro-micro)).
//
// Functionality:
// The code starts by checking if any of the buttons are depressed. If so, then the corresponding file will be opened (A00.txt, A01.txt ... A22.txt)the name of which corresponds to the
// position of the button in a zero indexed matrix. readFile() is then called to read the file, line by line, until it finishes the last line available. Each line is passed into a 
// seperate function (actInstruction()) where it is decoded and the relevant instruction is completed. Finally, after finishing with the specified file, the file will be closed, and
// the micro-controller will be ready to recieve another instruction. 
//
// For the Pro-Micro:
//
// SD card connections:
// miso: 14
// mosi: 16
// clk: 15
// cs: 10
//
// Matrix connections:
// rowOne: 9
// rowTwo: 3
// rowThree: 4
// columnOne: 5
// columnTwo: 6
// columnThree: 7
//
// Written by W.K. Liu
// Date: 21/07/2018

#include <Keypad.h>
#include <SD.h>
#include <Keyboard.h>
#include <Mouse.h>

// Initialize the File object
File instructionFile;

// This matrix keypad was inspired by work from https://www.electronicshub.org/arduino-keypad-interfacing-tutorial/
const byte ROWS = 3; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
                             {'1','2','3'},
                             {'4','5','6'},
                             {'7','8','9'},
                                              };
byte rowPins[ROWS] = {9, 3, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 6, 7}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {

  Serial.begin(9600);

  while(!Serial){
    // wait for the serial to connect for native USB. Can be removed after testing finishes
  }

  if (!SD.begin(10)){ // Initialize the SD object with port 10 as the card select pin
    Serial.println("SD card failed to open");
  } 

}

void loop() {

  // Get the key that was pressed (From 1-9 indicating A00 to A22 respectively)
  char customKey = customKeypad.getKey();
  
// Act out the according customKeys. For testing, we will just print the index to the serial
  switch(customKey) {
    
    case '1':
    instructionFile = SD.open("A00.txt"); // Open the file to our file instance
    readFile();
    break;
    
    case '2':
    instructionFile = SD.open("A01.txt");
    readFile();
    break;
    
    case '3':
    instructionFile = SD.open("A02.txt");
    readFile();
    break;
    
    case '4':
    instructionFile = SD.open("A10.txt");
    readFile();    
    break;
    
    case '5':
    instructionFile = SD.open("A11.txt");
    readFile();    
    break;
    
    case '6':
    instructionFile = SD.open("A12.txt");
    readFile();    
    break;
    
    case '7':
    instructionFile = SD.open("A20.txt");
    readFile();    
    break;
    
    case '8':
    instructionFile = SD.open("A21.txt");
    readFile();    
    break;
    
    case '9':
    instructionFile = SD.open("A22.txt");
    readFile();    
    break;
  }

}


void readFile(){
// The following function reads an instruction set preprogrammed by a user from the start to the finish.
// The name of the file should correspond to the 'name' input. (i.e A00.txt). Any errors will be printed out

  if(instructionFile){ // If it is found then we start reading, if it isnt then we alert the user of the error.
    while(instructionFile.available()){
      
      // Special thanks to this piece of wisdom: https://stackoverflow.com/questions/35486045/arduino-reading-sd-file-line-by-line-c
      String buffer = instructionFile.readStringUntil('\n'); 
         
      actInstruction(buffer);
      
    }
    
  }else{
    
    Serial.println("Error! File not found! Please check the files.");
    
  }

  instructionFile.close(); // close the file to ensure no errors later
  
}

void actInstruction(String buffer){
 // The following function takes in a string command, and acts out the relevant command according to the parameters in the string.
 // The mouse and keyboard commands shall be indexed in the following way:
 // Mouse: [M/K index(1 character)] [Click/No Click/Hold/Release (1/0/2/3)(1 character)] [Left/Right/Middle (1 character)] [horizontal movement(4 characters)] [vertical movement(4 characters)] [number of steps(3 characters)] [timestep(3 characters)] \n
 // Keyboard: [M/K index(1 character)] [Write/Press/Release(1 character)] [alphanumeric indicator(1 character)] [key(9 characters)] [number of steps(3 characters)] [timestep(3 characters)] \n
 // Note that for the keyboard, the alphanumeric indicator is a number that uses 1 for normal, 2 for special. Write/Press/Release 1/2/3 
 // The following program will deconstruct each buffer and act out the instruction based on the programming manual and instruction set.
  //Serial.println(buffer);


  char typeOfCommand = buffer.charAt(0);
  
  if (typeOfCommand == 'M'){

    Mouse.begin(); // starts Mouse control, It is kept here to minimize time that control is surrendered to the Mouse Object
    char clickStatus = buffer.charAt(2); // extract relevant information


    for (int i = 0; i < (buffer.substring(16,18+1).toInt()); i++){ // loops through the iteration according to number of steps specified (extracted from our code)
      
      switch (clickStatus){

        case '0':
        // Mouse movement. (x, y) negative x to go left, negative y to go up
        Mouse.move(buffer.substring(6,9+1).toInt(), buffer.substring(11,14+1).toInt());
        break;
        
        case '1':
        // Mouse click buttons according to specified button
        if (buffer.charAt(4) == 'L'){
          Mouse.click(MOUSE_LEFT);
        }else if(buffer.charAt(4) == 'R'){
          Mouse.click(MOUSE_RIGHT);
        }else if(buffer.charAt(4) == 'M'){
          Mouse.click(MOUSE_MIDDLE);
        }
        else{
          Serial.println("Incorrect argument, the thirk argument should be L, R, or M");
        }
        break;

        case '2': // Case 2 is for holding the click button
        if (buffer.charAt(4) == 'L'){
          Mouse.press(MOUSE_LEFT);
        }else if(buffer.charAt(4) == 'R'){
          Mouse.press(MOUSE_RIGHT);
        }else if(buffer.charAt(4) == 'M'){
          Mouse.press(MOUSE_MIDDLE);
        }   
        break;

        case '3': // Case 3 is for releasing the button
        if (buffer.charAt(4) == 'L'){
          Mouse.release(MOUSE_LEFT);
        }else if(buffer.charAt(4) == 'R'){
          Mouse.release(MOUSE_RIGHT);
        }else if(buffer.charAt(4) == 'M'){
          Mouse.release(MOUSE_MIDDLE);
        }
        break;
        
      }
      
      delay(buffer.substring(20,22+1).toInt()); // Add a delay as specified
    }
    
  Mouse.end(); // end mouse control
  
  } else if (typeOfCommand == 'K'){
    
    Keyboard.begin(); // start keyboard control
    
    char writePressRelease = buffer.charAt(2); // extract relevant information
    char indicator = buffer.charAt(4);

    
    for (int i = 0; i < (buffer.substring(16,18+1).toInt()); i++){
      
       switch(indicator) {
        
        case '1':
        // alphanumeric keyboard button representation. 

        switch(writePressRelease){ // act out the according argument extracted from the line

          
          case 'W':
          // Write
          Keyboard.write(buffer.charAt(14));
          break;

          case 'P':
          // Press
          Keyboard.press(buffer.charAt(14));
          break;

          case 'R':
          //Release
          Keyboard.press(buffer.charAt(14));
          break;
        }
        
        break;

        case '2': // A 2 indicates the use of decimal values(3 digits (12, 13, 14)). Refer to ascii or given table
        // This method is used to access special keys such as control, caps lock etc.

        switch(writePressRelease){
          
          
          case 'W':
          // Write
          Keyboard.write(buffer.substring(12,14+1).toInt()); // sends an int which indicates the decimal value of the desired press. 
          break;

          case 'P':
          // Press
          Keyboard.press(buffer.substring(12,14+1).toInt());
          break;

          case 'R':
          //Release
          Keyboard.press(buffer.substring(12,14+1).toInt());
          break;
        }
        
        break;
        
       }
      delay(buffer.substring(20,22+1).toInt()); // Add a delay as specified
    }
    
    Keyboard.end(); // end keyboard control
    
  } else {
    
    Serial.println("Incorrect argument, the first argument should be M or K");
    
  }
  
}


