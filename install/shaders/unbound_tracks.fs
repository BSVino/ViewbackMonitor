uniform vec4 vecDimensions;

uniform bool bDiffuse;
uniform sampler2D iDiffuse;
uniform vec4 vecColor;

uniform float flSolidWidth;

in vec3 vecFragmentPosition;
in vec2 vecFragmentTexCoord0;
in vec3 vecFragmentNormal;
in vec3 vecFragmentColor;

vec4 fragment_program()
{
	vec4 vecFragColor = vecColor;

	float x = vecDimensions.x;
	float y = vecDimensions.y;
	float w = vecDimensions.z;
	float h = vecDimensions.w;

	float border = 3.5;

	float flBorderStrength = 0.1;
	float flDistanceLeft = RemapVal(abs(x - vecFragmentPosition.x), 0.0, border, flBorderStrength, 0.0);
	float flDistanceTop = RemapVal(abs(y - vecFragmentPosition.y), 0.0, border, flBorderStrength, 0.0);
	float flDistanceRight = RemapVal(abs(x+w - vecFragmentPosition.x), 0.0, border, flBorderStrength, 0.0);
	float flDistanceBottom = RemapVal(abs(y+h - vecFragmentPosition.y), 0.0, border, flBorderStrength, 0.0);

	float flDistance = max(max(max(flDistanceLeft, flDistanceRight), flDistanceTop), flDistanceBottom);

	vecFragColor.r += flDistance;
	vecFragColor.g += flDistance;
	vecFragColor.b += flDistance;
	vecFragColor.a += flDistance * 1.0/flBorderStrength;

	vecFragColor.a *= RemapValSmooth(vecFragmentPosition.x, x, x+w/2-flSolidWidth/2, 0.0, 1.0);
	vecFragColor.a *= RemapValSmooth(vecFragmentPosition.x, x+w/2+flSolidWidth/2, x+w, 1.0, 0.0);

	return vecFragColor;
}
