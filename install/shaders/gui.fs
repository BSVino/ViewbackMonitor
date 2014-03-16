uniform vec4 vecDimensions;

uniform bool bDiffuse;
uniform sampler2D iDiffuse;
uniform vec4 vecColor;

uniform bool bScissor;
uniform vec4 vecScissor;

uniform float flBorder;
uniform bool bHighlight;

in vec3 vecFragmentPosition;
in vec2 vecFragmentTexCoord0;
in vec3 vecFragmentNormal;
in vec3 vecFragmentColor;

vec4 fragment_program()
{
	vec4 vecDiffuse = vecColor;

	if (bDiffuse)
		vecDiffuse *= texture(iDiffuse, vecFragmentTexCoord0);

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

	vec4 vecFragColor = vecDiffuse;

	if (vecColor.a > 0.0 && bHighlight)
	{
		float y = vecDimensions.y;
		float m = vecDimensions.x + vecDimensions.z/2.0;	// Midpoint

		float flDistance = abs(y - vecFragmentPosition.y);
		if (flDistance < 250.0)
		{
			float flAdd = RemapVal(flDistance, 0.0, 250.0, 0.1, 0.01);
			float flGlow = RemapVal(LengthSqr(vecFragmentPosition.xy - vec2(m, y)), 0.0, 300.0*300.0, 0.08, 0.0);
			if (flGlow > 0.0)
				flAdd += flGlow;
			vecFragColor.r += flAdd;
			vecFragColor.g += flAdd;
			vecFragColor.b += flAdd;
		}
	}

	return vecFragColor;
}
