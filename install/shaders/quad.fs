uniform sampler2D iDiffuse;
uniform vec4 vecColor;

uniform float flAlpha;

in vec2 vecFragmentTexCoord0;

vec4 fragment_program()
{
	return vecColor * texture(iDiffuse, vecFragmentTexCoord0);
}
