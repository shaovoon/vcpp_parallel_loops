#include "stdafx.h"
#include "BloomEffectAVX.h"

BloomEffectAVX::BloomEffectAVX(float bloomIntensity, float bloomSaturation, float baseIntensity, float baseSaturation)
	: BloomIntensity(bloomIntensity)
	, BloomSaturation(bloomSaturation)
	, BaseIntensity(baseIntensity)
	, BaseSaturation(baseSaturation)
{
}


BloomEffectAVX::~BloomEffectAVX(void)
{
}

std::pair<float, float> BloomEffectAVX::dot(const F32vec8& a, const F32vec8& b)
{
	F32vec8 c = a * b;
	return std::make_pair(c[1] + c[2] + c[3], c[5] + c[6] + c[7]);
}

void BloomEffectAVX::AdjustSaturation(F32vec8& color, const F32vec8& saturation)
{
	F32vec8 greyweight(0.11f, 0.59f, 0.3f, 0.0f, 0.11f, 0.59f, 0.3f, 0.0f);
	std::pair<float, float> grey2 = dot(color, greyweight);
	F32vec8 grey(grey2.second, grey2.second, grey2.second, grey2.second, grey2.first, grey2.first, grey2.first, grey2.first);
	F32vec8 dest(0.0f);
	lerp(grey, color, saturation, dest);

	color = dest;
}

void BloomEffectAVX::lerp(const F32vec8& grey, const F32vec8& color, const F32vec8& saturation, F32vec8& dest)
{
	dest = grey + saturation * (color - grey);
}

F32vec8 BloomEffectAVX::saturate(F32vec8& v)
{
	static const F32vec8 kMax(1.0f);
	static const F32vec8 kMin(0.0f);

	F32vec8 res = v;

	res = simd_max(res, kMin);
	res = simd_min(res, kMax);

	return res;
}

void BloomEffectAVX::ComputeBloom(F32vec8& color, F32vec8& dest)
{
	static const F32vec8 kMax(1.0f);
	static const F32vec8 BloomThreshold(0.25f);

	F32vec8 alpha(color[4], color[4], color[4], color[4], color[0], color[0], color[0], color[0]);
	F32vec8 base = color / alpha; // color[0] is alpha

	F32vec8 bloom = saturate((base - BloomThreshold) / (kMax - BloomThreshold));

	// Adjust color saturation and intensity.
	AdjustSaturation(bloom, BloomSaturation);
	AdjustSaturation(base, BaseSaturation);

	bloom = bloom * BloomIntensity;

	base = base * BaseIntensity;

	// Darken down the base image in areas where there is a lot of bloom,
	// to prevent things looking excessively burned-out.
	base *= (kMax - saturate(bloom));

	// Combine the two images.
	dest = (base + bloom) * alpha;
	dest[0] = alpha[0];
	dest[4] = alpha[4];
}

std::pair<UINT, UINT> BloomEffectAVX::ComputeBloomInt(UINT color, UINT color2)
{
	F32vec8 colorf;
	F32vec8 destf(0.0f);

	colorf[0] = ((color & 0xFF000000) >> 24);
	colorf[1] = ((color & 0xFF0000) >> 16);
	colorf[2] = ((color & 0xFF00) >> 8);
	colorf[3] = (color & 0xFF);
	colorf[4] = ((color2 & 0xFF000000) >> 24);
	colorf[5] = ((color2 & 0xFF0000) >> 16);
	colorf[6] = ((color2 & 0xFF00) >> 8);
	colorf[7] = (color2 & 0xFF);

	colorf /= F32vec8(255.0f);

	ComputeBloom(colorf, destf);
	destf = saturate(destf);

	BYTE destA = destf[0] * 255;
	BYTE destR = destf[1] * 255;
	BYTE destG = destf[2] * 255;
	BYTE destB = destf[3] * 255;

	BYTE destA2 = destf[4] * 255;
	BYTE destR2 = destf[5] * 255;
	BYTE destG2 = destf[6] * 255;
	BYTE destB2 = destf[7] * 255;

	UINT dest = destA << 24 | destR << 16 | destG << 8 | destB;
	UINT dest2 = destA2 << 24 | destR2 << 16 | destG2 << 8 | destB2;

	return std::make_pair(dest, dest2);
}
