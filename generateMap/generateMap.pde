/*
 * This program automatically generates notes for any song.
 * How to use:
 * Put any song renamed to "song.mp3" inside the data folder and run this code, which will generate
 * corresponding note maps for the song inside the "notes.txt" and "withComma.txt" files. Copy-paste these
 * note maps to the main game file to play your song in the game!
 */

import ddf.minim.*;
import ddf.minim.analysis.*;

// Declare audio variables
Minim minim;
AudioPlayer song;
BeatDetect beat;
BeatListener bl;


float kickSize, snareSize, hatSize;
int counter = 0;
int start;
int elapsed;
int end = 45 * 1000;
int current;
int last = 0;
String strip1 = "", strip2 = "", strip3 = "";
String cs1 = "", cs2 = "", cs3 = "";

class BeatListener implements AudioListener
{
  private BeatDetect beat;
  private AudioPlayer source;
  
  BeatListener(BeatDetect beat, AudioPlayer source)
  {
    this.source = source;
    this.source.addListener(this);
    this.beat = beat;
  }
  
  void samples(float[] samps)
  {
    beat.detect(source.mix);
  }
  
  void samples(float[] sampsL, float[] sampsR)
  {
    beat.detect(source.mix);
  }
}

void setup()
{
  start = millis();
  size(512, 200, P3D);
  
  // Initialize audio variables
  minim = new Minim(this);
  
  // Play song
  song = minim.loadFile("song.mp3", 1024);
  song.play();
  
  // Create a beat detection object that is FREQ_ENERGY mod
  beat = new BeatDetect(song.bufferSize(), song.sampleRate());
  beat.setSensitivity(10);  
  kickSize = snareSize = hatSize = 16;
  
  // Make a new beat listener, so that we won't miss any buffers for the analysis
  bl = new BeatListener(beat, song);
  
  // GUIT
  textFont(createFont("Helvetica", 16));
  textAlign(CENTER);
}

void draw()
{
  background(0);
  
  // Update timing
  current = millis();
  elapsed = current - start;
  
  // If-statement to synchronize timing with game
  if (current - last > 215)
  {
    last = current;
    
    // GUI draw a green rectange on beats
    float rectW = width / beat.detectSize();
    for(int i = 0; i < beat.detectSize(); ++i)
    {
      // test one frequency band for an onset
      //println("DetectSize: "+beat.detectSize());
      if (beat.isOnset(i))
      {
        fill(0,200,0);
        rect( i*rectW, 0, rectW, height);
      }
    }
  
    // GUI draw orange rectange
    int lowBand = 5;
    int highBand = 15;
    int numberOfOnsetsThreshold = 4;
    if (beat.isRange(lowBand, highBand, numberOfOnsetsThreshold))
    {
      fill(232,179,2,200);
      rect(rectW * lowBand, 0, (highBand - lowBand) * rectW, height);
    }
    
    // Detect type of beat and display/update array accordingly
    if (beat.isKick())
    {
      println(elapsed/1000+" s");
      println("kick");
      kickSize = 32;
      strip1 = strip1 + "1";
      cs1 = cs1 + "1,";
    }
    else
    {
      cs1 = cs1 + "0,";
      strip1 = strip1 + "0";
    }
    if (beat.isSnare())
    {
      println(elapsed/1000 + " s");
      println("snare");
      snareSize = 32;
      strip2 = strip2 + "2";
      cs2 = cs2 + "2,";
    }
    else
    {
      cs2 = cs2 + "0,";
      strip2 = strip2 + "0";
    }
    if (beat.isHat())
    {
      println(elapsed/1000 + " s");
      println("hat");
      hatSize = 32;
      strip3 = strip3 + "3";
      cs3 = cs3 + "3,";
    }
    else
    {
      strip3 = strip3 + "0";
      cs3 = cs3 + "0,";
    }
    
    
    // UPDATE GUI
    fill(255);
      
    textSize(kickSize);
    text("1", width/4, height/2);
    
    textSize(snareSize);
    text("2", width/2, height/2);
    
    textSize(hatSize);
    text("3", 3*width/4, height/2);
    
    kickSize = constrain(kickSize * 0.95, 16, 32);
    snareSize = constrain(snareSize * 0.95, 16, 32);
    hatSize = constrain(hatSize * 0.95, 16, 32);
    
    // Update and display counter
    counter++;
    println("Counter: "+counter);
  }
  
  // Exit if exceeded maximum song length
  if (counter >= 200)
  {
    // Format strings
    strip1 = strip1 + " ";
    strip2 = strip2 + " ";
    strip3 = strip3+" ";
    
    cs1 = cs1 + " ";
    cs2 = cs2+" ";
    cs3 = cs3+" ";
    
    // Format list
    String all = strip1 + strip2 + strip3;
    String list[] = split(all, " ");
    
    // Format listComma
    String allComma = cs1 + cs2 + cs3;
    String listComma[] = split(allComma, " ");
    
    // Save to text files
    saveStrings("notes.txt", list);
    saveStrings("withComma.txt", listComma);
    
    // Display the generated notes
    println(strip1);
    println(strip2);
    println(strip3);
    
    // Long delay to prevent looping again
    delay(100000000);
  }
}
