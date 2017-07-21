#include <mtlView.c>

static BOOL pp_isBloomEnabled = false;

MATERIAL *PPThresholdLuminanceMaterial =
{
	effect = "shaders/ThresholdLum.fx";
	flags = AUTORELOAD;
}

void pp_bloom(float threshold, float strength)
{
	PPThresholdLuminanceMaterial.skill1 = threshold;
	
	if(!pp_isBloomEnabled)
	{
		pp_add(PPThresholdLuminanceMaterial);
		pp_isBloomEnabled = true;
	}
}
