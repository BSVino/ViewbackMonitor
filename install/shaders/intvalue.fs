uniform vec4 vecDimensions;

uniform vec4 vecColor;

uniform bool bScissor;
uniform vec4 vecScissor;

uniform float flBorder;
uniform bool bHighlight;

in vec3 vecFragmentPosition;
in vec2 vecFragmentTexCoord0;
in vec3 vecFragmentColor;

vec4 fragment_program()
{
	vec4 vecDiffuse = vecColor;

	//float flLerp = clamp(vecFragmentPosition.x - vecDimensions.x, 0.0, 200.0)/200;
	float flLerp = smoothstep(0.0, 200.0, vecFragmentPosition.x - vecDimensions.x);
	float flMixed = mix(1.0, 0.1, flLerp);

	vecDiffuse.r *= flMixed;
	vecDiffuse.g *= flMixed;
	vecDiffuse.b *= flMixed;

	if (bScissor)
	{
		if (vecFragmentPosition.x < vecScissor.x)
			discard;
		if (vecFragmentPosition.y < vecScissor.y)
			discard;
		if (vecFragmentPosition.x > vecScissor.x+vecScissor.z)
			discard;
		if (vecFragmentPosition.y > vecScissor.y+vecScissor.w)
			discard;

		/*if (vecFragmentPosition.x > vecScissor.x &&
			vecFragmentPosition.y > vecScissor.y &&
			vecFragmentPosition.x < vecScissor.x+vecScissor.z &&
			vecFragmentPosition.y < vecScissor.y+vecScissor.w)
		{
			vecDiffuse += vec4(0.5, 0.5, 0.0, 0.0);
		}*/
	}

	if (flBorder > 0.0)
	{
		float x = vecDimensions.x;
		float y = vecDimensions.y;
		float w = vecDimensions.z;
		float h = vecDimensions.w;

		float flBorderStrength = 0.2;
		float flDistanceLeft = RemapVal(abs(x - vecFragmentPosition.x), 0.0, flBorder, flBorderStrength, 0.0);
		float flDistanceTop = RemapVal(abs(y - vecFragmentPosition.y), 0.0, flBorder, flBorderStrength, 0.0);
		float flDistanceRight = RemapVal(abs(x+w - vecFragmentPosition.x), 0.0, flBorder, flBorderStrength, 0.0);
		float flDistanceBottom = RemapVal(abs(y+h - vecFragmentPosition.y), 0.0, flBorder, flBorderStrength, 0.0);

		float flDistance = max(max(max(flDistanceLeft, flDistanceRight), flDistanceTop), flDistanceBottom);
		if (flDistance > 0.0)
		{
			vecDiffuse.r += flDistance;
			vecDiffuse.g += flDistance;
			vecDiffuse.b += flDistance;
			vecDiffuse.a += flDistance * 1.0/flBorderStrength;
		}
	}

	return vecDiffuse;
}
