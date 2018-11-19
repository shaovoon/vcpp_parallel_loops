#include "StdAfx.h"
#include "BloomEffect.h"


BloomEffect::BloomEffect(float BloomIntensity, float BloomSaturation, float BaseIntensity, float BaseSaturation)
{
	this->BloomIntensity=BloomIntensity;
	this->BloomSaturation=BloomSaturation;
	this->BaseIntensity=BaseIntensity;
	this->BaseSaturation=BaseSaturation;
}


BloomEffect::~BloomEffect(void)
{
}

float BloomEffect::dot(float a[3], float b[3])
{
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

void BloomEffect::AdjustSaturation(float color[3], float saturation)
{
	float greyweight[3] = {0.3f, 0.59f, 0.11f};
	float grey = dot(color, greyweight);
	float dest[3] = {0.0f,0.0f,0.0f};
	lerp(grey, color, saturation, dest);

	color[0] = dest[0];
	color[1] = dest[1];
	color[2] = dest[2];
}

void BloomEffect::lerp(float grey, float color[3], float saturation, float dest[3])
{
	//dest[0] = (color[0] - grey) * saturation;
	//dest[1] = (color[1] - grey) * saturation;
	//dest[2] = (color[2] - grey) * saturation;

	//dest[0] += grey; 
	//dest[1] += grey; 
	//dest[2] += grey; 

	// y= y0 + ((x-x0)*((y1-y0)/(x1-x0)))
	// x + s(y-x)
	dest[0] = grey + saturation * (color[0]-grey); 
	dest[1] = grey + saturation * (color[1]-grey); 
	dest[2] = grey + saturation * (color[2]-grey); 

}

float BloomEffect::saturate(float a)
{
	if(a>1.0f)
		a = 1.0f;
	if(a<0.0f)
		a = 0.0f;

	return a;
}

void BloomEffect::ComputeBloom(float color[4], float dest[4])
{
	float BloomThreshold = 0.25f;

	float base[3];
	base[0] = color[1] / color[0]; // color[0] is alpha
	base[1] = color[2] / color[0]; // color[0] is alpha
	base[2] = color[3] / color[0]; // color[0] is alpha

	//float3 bloom = saturate((base - BloomThreshold) / (1 - BloomThreshold));

	float bloom[3];
	bloom[0] = saturate((base[0] - BloomThreshold) / (1.0f - BloomThreshold));
	bloom[1] = saturate((base[1] - BloomThreshold) / (1.0f - BloomThreshold));
	bloom[2] = saturate((base[2] - BloomThreshold) / (1.0f - BloomThreshold));

	// Adjust color saturation and intensity.
	AdjustSaturation(bloom, BloomSaturation);
	AdjustSaturation(base, BaseSaturation);

	bloom[0] = bloom[0] * BloomIntensity;
	bloom[1] = bloom[1] * BloomIntensity;
	bloom[2] = bloom[2] * BloomIntensity;

	base[0] = base[0] * BaseIntensity;
	base[1] = base[1] * BaseIntensity;
	base[2] = base[2] * BaseIntensity;

	// Darken down the base image in areas where there is a lot of bloom,
	// to prevent things looking excessively burned-out.
	base[0] *= (1.0f - saturate(bloom[0]));
	base[1] *= (1.0f - saturate(bloom[1]));
	base[2] *= (1.0f - saturate(bloom[2]));

	// Combine the two images.
	dest[1] = (base[0] + bloom[0]) * color[0];
	dest[2] = (base[1] + bloom[1]) * color[0];
	dest[3] = (base[2] + bloom[2]) * color[0];
	dest[0] = color[0];

}

UINT BloomEffect::ComputeBloomInt(UINT color)
{
	float colorf[4];
	float destf[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	colorf[0] = ((color & 0xFF000000)>> 24) / 255.0f;
	colorf[1] = ((color & 0xFF0000) >> 16) / 255.0f;
	colorf[2] = ((color & 0xFF00) >> 8) / 255.0f;
	colorf[3] = (color & 0xFF) / 255.0f;

	ComputeBloom(colorf, destf);

	BYTE destA = saturate(destf[0]) * 255;
	BYTE destR = saturate(destf[1]) * 255;
	BYTE destG = saturate(destf[2]) * 255;
	BYTE destB = saturate(destf[3]) * 255;

	UINT dest = destA << 24 | destR << 16 | destG << 8 | destB;

	return dest;
}
