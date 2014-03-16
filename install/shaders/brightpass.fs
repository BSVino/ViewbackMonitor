uniform sampler2D iSource;
uniform float flBrightness;
uniform float flScale;

in vec2 vecFragmentTexCoord0;

vec4 fragment_program()
{
	vec4 vecFragmentColor = texture(iSource, vecFragmentTexCoord0);

	float flValue = vecFragmentColor.x;
	if (vecFragmentColor.y > flValue)
		flValue = vecFragmentColor.y;
	if (vecFragmentColor.z > flValue)
		flValue = vecFragmentColor.z;

	if (flValue < flBrightness && flValue > flBrightness - 0.2)
	{
		float flStrength = RemapVal(flValue, flBrightness - 0.2, flBrightness, 0.0, 1.0);
		vecFragmentColor = vecFragmentColor*flStrength;
	}
	else if (flValue < flBrightness - 0.2)
		vecFragmentColor = vec4(0.0, 0.0, 0.0, 0.0);

	vec4 vecFragColor = vecFragmentColor*flScale;
	vecFragColor.a = 1.0;

	return vecFragColor;
}
