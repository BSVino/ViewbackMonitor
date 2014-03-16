uniform vec4 vecColor;

uniform float flAlpha;

in vec4 vecFragmentColor;

vec4 fragment_program()
{
	return vecColor * vecFragmentColor;
}
