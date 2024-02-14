#include "Camera.h"

void Camera::Initialize(){

	matView = MakeIdentityMatrix();
	matProjection = MakeIdentityMatrix();
	sMatProjection = MakeIdentityMatrix();
}

void Camera::UpdateMatrix(){

	matView = Inverse(MakeAffineMatrix(scale, rotate, translate));
	matProjection = MakePerspectiveFovMatrix(fov, aspectRatio, nearZ, farZ);

	sMatView = MakeIdentityMatrix();
	sMatProjection = MakeOrthographicMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 100.0f);

}
