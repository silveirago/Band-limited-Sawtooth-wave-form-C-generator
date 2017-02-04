//
//  main.cpp
//  Sawtooh wavetable
//
//  Created by Gustavo Silveira on 1/26/17.
//  Copyright © 2017 Gustavo Silveira. All rights reserved.
//  Sawtooth wavetable generator


#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

float scaleBetween(float unscaledNum, float minAllowed, float maxAllowed, float min, float max) { //scaling function
    return (maxAllowed - minAllowed) * (unscaledNum - min) / (max - min) + minAllowed;
}

const int tableSize = 256; // the size of your wavetable
const float PI = 3.141592653589793238462643;
float angularFreq = (2* PI)/tableSize;
float table[tableSize] = {0};
float output;
int16_t scaledOutput;

float maxAmp = 1;
float numberOfHarmonics = 27; //choose how many harmonics your tabke will have

float minValue = 0; //those will be used for normalizing the wavetable
float maxValue = 0;

int16_t resolution = 32767; //how big the biggest value will be. Use "32767" for teensy

int main(int argc, const char * argv[]) {
    
    //open file
    ofstream myFile;
    
    //This is the file the wavetable is going to show up in.
    myFile.open("sawtoothWave[257].h");
    
    myFile << "const int16_t sawtoothWaveTable16bit[257] = {";
    
    for (int n=0; n<tableSize; n++) { //creates a Sawtooth wavetable
        
        for(int m=0; m<numberOfHarmonics; m++) {
            
            table[n] = (table[n] + ( (maxAmp/(m+1) * sin((angularFreq*(m+1))*n)))); //sawtooth formula
        }
        
        if (table[n] > maxValue) { //checks highest value
            maxValue = table[n];
        }
        if (table[n] < minValue) { //checks lowest value
            minValue = table[n];
        }
        
    }
    
    cout <<"maxValue = ";
    cout << maxValue;
    cout <<" | minValue = ";
    cout << minValue;
    
    
    for (int i=0; i<tableSize; i++) { // normalize the table
        
        output = table[i];
        output = scaleBetween(output, resolution*(-1), resolution, minValue, maxValue);
        output = round(output);
        myFile << output; // writes each scaled value in the table
        myFile << ", ";
    }
    
    myFile << "0};"; // the table needs this last value
    //close connection to text file
    
    myFile.close();
    
    return 0;
    
}