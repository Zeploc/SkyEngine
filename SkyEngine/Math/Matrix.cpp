// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Matrix.h"

#include "Quaternion.h"
#include "Vector.h"
#include "Vector4.h"

void Matrix4::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
	M[0][0] = ScaleX;
	M[0][1] = 0.0f;
	M[0][2] = 0.0f;
	M[0][3] = 0.0f;
	M[1][0] = 0.0f;
	M[1][1] = ScaleY;
	M[1][2] = 0.0f;
	M[1][3] = 0.0f;
	M[2][0] = 0.0f;
	M[2][1] = 0.0f;
	M[2][2] = ScaleZ;
	M[2][3] = 0.0f;
	M[3][0] = 0.0f;
	M[3][1] = 0.0f;
	M[3][2] = 0.0f;
	M[3][3] = 1.0f;
}

void Matrix4::InitRotateTransform(float RotateX, float RotateY, float RotateZ)
{
	Matrix4 rx, ry, rz;

	const float x = ToRadian(RotateX);
	const float y = ToRadian(RotateY);
	const float z = ToRadian(RotateZ);

	rx.M[0][0] = 1.0f;
	rx.M[0][1] = 0.0f;
	rx.M[0][2] = 0.0f;
	rx.M[0][3] = 0.0f;
	rx.M[1][0] = 0.0f;
	rx.M[1][1] = cosf(x);
	rx.M[1][2] = -sinf(x);
	rx.M[1][3] = 0.0f;
	rx.M[2][0] = 0.0f;
	rx.M[2][1] = sinf(x);
	rx.M[2][2] = cosf(x);
	rx.M[2][3] = 0.0f;
	rx.M[3][0] = 0.0f;
	rx.M[3][1] = 0.0f;
	rx.M[3][2] = 0.0f;
	rx.M[3][3] = 1.0f;

	ry.M[0][0] = cosf(y);
	ry.M[0][1] = 0.0f;
	ry.M[0][2] = -sinf(y);
	ry.M[0][3] = 0.0f;
	ry.M[1][0] = 0.0f;
	ry.M[1][1] = 1.0f;
	ry.M[1][2] = 0.0f;
	ry.M[1][3] = 0.0f;
	ry.M[2][0] = sinf(y);
	ry.M[2][1] = 0.0f;
	ry.M[2][2] = cosf(y);
	ry.M[2][3] = 0.0f;
	ry.M[3][0] = 0.0f;
	ry.M[3][1] = 0.0f;
	ry.M[3][2] = 0.0f;
	ry.M[3][3] = 1.0f;

	rz.M[0][0] = cosf(z);
	rz.M[0][1] = -sinf(z);
	rz.M[0][2] = 0.0f;
	rz.M[0][3] = 0.0f;
	rz.M[1][0] = sinf(z);
	rz.M[1][1] = cosf(z);
	rz.M[1][2] = 0.0f;
	rz.M[1][3] = 0.0f;
	rz.M[2][0] = 0.0f;
	rz.M[2][1] = 0.0f;
	rz.M[2][2] = 1.0f;
	rz.M[2][3] = 0.0f;
	rz.M[3][0] = 0.0f;
	rz.M[3][1] = 0.0f;
	rz.M[3][2] = 0.0f;
	rz.M[3][3] = 1.0f;

	*this = rz * ry * rx;
}

void Matrix4::InitRotateTransform(const Quaternion& quat)
{
	float yy2 = 2.0f * quat.Y * quat.Y;
	float xy2 = 2.0f * quat.X * quat.Y;
	float xz2 = 2.0f * quat.X * quat.Z;
	float yz2 = 2.0f * quat.Y * quat.Z;
	float zz2 = 2.0f * quat.Z * quat.Z;
	float wz2 = 2.0f * quat.W * quat.Z;
	float wy2 = 2.0f * quat.W * quat.Y;
	float wx2 = 2.0f * quat.W * quat.X;
	float xx2 = 2.0f * quat.X * quat.X;
	M[0][0] = - yy2 - zz2 + 1.0f;
	M[0][1] = xy2 + wz2;
	M[0][2] = xz2 - wy2;
	M[0][3] = 0;
	M[1][0] = xy2 - wz2;
	M[1][1] = - xx2 - zz2 + 1.0f;
	M[1][2] = yz2 + wx2;
	M[1][3] = 0;
	M[2][0] = xz2 + wy2;
	M[2][1] = yz2 - wx2;
	M[2][2] = - xx2 - yy2 + 1.0f;
	M[2][3] = 0.0f;
	M[3][0] = M[3][1] = M[3][2] = 0;
	M[3][3] = 1.0f;
}

