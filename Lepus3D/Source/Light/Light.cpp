#include "../Light.h"
#include "../../../LepusEngine/Source/Logger.h"

using namespace LepusEngine;
using namespace Lepus3D;

Light::Light()
{
	mIntensity = 1.0f;
	mCol.SetR((unsigned char)255);
	mCol.SetG((unsigned char)255);
	mCol.SetB((unsigned char)255);
	mCol.SetA((unsigned char)255);
}

Light::Light(Vector3 position, Color color, float intensity)
{
	mIntensity = intensity;

	mCol.SetR(color.GetVector4().x);
	mCol.SetG(color.GetVector4().y);
	mCol.SetB(color.GetVector4().z);
	mCol.SetA(color.GetVector4().w);
	mTransform.SetPosition(position);
}

Vector3 Light::GetPosition()
{
	return mTransform.GetPosition();
}

Color Light::GetColor()
{
	return mCol;
}

float Light::GetIntensity()
{
	return mIntensity;
}

void Light::SetPosition(Vector3 pos)
{
	mTransform.SetPosition(pos);
}

void Light::SetColor(Color col)
{
	mCol = col;
}

void Light::SetIntensity(float intensity)
{
	mIntensity = intensity;
}
