uniform vec4 vecColor;

in vec3 vecPosition;
in vec3 vecNormal;
in vec2 vecTexCoord0;
in vec3 vecVertexColor;

out vec3 vecFragmentPosition;
out vec3 vecFragmentNormal;
out vec2 vecFragmentTexCoord0;
out vec3 vecFragmentColor;

vec4 vertex_program()
{
	vecFragmentPosition = (mGlobal * vec4(vecPosition, 1.0)).xyz;
	vecFragmentNormal = vecNormal;
	vecFragmentTexCoord0 = vec2(vecTexCoord0.x, 1.0-vecTexCoord0.y);
	vecFragmentColor = vecVertexColor;

	return mProjection * mView * mGlobal * vec4(vecPosition, 1.0);
}
