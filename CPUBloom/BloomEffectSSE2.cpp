#include "stdafx.h"
#include "BloomEffectSSE2.h"

BloomEffectSSE2::BloomEffectSSE2(float bloomIntensity, float bloomSaturation, float baseIntensity, float baseSaturation)
	: BloomIntensity(bloomIntensity)
	, BloomSaturation(bloomSaturation)
	, BaseIntensity(baseIntensity)
	, BaseSaturation(baseSaturation)
{
}


BloomEffectSSE2::~BloomEffectSSE2(void)
{
}

float BloomEffectSSE2::dot(const F32vec4& a, const F32vec4& b)
{
	F32vec4 c = a * b;
	return c[1] + c[2] + c[3];
}

void BloomEffectSSE2::AdjustSaturation(F32vec4& color, const F32vec4& saturation)
{
	F32vec4 greyweight( 0.11f, 0.59f, 0.3f, 0.0f );
	F32vec4 grey(dot(color, greyweight));
	F32vec4 dest(0.0f);
	lerp(grey, color, saturation, dest);

	color = dest;
}

void BloomEffectSSE2::lerp(const F32vec4& grey, const F32vec4& color, const F32vec4& saturation, F32vec4& dest)
{
	dest = grey + saturation * (color - grey);
}

F32vec4 BloomEffectSSE2::saturate(F32vec4& v)
{
	static const F32vec4 kMax(1.0f);
	static const F32vec4 kMin(0.0f);

	F32vec4 res = v;

	res = simd_max(res, kMin);
	res = simd_min(res, kMax);

	return res;
}

void BloomEffectSSE2::ComputeBloom(F32vec4& color, F32vec4& dest)
{
	static const F32vec4 kMax(1.0f);
	static const F32vec4 BloomThreshold(0.25f);

	F32vec4 alpha(color[0]);
	F32vec4 base = color / alpha; // color[0] is alpha

	F32vec4 bloom = saturate((base - BloomThreshold) / (kMax - BloomThreshold));

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
}

UINT BloomEffectSSE2::ComputeBloomInt(UINT color)
{
	F32vec4 colorf;
	F32vec4 destf(0.0f);

	colorf[0] = ((color & 0xFF000000) >> 24);
	colorf[1] = ((color & 0xFF0000) >> 16);
	colorf[2] = ((color & 0xFF00) >> 8);
	colorf[3] = (color & 0xFF);

	colorf /= F32vec4(255.0f);

	ComputeBloom(colorf, destf);
	destf = saturate(destf);

	BYTE destA = destf[0] * 255;
	BYTE destR = destf[1] * 255;
	BYTE destG = destf[2] * 255;
	BYTE destB = destf[3] * 255;

	UINT dest = destA << 24 | destR << 16 | destG << 8 | destB;

	return dest;
}
