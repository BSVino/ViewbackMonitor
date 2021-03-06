uniform sampler2D iDiffuse;
uniform vec4 vecColor;

uniform float flAlpha;

uniform bool bScissor;
uniform vec4 vecScissor;

in vec3 vecFragmentPosition;
in vec2 vecFragmentTexCoord0;

vec4 fragment_program()
{
	vec4 vecDiffuse = vecColor * (vec4(1.0, 1.0, 1.0, texture(iDiffuse, vecFragmentTexCoord0).a));

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
			vecDiffuse += vec4(0.5, 0.5, 0.0, 0.5);
		}*/
	}

	return vecDiffuse;
}
