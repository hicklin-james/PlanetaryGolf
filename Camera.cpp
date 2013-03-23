#include "Camera.h"

Vec3 Camera::positionCamera(float eyex, float eyey, float eyez,
					float lookatx, float lookaty, float lookatz,
					float upx, float upy, float upz) {

	gluLookAt(eyex, eyey, eyez, lookatx+eyex, lookaty+eyey, lookatz+eyez, upx, upy, upz);

	Vec3 Direction = setUpDirectionVector(eyex, eyey, eyez, lookatx, lookaty, lookatz);

	return Direction;

}

Vec3 Camera::setUpDirectionVector(float eyex, float eyey, float eyez,
					      float lookatx, float lookaty, float lookatz) {

	Vec3 CameraPoint(eyex,eyey,eyez);
	Vec3 LookAtPoint(eyex+lookatx,eyey+lookaty,eyez+lookatz);

	Vec3 Direction (LookAtPoint[0] - CameraPoint[0],
					LookAtPoint[1] - CameraPoint[1],
					LookAtPoint[2] - CameraPoint[2]);

	return Direction;
}