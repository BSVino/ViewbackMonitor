in vec3 vecPosition;
in vec4 vecVertexColor;

out vec4 vecFragmentColor;

vec4 vertex_program()
{
	vecFragmentColor = vecVertexColor;

	return mProjection * mGlobal * vec4(vecPosition, 1);
}
