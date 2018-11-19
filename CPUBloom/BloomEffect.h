#pragma once
class BloomEffect
{
public:
	BloomEffect(float BloomIntensity, float BloomSaturation, float BaseIntensity, float BaseSaturation);
	~BloomEffect(void);

	/// <summary>Intensity of the bloom image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>4</maxValue>
	/// <defaultValue>1</defaultValue>
	float BloomIntensity;

	/// <summary>Saturation of the bloom image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>10</maxValue>
	/// <defaultValue>1</defaultValue>
	float BloomSaturation;

	/// <summary>Intensity of the base image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>4</maxValue>
	/// <defaultValue>1</defaultValue>
	float BaseIntensity;

	/// <summary>Saturation of the base image.</summary>
	/// <minValue>0</minValue>
	/// <maxValue>10</maxValue>
	/// <defaultValue>1</defaultValue>
	float BaseSaturation;

	float dot(float a[3], float b[3]);
	void AdjustSaturation(float color[3], float saturation);
	void lerp(float grey, float color[3], float saturation, float dest[3]);
	float saturate(float a);
	void ComputeBloom(float color[4], float dest[4]);
	UINT ComputeBloomInt(UINT color);
};

