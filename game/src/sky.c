#ifndef sky_c
#define sky_c

#include "sky.h"

COLOR* getRGB (var hue, var brightness, var saturation)
{
	COLOR vecRGB;
	
	var b = clamp(brightness, 0 , 100); // brightness %
	var s = clamp(saturation, 0 , 100); // saturation %
	var h = clamp(hue, 0, 359); //hue ° 
	
	// scale values
	b /= 100.0;
	s /= 100.0;
	h /= 60.0; // 6 segments 60 deg each
	
	switch (integer(h))
	{
		case 0:
			vecRGB.red   = b;
			vecRGB.blue  = b * (1 - s);
			vecRGB.green = vecRGB.blue + (b - vecRGB.blue) * h;
			break;

		case 1:
			vecRGB.green = b;
			vecRGB.blue  = b * (1 - s);
			vecRGB.red   = vecRGB.green - (b - vecRGB.blue) * (h - 1);
			break;

		case 2:
			vecRGB.green = b;
			vecRGB.red   = b * (1 - s);
			vecRGB.blue  = vecRGB.red + (b - vecRGB.red) * (h - 2);
			break;

		case 3:
			vecRGB.blue  = b;
			vecRGB.red   = b * (1 - s);
			vecRGB.green = vecRGB.blue - (b - vecRGB.red) * (h - 3);
			break;

		case 4:
			vecRGB.blue  = b;
			vecRGB.green = b * (1 - s);
			vecRGB.red   = vecRGB.green + (b - vecRGB.green) * (h - 4);
			break;

		case 5:
			vecRGB.red   = b;
			vecRGB.green = b * (1 - s);
			vecRGB.blue  = vecRGB.red - (b - vecRGB.green) * (h - 5);
			break;

		default:
			vecRGB.blue  = b;
			vecRGB.green = b;
			vecRGB.red   = b;
			break;

		
	}
	vec_scale(&vecRGB, 255);
	
	return &vecRGB;
}


void skychange (float s)
{
	COLOR oldColor, newColor;
	var factor = 0.0;
	var hue = 0;
	
	vec_set(oldColor.blue, screen_color.blue);
//	vec_set(newColor.blue, vector(random(255), random(255), random(255)));
	vec_set (newColor.blue, getRGB(hue, 100, 100));	
	sky_active = 1; //fix by firo
	while (sky_active == 1) //fix by firo
	{

		factor += s * 0.2 * time_step;
		factor = clamp(factor, 0.0, 1.0);
		
		vec_lerp(screen_color.blue, oldColor.blue, newColor.blue, factor);
		vec_set(sky_color.blue, screen_color.blue);
		
		if (factor == 1.0)
		{
			factor = 0.0;
			vec_set(oldColor.blue, screen_color.blue);
//			vec_set(newColor.blue, vector(random(255), random(255), random(255)) );
			hue = cycle(hue + 50 + random(30), 0, 359);
			vec_set (newColor.blue, getRGB(hue, 100, 100));	
		}	
		wait(1);
	}
	vec_set(screen_color.blue, vector(0,0,0)); //fix by firo
	vec_set(sky_color.blue, screen_color.blue); //fix by firo
}

void skychange ()
{
	sky_active = 0;
	wait (1);
	skychange(1);
}

#endif /* sky_c */