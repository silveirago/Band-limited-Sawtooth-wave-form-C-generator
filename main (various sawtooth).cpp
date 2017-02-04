//
//  main.cpp
//  Sawtooth wavetable
//
//  Created by Gustavo Silveira on 1/26/17.
//  Copyright © 2017 Gustavo Silveira. All rights reserved.
//  Sawtooth wavetables generator


#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <math.h>

#include "midiFrequencies.h"

using namespace std;

float scaleBetween(float unscaledNum, float minAllowed, float maxAllowed, float min, float max) { //scaling function
    return (maxAllowed - minAllowed) * (unscaledNum - min) / (max - min) + minAllowed;
}

//const int16_t sampleRate = 44100;
const int16_t nyquist = 21000;

const int tableSize = 256; // the size of your wavetable
const int numberOfTables = 45;
//const int notesInTables = nyquist/numberOfTables;
const float PI = 3.141592653589793238462643;
//float phase = PI; //Uncoment this if you want to flip the wave phase
float phase = 0;
float angularFreq = (2* PI)/tableSize;
float table[numberOfTables][tableSize] = {{0},{0}};

float output;
int16_t scaledOutput;

float maxAmp = 1;
int numberOfHarmonics; //those will be calculated in the loop

float minValue = 0; //those will be used for normalizing the scale
float maxValue = 0;

int16_t resolution = 32767; //how big the biggest value will be


int z = 0;

int main(int argc, const char * argv[]) {
    
    
    //open file
    ofstream myFile;
    
    //This is the file the wavetable is going to show up in.
    myFile.open("sawtoothWave[45][257].h");
    
    myFile << "const int16_t sawtoothWavetable[45][257] = {";
    
    for (int t=0; t<numberOfTables; t++) {
        
        numberOfHarmonics = round(nyquist/midiFrequencies[z]); //skips every three notes
        
        for (int n=0; n<tableSize; n++) { //creates a Sawtooth wavetable
            
            for(int m=0; m<numberOfHarmonics; m++) {
                
                table[t][n] = (table[t][n] + ( (maxAmp/(m+1) * sin((angularFreq*(m+1))*n + (phase))))); //sawtooth formula
            }
            
            if (table[t][n] > maxValue) { //checks highest value
                maxValue = table[t][n];
            }
            if (table[t][n] < minValue) { //checks lowest value
                minValue = table[t][n];
            }
            
        }
        z = z+3;
    }
    
    // numberOfHarmonics = nyquist/midiFrequencies[z]; //skips every three notes
    cout <<"maxValue = ";
    cout << maxValue;
    cout <<" | minValue = ";
    cout << minValue;
    
    for (int t=0; t<numberOfTables; t++) {
        myFile << "{";
        
        for (int i=0; i<tableSize; i++) { // normalize the table
            
            output = table[t][i];
            output = scaleBetween(output, resolution*(-1), resolution, minValue, maxValue);
            output = round(output);
            myFile << output; // writes each scaled value in the table
            myFile << ",";
        }
        
        myFile << "0}, \n"; // the table needs this last value
        //close connection to text file
    }
    myFile << "};";
    
    myFile.close();
    
    return 0;
    
}