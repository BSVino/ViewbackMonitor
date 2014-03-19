uniform vec4 vecColor;

uniform vec4 vecDimensions;

uniform bool bTexCoords;
uniform vec4 vecTexCoords;

in vec3 vecPosition;
in vec2 vecTexCoord0;

out vec3 vecFragmentPosition;
out vec3 vecFragmentNormal;
out vec2 vecFragmentTexCoord0;

vec4 vertex_program()
{
	float x = vecDimensions.x;
	float y = vecDimensions.y;
	float w = vecDimensions.z;
	float h = vecDimensions.w;

	vecFragmentPosition = vec3(x + vecPosition.x * w, y + vecPosition.y * h, 0);

	if (bTexCoords)
	{
		float tx = vecTexCoords.x;
		float ty = vecTexCoords.y;
		float tw = vecTexCoords.z;
		float th = vecTexCoords.w;

		vecFragmentTexCoord0 = vec2(tx + vecTexCoord0.x * tw, ty + vecTexCoord0.y * th);
	}
	else
		vecFragmentTexCoord0 = vecTexCoord0;

	return mProjection * mGlobal * vec4(vecFragmentPosition, 1.0);
}
