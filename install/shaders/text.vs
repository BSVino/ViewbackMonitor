in vec3 vecPosition;
in vec2 vecTexCoord0;

out vec3 vecFragmentPosition;
out vec2 vecFragmentTexCoord0;

vec4 vertex_program()
{
	vecFragmentPosition = vecPosition;

	vecFragmentTexCoord0 = vecTexCoord0;

	return mProjection * mView * mGlobal * vec4(vecPosition, 1);
}
