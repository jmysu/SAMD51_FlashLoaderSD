
/* Routine to Draw Large 7-Segment formated number with Arduino TFT Library
Contributed by William Zaggle  (Uses TFT Library DrawLine or DrawFastLine functions).

long n - The number to be displayed
int xLoc = The x location of the upper left corner of the number
int yLoc = The y location of the upper left corner of the number
int cS = The size of the number.
fC is the foreground color of the number
bC is the background color of the number (prevents having to clear previous space)
nD is the number of digit spaces to occupy (must include space for minus sign for numbers < 0).


// Example to draw the number 37 in four directions in four corners of the display
draw7Number(37,10,10,4, ILI9341_WHITE , ILI9341_BLACK,2);       //LEFT2RIGHT
draw7Number90(37,10,310,4, ILI9341_WHITE , ILI9341_BLACK,2);    //DOWN2UP
draw7Number180(37,230,310,4, ILI9341_WHITE , ILI9341_BLACK,2);  //RIGHT2LEFT
draw7Number270(37,230,10,4, ILI9341_WHITE , ILI9341_BLACK,2);   //UP2DOWN
*/
#include <Arduino.h>
#include <TFT_eSPI.h>
extern TFT_eSPI tft;

void draw7Number(long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
	unsigned int num = abs(n), i, s, t, w, col, h, x, y, si = 0, j = 1, d = 0;
	unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
	unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
	unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
	unsigned int x1 = cS + 1;                     // starting x location of horizontal segments
	unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
	unsigned int y1 = yLoc + x1;                  // starting y location of top side segments
	unsigned int y3 = yLoc + S3 + S4 + 1;         // starting y location of bottom side segments
	// actual x,y locations and direction of all 7 segments 
	unsigned int seg[7][2] = {{x1, yLoc}, {x2, y1}, {x2, y3 + x1}, {x1, (2 * y3) - yLoc}, {0, y3 + x1}, {0, y1}, {x1, y3}}; 
	// segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (*,g,f,e,d,c,b,a)  
	static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 
	unsigned char c, cnt;

	c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
	if (c>10) c= 10;
	if (c<1) c = 1;

	cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
	if (cnt > 10) cnt = 10;
	if (cnt < 1) cnt = 1; 

	d = S2+(3*S3)+2;                              // width of one number

	xLoc += (cnt-1) * d;                          // set starting x at last digit location

	while( cnt > 0) {                             // for cnt number of places

		--cnt;

		if (num > 9) i = num%10;                    // get the last digit 
		else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
		else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
		else i = num;

		num = num/10;                               // trim this digit from the number  

		for (j = 0; j < 7; ++j) {                   // draw all seven segments

			if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
			else col = bC;                            // else use background color

			if (j==0 || j==3 || j==6) {

				w = S2;                                 // Starting width of segment (side)
				x = xLoc + seg[j][0] + cS;              // starting x location
				y = seg[j][1];                          // starting y location
				t = y + S3;                             // maximum thickness of segment
				h = y + S3/2;                           // half way point thickness of segment


				while (y < h) {                         // until y location = half way
					tft.drawFastHLine(x, y, w, col);      // Draw a horizontal segment top 
					++y;                                  // move the y position by 1
					--x;                                  // move the x position by -1           
					w += 2;                               // make the line wider by 2
				}

				while (y < t) {                         //finish drawing horizontal bottom  
					tft.drawFastHLine(x, y, w, col);      // Draw Horizonal segment bottom
					++y;                                  // keep moving the y or y draw position until t 
					++x;                                  // move the length or height starting position back the other way.
					w -= 2;                               // move the length or height back the other way
				}

			} else {

				w = S4;                                 // Starting height of segment (side)
				x = xLoc + seg[j][0];                   // starting x location
				y = seg[j][1] + cS;                     // starting y location 
				t = x + S3;                             // maximum thickness of segment
				h = x + S3/2;                           // half way point thickness of segment

				while (x < h) {                         // until x location = half way
					tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
					++x;                                  // move the x position by 1
					--y;                                  // move the y position by -1          
					w += 2;                               // make the line wider by 2
				}
				while (x < t) {
					tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
					++x;                                  // move the x position by 1       
					++y;                                  // move the length or height starting position back the other way.
					w -= 2;                               // move the length or height back the other way
				}
			}       
		}

		xLoc -=d;                                   // move to next digit position
	}
}

