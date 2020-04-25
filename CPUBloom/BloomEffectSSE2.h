#pragma once
#include <fvec.h>

class BloomEffectSSE2
{
public:
	BloomEffectSSE2(float bloomIntensity, float bloomSaturation, float baseIntensity, float baseSaturation);
	~BloomEffectSSE2(void);

	/// <summary>Intensity of the bloom image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>4</maxValue>
	/// <defaultValue>1</defaultValue>
	F32vec4 BloomIntensity;

	/// <summary>Saturation of the bloom image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>10</maxValue>
	/// <defaultValue>1</defaultValue>
	F32vec4 BloomSaturation;

	/// <summary>Intensity of the base image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>4</maxValue>
	/// <defaultValue>1</defaultValue>
	F32vec4 BaseIntensity;

	/// <summary>Saturation of the base image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>10</maxValue>
	/// <defaultValue>1</defaultValue>
	F32vec4 BaseSaturation;

	float dot(const F32vec4& a, const F32vec4& b);
	void AdjustSaturation(F32vec4& color, const F32vec4& saturation);
	void lerp(const F32vec4& grey, const F32vec4& color, const F32vec4& saturation, F32vec4& dest);
	F32vec4 saturate(F32vec4& v);
	void ComputeBloom(F32vec4& color, F32vec4& dest);
	UINT ComputeBloomInt(UINT color);
};

