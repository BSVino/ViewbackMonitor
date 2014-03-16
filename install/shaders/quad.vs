in vec3 vecPosition;
in vec2 vecTexCoord0;

out vec2 vecFragmentTexCoord0;

vec4 vertex_program()
{
	vecFragmentTexCoord0 = vec2(vecTexCoord0.x, 1.0-vecTexCoord0.y);

	return vec4(vecPosition, 1.0);
}
