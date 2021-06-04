#include <Adafruit_NeoPixel.h>

#define PIN 2   // input pin Neopixel is attached to
#define NUMPIXELS 221  // number of neopixels

const byte lights[20][10] PROGMEM = {       // Map the x,y coordinates of each led on the strip
  {  220 , 219 , 218 , 217 , 216 , 215 , 214 , 213 , 212 , 211 },
  { 199 , 200 , 201 , 202 , 203 , 204 , 205 , 206 , 207 , 208 },
  { 198 , 197 , 196 , 195 , 194 , 193 , 192 , 191 , 190 , 189 },
  { 177 , 178 , 179 , 180 , 181 , 182 , 183 , 184 , 185 , 186 },
  { 176 , 175 , 174 , 173 , 172 , 171 , 170 , 169 , 168 , 167 },
  { 155 , 156 , 157 , 158 , 159 , 160 , 161 , 162 , 163 , 164 },
  { 154 , 153 , 152 , 151 , 150 , 149 , 148 , 147 , 146 , 145 },
  { 133 , 134 , 135 , 136 , 137 , 138 , 139 , 140 , 141 , 142 },
  { 132 , 131 , 130 , 129 , 128 , 127 , 126 , 125 , 124 , 123 },
  { 111 , 112 , 113 , 114 , 115 , 116 , 117 , 118 , 119 , 120 },
  { 110 , 109 , 108 , 107 , 106 , 105 , 104 , 103 , 102 , 101 },
  { 89  , 90  , 91  , 92  , 93  , 94  , 95  , 96  , 97  , 98  },
  { 88  , 87  , 86  , 85  , 84  , 83  , 82  , 81  , 80  , 79  },
  { 67  , 68  , 69  , 70  , 71  , 72  , 73  , 74  , 75  , 76  },
  { 66  , 65  , 64  , 63  , 62  , 61  , 60  , 59  , 58  , 57  },
  { 45  , 46  , 47  , 48  , 49  , 50  , 51  , 52  , 53  , 54  },
  { 44  , 43  , 42  , 41  , 40  , 39  , 38  , 37  , 36  , 35  },
  { 23  , 24  , 25  , 26  , 27  , 28  , 29  , 30  , 31  , 32  },
  { 22  ,  21  , 20  , 19  , 18  , 17  , 16  , 15  , 14  , 13  },
  { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10  }
};

const uint8_t PROGMEM gamma8[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

String serialResponse = "";
String newString = "";
String command = "";
String commandQueue = "";

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pixels.begin(); // Initializes the NeoPixel library.
}

void loop() {
  serialEvent();
}

void serialEvent() {
  if ( Serial.available()) {
    serialResponse = Serial.readString();
    commandQueue += serialResponse;
    while (commandQueue.length() > 12) {
      // get first 13 chars off the response
      newString = commandQueue.substring(0, 13);
      Serial.print("Current Command: ");
      Serial.println(newString);
      // remove those chars off the "queue"
      commandQueue.remove(0, 13);
      Serial.print("Queue: ");
      Serial.println(commandQueue);
      getCommand();
    }
  }
}

void getCommand() {
  //ex: 0305255050255 = (3,5) 255,50,255
  // Set led at coordinates (3,5) (index 157) to rgb value 255,50,255
  int x = newString.substring(0, 2).toInt();
  int y = newString.substring(2, 4).toInt();
  int r = newString.substring(4, 7).toInt();
  int g = newString.substring(7, 10).toInt();
  int b = newString.substring(10, 13).toInt();
  int index = getLedIndexByCoordinates(x, y);
  setPixelColor(index, r, g, b);
}

void setPixelColor(int index, int r, int g, int b) {
  String msg = "setPixelColor(";
  msg = msg + index + ", " + r + ", " + g + ", " + b + ")";
  Serial.println(msg);
  pixels.setPixelColor(index, pgm_read_byte(&gamma8[r]), pgm_read_byte(&gamma8[g]), pgm_read_byte(&gamma8[b]));
  pixels.show();
}

int getLedIndexByCoordinates(int x, int y) {
  return pgm_read_byte_near(&lights[y][x]);
}