void draw7Number90(long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
	unsigned int num = abs(n), i, s, t, w, col, h, x, y, si = 0, j = 1, d = 0;
	unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
	unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
	unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
	unsigned int x1 = cS + 1;                     // starting x location of horizontal segments
	unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
	unsigned int y1 = xLoc + x1;                  // starting y location of top side segments
	unsigned int y3 = xLoc + S3 + S4 + 1;         // starting y location of bottom side segments
	// actual x,y locations and direction of all 7 segments 
	unsigned int seg[7][2] = {{x1, xLoc}, {x2, y1}, {x2, y3 + x1}, {x1, (2 * y3) - xLoc}, {0, y3 + x1}, {0, y1}, {x1, y3}}; 
	// segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (bits 7-0)=(*,g,f,e,d,c,b,a)  
	static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 

	unsigned char c, cnt;

	c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
	if (c>10) c= 10;
	if (c<1) c = 1;

	cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
	if (cnt > 10) cnt = 10;
	if (cnt < 1) cnt = 1; 

	d = S2+(3*S3)+2;                              // width of one number

	yLoc -= (cnt-1) * d;                          // set starting x at last digit location

	while( cnt > 0) {                             // for cnt number of places

		--cnt;

		if (num > 9) i = num%10;                    // get the last digit 
		else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
		else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
		else i = num;

		num = num/10;                               // trim this digit from the number  

		for (j = 0; j < 7; ++j) {                   // draw all seven segments

			if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
			else col = bC;                            // else use background color

			if (j==0 || j==3 || j==6) {               // only three segments are horizontal

				w = S2;                                 // Starting width of segment (side)
				y = yLoc - (seg[j][0] + cS + S2 - 1);              // starting x location
				x = seg[j][1];                          // starting y location
				t = x + S3;                             // maximum thickness of segment
				h = x + S3/2;                           // half way point thickness of segment


				while (x < h) {                         // until y location = half way
					tft.drawFastVLine(x, y, w, col);      // Draw a horizontal segment top 
					++x;                                  // move the y position by 1
					--y;                                  // move the x position by -1           
					w += 2;                               // make the line wider by 2
				}

				while (x < t) {                         // finish drawing horizontal bottom  
					tft.drawFastVLine(x, y, w, col);      // Draw Horizonal segment bottom
					++x;                                  // keep moving the x draw position until t 
					++y;                                  // move the length or height starting position back the other way.
					w -= 2;                               // move the length or height back the other way
				}

			} else {

				w = S4;                                 // Starting height of segment (side)
				y = yLoc - seg[j][0];                   // starting y location
				x = seg[j][1] + cS;                     // starting x location 
				t = y - S3;                             // maximum thickness of segment
				h = y - S3/2;                           // half way point thickness of segment

				while (y > h) {                         // until y location = half way
					tft.drawFastHLine(x, y, w, col);      // Draw a vertical line right side
					--x;                                  // move the x position by 1
					--y;                                  // move the y position by 1          
					w += 2;                               // make the line wider by 2
				}
				while (y > t) {
					tft.drawFastHLine(x, y, w, col);      // Draw a vertical line right side
					++x;                                  // move the x position back the other way        
					--y;                                  // move the y position by 1.
					w -= 2;                               // make line narrower by 2
				}
			}       
		}

		yLoc +=d;                                   // move to next digit position
	}
}

void draw7Number180(long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
	unsigned int num = abs(n), i, s, t, w, col, h, x, y, si = 0, j = 1, d = 0;
	unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
	unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
	unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
	unsigned int x1 = cS;                         // starting x location of horizontal segments
	unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
	unsigned int y1 = yLoc - x1;                  // starting y location of top side segments
	unsigned int y3 = yLoc - S3 - S4 - 1;         // starting y location of bottom side segments
	// actual x,y locations and direction of all 7 segments 
	unsigned int seg[7][2] = {{x1, yLoc}, {x2, y1}, {x2, y3 - x1}, {x1, (2 * y3) - yLoc}, {0, y3 - x1}, {0, y1}, {x1, y3}}; 
	// segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (bits 7-0)=(*,g,f,e,d,c,b,a)  
	static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 
	unsigned char c, cnt;

	c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
	if (c>10) c= 10;
	if (c<1) c = 1;

	cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
	if (cnt > 10) cnt = 10;
	if (cnt < 1) cnt = 1; 

	d = S2+(3*S3)+2;                              // width of one number

	xLoc -= (cnt-1) * d;                          // set starting x at last digit location

	while( cnt > 0) {                             // for cnt number of places

		--cnt;

		if (num > 9) i = num%10;                    // get the last digit 
		else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
		else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
		else i = num;

		num = num/10;                               // trim this digit from the number  

		for (j = 0; j < 7; ++j) {                   // draw all seven segments

			if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
			else col = bC;                            // else use background color

			if (j==0 || j==3 || j==6) {               // only three segments are horizontal

				w = S2;                                 // Starting width of segment (side)
				x = xLoc - (seg[j][0] + cS + S2);       // starting x location
				y = seg[j][1];                          // starting y location
				t = y - S3;                             // maximum thickness of segment
				h = y - S3/2;                           // half way point thickness of segment


				while (y > h) {                         // until y location = half way
					tft.drawFastHLine(x, y, w, col);      // Draw a horizontal segment top 
					--y;                                  // move the y position by 1
					--x;                                  // move the x position by -1           
					w += 2;                               // make the line wider by 2
				}

				while (y > t) {                         // finish drawing horizontal bottom  
					tft.drawFastHLine(x, y, w, col);      // Draw Horizonal segment bottom
					--y;                                  // keep moving the y draw position until t 
					++x;                                  // move the length or height starting position back the other way.
					w -= 2;                               // move the length or height back the other way
				}

			} else {

				w = S4;                                 // Starting height of segment (side)
				y = seg[j][1] - cS - S4;                // starting y location
				x = xLoc - seg[j][0];                   // starting x location 
				t = x - S3;                             // maximum thickness of segment
				h = x - S3/2;                           // half way point thickness of segment

				while (x > h) {                         // until y location = half way
					tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
					--x;                                  // move the x position by 1
					--y;                                  // move the y position by 1          
					w += 2;                               // make the line wider by 2
				}
				while (x > t) {
					tft.drawFastVLine(x, y, w, col);      // Draw a vertical line right side
					--x;                                  // move the x position back the other way        
					++y;                                  // move the y position by 1.
					w -= 2;                               // make the line narrower by 2
				}
			}       
		}

		xLoc +=d;                                   // move to next digit position
	}
}

