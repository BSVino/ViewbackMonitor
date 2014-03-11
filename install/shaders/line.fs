uniform vec4 vecColor = vec4(1.0, 1.0, 1.0, 1.0);

uniform float flAlpha;

in vec4 vecFragmentColor;

void main()
{
	vec4 vecDiffuse = vecColor * vecFragmentColor;

	vecOutputColor = vecDiffuse;
}
