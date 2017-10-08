#ifndef MATHS_H
#define MATHS_H

#include "GLHeaders.h"

class Maths {
public:
	//Makes the class uninstansiable
	virtual ~Maths() = 0;

	//Takes in a plane (p1,p2,p3 defines a plane) and then a 2D x,z position on the plane
	//And works out out its height on the terrain by interpolating the heights of each corner of the plane
	static float barryCentricInterpolation(vec3 p1, vec3 p2, vec3 p3, vec2 pos) {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}


	static mat4 createTransformMat(vec3 trans, quat rot, vec3 scale) {
		//Rotation using quaternions (still has gimbal lock on z-axis,
		//but is more efficient then vector/matrix rotations)
		mat4 m(1.0f);
		m = glm::translate(m, trans);
		m = m * mat4_cast(rot);
		m = glm::scale(m, scale);
		return m;
	}

	static mat4 createTransformMat(vec3 trans, quat rot, float scale = 1.0f) {
		//Rotation using quaternions (still has gimbal lock on z-axis,
		//but is more efficient then vector/matrix rotations)
		mat4 m(1.0f);
		m = glm::translate(m, trans);
		m = m * mat4_cast(rot);
		m = glm::scale(m, vec3(scale));
		return m;
	}

	static mat4 createTransformMat(vec3 trans, vec3 rot, float scale = 1.0f) {
		//Rotation using quaternions (still has gimbal lock on z-axis,
		//but is more efficient then vector/matrix rotations)
		mat4 m(1.0f);
		m = glm::translate(m, trans);
		rot = glm::radians(vec3(rot));
		glm::quat rotationQuat = quat(rot);
		m = m * mat4_cast(rotationQuat);
		m = glm::scale(m, vec3(scale));
		return m;
	}

	static mat4 createProjectionMatrix(float width, float height, float fov, float nearPlane, float farPlane) {
		//Get the aspect ratio of the screen (this will make sure that square objects actually appear square
		//and not skewed by the size of shape of the display)
		const float aspectRatio = width / height;
		//Create the projection matrix
		return  glm::perspective(radians(fov), aspectRatio, nearPlane, farPlane);
	}
};

#endif