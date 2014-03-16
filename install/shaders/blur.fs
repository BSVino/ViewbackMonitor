uniform sampler2D iSource;
uniform float aflCoefficients[3];
uniform float flOffsetX;
uniform float flOffsetY;

in vec2 vecFragmentTexCoord0;

vec4 fragment_program()
{
	vec2 vecTC = vecFragmentTexCoord0;
	vec2 vecOffset = vec2(flOffsetX, flOffsetY);

	vec4 vecColorSum;
	vecColorSum  = aflCoefficients[0] * texture(iSource, vecTC - vecOffset);
	vecColorSum += aflCoefficients[1] * texture(iSource, vecTC);
	vecColorSum += aflCoefficients[2] * texture(iSource, vecTC + vecOffset);

	return vecColorSum;
}