// TODO: Complete
void Matrix4::SetViewTranslation(const SVector Location)
{
	SVector const Forward(0,0,1);// GetForward().GetNormalized());
	SVector const Right(-1,0,0);//(Forward.Cross(GetUp())).GetNormalized());
	SVector const NewUp(0,1,0);//Right.Cross(Forward));

	M[3][0] = -Right.Dot(Location);
	M[3][1] = -NewUp.Dot(Location);
	M[3][2] = Forward.Dot(Location);
}

void Matrix4::SetTranslationTransform(const SVector Location)
{
	SetTranslationTransform(Location.X, Location.Y, Location.Z);
}

void Matrix4::SetTranslationTransform(const float X, const float Y, const float Z)
{
	M[0][3] = X;
	M[1][3] = Y;
	M[2][3] = Z;
	M[3][3] = 1.0f;	
}

void Matrix4::SetCameraOrientation(SVector Forward, SVector Up)
{
	Forward.Normalize();
	Up.Normalize();
	const SVector Right = Forward.Cross(Up);
	const SVector CorrectedUp = (Right.Cross(Forward)).GetNormalized();
	
	M[0][0] = Right.X;
	M[1][0] = Right.Y;
	M[2][0] = Right.Z;
	M[0][1] = CorrectedUp.X;
	M[1][1] = CorrectedUp.Y;
	M[2][1] = CorrectedUp.Z;
	M[0][2] = -Forward.X;
	M[1][2] = -Forward.Y;
	M[2][2] = -Forward.Z;

	// TODO: Complete
	// Update Translation
	SetViewTranslation(GetViewLocation());	
}

void Matrix4::SetLookAt(SVector Location, SVector Target, SVector Up)
{	
	SVector Forward(Target - Location);
	Forward.Normalize();
	const SVector Right((Forward.Cross(Up)).GetNormalized());
	const SVector NewUp(Right.Cross(Forward));

	M[0][0] = Right.X;
	M[1][0] = Right.Y;
	M[2][0] = Right.Z;
	M[0][1] = NewUp.X;
	M[1][1] = NewUp.Y;
	M[2][1] = NewUp.Z;
	M[0][2] = -Forward.X;
	M[1][2] = -Forward.Y;
	M[2][2] = -Forward.Z;
	M[3][0] = -Right.Dot(Location);
	M[3][1] = -NewUp.Dot(Location);
	M[3][2] = Forward.Dot(Location);
}

void Matrix4::SetPerspectiveProjection(const PersProjInfo& P)
{
	const float AspectRatio = P.Width / P.Height;
	const float TanHalfFov = tanf(P.FOV / 2.0f);
	
	InitIdentity();
	M[0][0] = 1 / (AspectRatio * TanHalfFov);
	M[1][1] = 1 / (TanHalfFov);
	M[2][3] = -1;
	
	M[2][2] = - (P.zFar + P.zNear) / (P.zFar - P.zNear);
	M[3][2] = - (2 * P.zFar * P.zNear) / (P.zFar - P.zNear);
	return;

	// glm::mat4x4 projection = glm::perspective(P.FOV, AspectRatio, P.zNear, P.zFar);
	//
	// *this = Matrix4(projection);
	// return;
	
	const float ZRange = P.zNear - P.zFar;
	// const float TanHalfFov = tanf(ToRadian(P.FOV / 2.0f));
	M[0][0] = 1.0f / (TanHalfFov * AspectRatio);
	M[0][1] = 0.0f;
	M[0][2] = 0.0f;
	M[0][3] = 0.0;
	M[1][0] = 0.0f;
	M[1][1] = 1.0f / TanHalfFov;
	M[1][2] = 0.0f;
	M[1][3] = 0.0;
	M[2][0] = 0.0f;
	M[2][1] = 0.0f;
	M[2][2] = (-P.zNear - P.zFar) / ZRange;
	M[2][3] = 2.0f * P.zFar * P.zNear / ZRange;
	M[3][0] = 0.0f;
	M[3][1] = 0.0f;
	M[3][2] = 1.0f;
	M[3][3] = 0.0;

	return;
}

