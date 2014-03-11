in vec3 vecPosition;
in vec4 vecVertexColor;

out vec4 vecFragmentColor;

void main()
{
	gl_Position = mProjection * mGlobal * vec4(vecPosition, 1);

	vecFragmentColor = vecVertexColor;
}