void draw7Number270(long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD) {
	unsigned int num = abs(n), i, s, t, w, col, h, x, y, si = 0, j = 1, d = 0;
	unsigned int S2 = 5 * cS;                     // width of horizontal segments   5 times the cS
	unsigned int S3 = 2 * cS;                     // thickness of a segment 2 times the cs
	unsigned int S4 = 7 * cS;                     // height of vertical segments 7 times the cS
	unsigned int x1 = cS + 1;                     // starting x location of horizontal segments
	unsigned int x2 = S3 + S2 + 1;                // starting x location of right side segments
	unsigned int y1 = xLoc - x1;                  // starting y location of top side segments
	unsigned int y3 = xLoc - S3 - S4 - 1;         // starting y location of bottom side segments
	// actual x,y locations and direction of all 7 segments 
	unsigned int seg[7][2] = {{x1, xLoc}, {x2, y1}, {x2, y3 - x1}, {x1, (2 * y3) - xLoc}, {0, y3 - x1}, {0, y1}, {x1, y3}}; 
	// segment defintions for all 10 numbers plus blank and minus sign bit mapped as typical segment labels (bits 7-0)=(*,g,f,e,d,c,b,a)  
	static const unsigned char nums[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x40}; 

	unsigned char c, cnt;

	c = abs(cS);                                  // get character size between 1 and 10 ignoring sign
	if (c>10) c= 10;
	if (c<1) c = 1;

	cnt = abs(nD);                                // get number of digits between 1 and 10 ignoring sign
	if (cnt > 10) cnt = 10;
	if (cnt < 1) cnt = 1; 

	d = S2+(3*S3)+2;                              // width of one number

	yLoc += (cnt-1) * d;                          // set starting x at last digit location

	while( cnt > 0) {                             // for cnt number of places

		--cnt;

		if (num > 9) i = num%10;                    // get the last digit 
		else if (!cnt && n<0) i = 11;               // show minus sign if 1st position and negative number
		else if (nD < 0 && !num) i = 10;            // show blanks if remaining number is zero
		else i = num;

		num = num/10;                               // trim this digit from the number  

		for (j = 0; j < 7; ++j) {                   // draw all seven segments

			if (nums[i] & (1 << j)) col = fC;         // if segment is On use foreground color
			else col = bC;                            // else use background color

			if (j==0 || j==3 || j==6) {               // only three segments are horizontal

				w = S2;                                 // Starting width of segment (side)
				y = yLoc + seg[j][0] + cS;              // starting x location
				x = seg[j][1];                          // starting y location
				t = x - S3;                             // maximum thickness of segment
				h = x - S3/2;                           // half way point thickness of segment


				while (x > h) {                         // until y location = half way
					tft.drawFastVLine(x, y, w, col);      // Draw a horizontal segment top 
					--x;                                  // move the y position by 1
					--y;                                  // move the x position by -1           
					w += 2;                               // make the line wider by 2
				}

				while (x > t) {                         // finish drawing horizontal bottom  
					tft.drawFastVLine(x, y, w, col);      // Draw Horizonal segment bottom
					--x;                                  // keep moving the x draw position until t 
					++y;                                  // move the length or height starting position back the other way.
					w -= 2;                               // move the length or height back the other way
				}

			} else {

				w = S4;                                 // Starting height of segment (side)
				y = yLoc + seg[j][0];                   // starting y location
				x = seg[j][1] - cS - S4 + 1;            // starting x location 
				t = y + S3;                             // maximum thickness of segment
				h = y + S3/2;                           // half way point thickness of segment

				while (y < h) {                         // until y location = half way
					tft.drawFastHLine(x, y, w, col);      // Draw a vertical line right side
					--x;                                  // move the x position by 1
					++y;                                  // move the y position by 1          
					w += 2;                               // make the line wider by 2
				}
				while (y < t) {
					tft.drawFastHLine(x, y, w, col);      // Draw a vertical line right side
					++x;                                  // move the x position back the other way        
					++y;                                  // move the y position by 1.
					w -= 2;                               // make line narrower by 2
				}
			}       
		}

		yLoc -=d;                                   // move to next digit position
	}
}




