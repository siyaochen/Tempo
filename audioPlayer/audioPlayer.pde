import processing.serial.*;
import ddf.minim.*;

// Initialize audio variables
Minim minim;
AudioPlayer player;

// Initialize Arduino communication variables
Serial port;  // Create object from Serial class
String val;   // Data received from the serial port

void setup()
{
  // Set up GUI
  size(512, 200, P3D);
  
  // Initialize audio player object
  minim = new Minim(this);
  
  // SONGS LIST
  // The Weeknd - Wicked Games.mp3
  // Lil Uzi Vert - XO TOUR Llif3.mp3
  // Rihanna - Love On The Brain.mp3
  player = minim.loadFile("Lil Uzi Vert - XO TOUR Llif3.mp3");

  
  // Set up serial communication
  // MAC PORT: /dev/cu.usbmodem14101
  // WINDOWS PORT: COM5
  port = new Serial(this, "COM5", 9600);
}

void draw()
{ 
  // GUI VISUALIZATION STUFF
  background(0);
  stroke(255);
  
  for(int i = 0; i < player.bufferSize() - 1; i++)
  {
    float x1 = map( i, 0, player.bufferSize(), 0, width );
    float x2 = map( i+1, 0, player.bufferSize(), 0, width );
    line( x1, 50 + player.left.get(i)*50, x2, 50 + player.left.get(i+1)*50 );
    line( x1, 150 + player.right.get(i)*50, x2, 150 + player.right.get(i+1)*50 );
  }
  
  float posx = map(player.position(), 0, player.length(), 0, width);
  stroke(0,200,0);
  line(posx, 0, posx, height);
}

void serialEvent(Serial port)
{
  // Read in value
  val = port.readStringUntil('\n');
  
  if (val != null)
  {
    val = trim(val);
    
    // Rewind and play value
    if (val.equals("start"))
    {
      player.rewind();
      player.play();
    }
    
    // Stop playing
    if (val.equals("end"))
    {
      player.pause();
    }
  }
}
