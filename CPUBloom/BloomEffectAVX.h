#pragma once
#include <dvec.h>
#include <map>
class BloomEffectAVX
{
public:
	BloomEffectAVX(float bloomIntensity, float bloomSaturation, float baseIntensity, float baseSaturation);
	~BloomEffectAVX(void);

	/// <summary>Intensity of the bloom image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>4</maxValue>
	/// <defaultValue>1</defaultValue>
	F32vec8 BloomIntensity;

	/// <summary>Saturation of the bloom image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>10</maxValue>
	/// <defaultValue>1</defaultValue>
	F32vec8 BloomSaturation;

	/// <summary>Intensity of the base image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>4</maxValue>
	/// <defaultValue>1</defaultValue>
	F32vec8 BaseIntensity;

	/// <summary>Saturation of the base image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>10</maxValue>
	/// <defaultValue>1</defaultValue>
	F32vec8 BaseSaturation;

	std::pair<float, float> dot(const F32vec8& a, const F32vec8& b);
	void AdjustSaturation(F32vec8& color, const F32vec8& saturation);
	void lerp(const F32vec8& grey, const F32vec8& color, const F32vec8& saturation, F32vec8& dest);
	F32vec8 saturate(F32vec8& v);
	void ComputeBloom(F32vec8& color, F32vec8& dest);
	std::pair<UINT, UINT> ComputeBloomInt(UINT color, UINT color2);
};

