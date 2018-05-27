#include "../Color.h"

using namespace LepusEngine;
using namespace Lepus3D;

Color::Color()
{
	mVec.x = mVec.y = mVec.z = mVec.w = 0.0f;
}

Color::Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : Color()
{
	mVec.x = (float)R / 255.0f;
	mVec.y = (float)G / 255.0f;
	mVec.z = (float)B / 255.0f;
	mVec.w = (float)A / 255.0f;
}

Vector4 Color::GetVector4()
{
	return mVec;
}

Vector3 Color::GetVector3()
{
	return Vector3(mVec.x, mVec.y, mVec.z);
}

void Color::SetR(unsigned char R)
{
	mVec.x = (float)R / 255.0f;
}

void Color::SetG(unsigned char G)
{
	mVec.y = (float)G / 255.0f;
}

void Color::SetB(unsigned char B)
{
	mVec.z = (float)B / 255.0f;
}

void Color::SetA(unsigned char A)
{
	mVec.w = (float)A / 255.0f;
}