void Matrix4::SetOrthographicProjection(const OrthoProjInfo& P)
{
	float l = P.Left;
	float r = P.Right;
	float b = P.Bottom;
	float t = P.Top;
	float n = P.ZNear;
	float f = P.ZFar;

	M[0][0] = 2.0f / (r - l);
	M[0][3] = -(r + l) / (r - l);
	M[1][1] = 2.0f / (t - b);
	M[1][3] = -(t + b) / (t - b);
	M[2][2] = 2.0f / (f - n);
	M[2][3] = -(f + n) / (f - n);
}

SVector4 Matrix4::operator*(const SVector4& V) const
{
	glm::vec4 Vec4 = ToGLM() * V;
	return Vec4;

	// TODO: Fix
	SVector4 Result;

	Result.X = M[0][0] * V.X + M[0][1] * V.Y + M[0][2] * V.Z + M[0][3] * V.W;
	Result.Y = M[1][0] * V.X + M[1][1] * V.Y + M[1][2] * V.Z + M[1][3] * V.W;
	Result.Z = M[2][0] * V.X + M[2][1] * V.Y + M[2][2] * V.Z + M[2][3] * V.W;
	Result.W = M[3][0] * V.X + M[3][1] * V.Y + M[3][2] * V.Z + M[3][3] * V.W;

	return Result;
}

void Matrix4::Print() const
{
	for (int i = 0; i < 4; i++)
	{
		printf("%f %f %f %f\n", M[i][0], M[i][1], M[i][2], M[i][3]);
	}
}

float Matrix4::Determinant() const
{
	return M[0][0] * M[1][1] * M[2][2] * M[3][3] - M[0][0] * M[1][1] * M[2][3] * M[3][2] + M[0][0] * M[1][2] * M[2][3] * M[3][1] - M[0][0] * M[1][2] * M[2][1] * M[3][3]
		+ M[0][0] * M[1][3] * M[2][1] * M[3][2] - M[0][0] * M[1][3] * M[2][2] * M[3][1] - M[0][1] * M[1][2] * M[2][3] * M[3][0] + M[0][1] * M[1][2] * M[2][0] * M[3][3]
		- M[0][1] * M[1][3] * M[2][0] * M[3][2] + M[0][1] * M[1][3] * M[2][2] * M[3][0] - M[0][1] * M[1][0] * M[2][2] * M[3][3] + M[0][1] * M[1][0] * M[2][3] * M[3][2]
		+ M[0][2] * M[1][3] * M[2][0] * M[3][1] - M[0][2] * M[1][3] * M[2][1] * M[3][0] + M[0][2] * M[1][0] * M[2][1] * M[3][3] - M[0][2] * M[1][0] * M[2][3] * M[3][1]
		+ M[0][2] * M[1][1] * M[2][3] * M[3][0] - M[0][2] * M[1][1] * M[2][0] * M[3][3] - M[0][3] * M[1][0] * M[2][1] * M[3][2] + M[0][3] * M[1][0] * M[2][2] * M[3][1]
		- M[0][3] * M[1][1] * M[2][2] * M[3][0] + M[0][3] * M[1][1] * M[2][0] * M[3][2] - M[0][3] * M[1][2] * M[2][0] * M[3][1] + M[0][3] * M[1][2] * M[2][1] * M[3][0];
}

Matrix4& Matrix4::Inverse()
{
	*this = GetInverse();
	return *this;
}

Matrix4 Matrix4::GetInverse()
{
	// Compute the reciprocal determinant
	float det = Determinant();
	if (det == 0.0f)
	{
		// Matrix not invertible. Setting all elements to nan is not really
		// correct in a mathematical sense but it is easy to debug for the
		// programmer.
		/*const float nan = std::numeric_limits<float>::quiet_NaN();
		*this = Matrix4(
			nan,nan,nan,nan,
			nan,nan,nan,nan,
			nan,nan,nan,nan,
			nan,nan,nan,nan);*/
		ensure(0, "Matrix function inverse doesn't exist");
		return *this;
	}

	float invdet = 1.0f / det;

	Matrix4 res;
	res.M[0][0] = invdet * (M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) + M[1][2] * (M[2][3] * M[3][1] - M[2][1] * M[3][3]) + M[1][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]));
	res.M[0][1] = -invdet * (M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) + M[0][2] * (M[2][3] * M[3][1] - M[2][1] * M[3][3]) + M[0][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]));
	res.M[0][2] = invdet * (M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) + M[0][2] * (M[1][3] * M[3][1] - M[1][1] * M[3][3]) + M[0][3] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]));
	res.M[0][3] = -invdet * (M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) + M[0][2] * (M[1][3] * M[2][1] - M[1][1] * M[2][3]) + M[0][3] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]));
	res.M[1][0] = -invdet * (M[1][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) + M[1][2] * (M[2][3] * M[3][0] - M[2][0] * M[3][3]) + M[1][3] * (M[2][0] * M[3][2] - M[2][2] * M[3][0]));
	res.M[1][1] = invdet * (M[0][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) + M[0][2] * (M[2][3] * M[3][0] - M[2][0] * M[3][3]) + M[0][3] * (M[2][0] * M[3][2] - M[2][2] * M[3][0]));
	res.M[1][2] = -invdet * (M[0][0] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) + M[0][2] * (M[1][3] * M[3][0] - M[1][0] * M[3][3]) + M[0][3] * (M[1][0] * M[3][2] - M[1][2] * M[3][0]));
	res.M[1][3] = invdet * (M[0][0] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) + M[0][2] * (M[1][3] * M[2][0] - M[1][0] * M[2][3]) + M[0][3] * (M[1][0] * M[2][2] - M[1][2] * M[2][0]));
	res.M[2][0] = invdet * (M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) + M[1][1] * (M[2][3] * M[3][0] - M[2][0] * M[3][3]) + M[1][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]));
	res.M[2][1] = -invdet * (M[0][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) + M[0][1] * (M[2][3] * M[3][0] - M[2][0] * M[3][3]) + M[0][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]));
	res.M[2][2] = invdet * (M[0][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) + M[0][1] * (M[1][3] * M[3][0] - M[1][0] * M[3][3]) + M[0][3] * (M[1][0] * M[3][1] - M[1][1] * M[3][0]));
	res.M[2][3] = -invdet * (M[0][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1]) + M[0][1] * (M[1][3] * M[2][0] - M[1][0] * M[2][3]) + M[0][3] * (M[1][0] * M[2][1] - M[1][1] * M[2][0]));
	res.M[3][0] = -invdet * (M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) + M[1][1] * (M[2][2] * M[3][0] - M[2][0] * M[3][2]) + M[1][2] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]));
	res.M[3][1] = invdet * (M[0][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) + M[0][1] * (M[2][2] * M[3][0] - M[2][0] * M[3][2]) + M[0][2] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]));
	res.M[3][2] = -invdet * (M[0][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) + M[0][1] * (M[1][2] * M[3][0] - M[1][0] * M[3][2]) + M[0][2] * (M[1][0] * M[3][1] - M[1][1] * M[3][0]));
	res.M[3][3] = invdet * (M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) + M[0][1] * (M[1][2] * M[2][0] - M[1][0] * M[2][2]) + M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0]));
	
	return res;
}

SVector Matrix4::GetViewLocation() const
{
	return SVector(M[3][0], M[3][1], M[3][2]);
}

SVector Matrix4::GetLocation() const
{
	return SVector(M[0][3], M[1][3], M[2][3]);
}

SVector Matrix4::GetForward() const
{
	return SVector(-M[0][2], -M[1][2], -M[2][2]);
}

SVector Matrix4::GetRight() const
{
	return {M[0][0], M[1][0], M[2][0]};
}

 SVector Matrix4::GetUp() const
{
	return {M[0][1], M[1][1], M[2][1]};
}