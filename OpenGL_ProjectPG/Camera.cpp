#include "Camera.hpp"

namespace gps {

	//Camera constructor
	Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUpDirection) {
		//TODO
		this->cameraPosition = cameraPosition;
		this->cameraTarget = cameraTarget;

		this->cameraUpDirection = cameraUpDirection;
		this->cameraFrontDirection = cameraPosition - cameraTarget;
		this->cameraRightDirection = glm::cross(this->cameraUpDirection, this->cameraFrontDirection);


	}

	//return the view matrix, using the glm::lookAt() function
	glm::mat4 Camera::getViewMatrix() {
		//TODO
		return glm::lookAt(cameraPosition, cameraTarget, cameraUpDirection);
	}



	//update the camera internal parameters following a camera move event
	void Camera::move(MOVE_DIRECTION direction, float speed) {
		//TODO
		glm::vec3 dir;
		switch (direction) {
		case MOVE_FORWARD: {
			dir = this->cameraFrontDirection * (-speed);
			break;
		}
		case MOVE_BACKWARD: {
			dir = this->cameraFrontDirection * speed;
			break;
		}
		case MOVE_RIGHT: {
			dir = this->cameraRightDirection * speed;
			break;
		}
		case MOVE_LEFT: {
			dir = this->cameraRightDirection * (-speed);
			break;
		}
		case MOVE_UP: {
			dir = this->cameraUpDirection * speed;
			break;
		}
		case MOVE_DOWN: {
			dir = this->cameraUpDirection * (-speed);
			break;
		}
		}
		glm::mat4 model = glm::mat4(1.0f);
		this->cameraPosition += dir;
		this->cameraTarget += dir;

	}

	void Camera::rotate(float pitch, float yaw) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, this->cameraPosition);
		model = glm::rotate(model, yaw, this->cameraUpDirection);
		model = glm::rotate(model, pitch, this->cameraRightDirection);
		model = glm::translate(model, -this->cameraPosition);
		glm::vec4 temp = model * glm::vec4(this->cameraTarget, 1.0f);
		this->cameraTarget = temp;
		this->cameraFrontDirection = this->cameraPosition - this->cameraTarget;
		this->cameraRightDirection = glm::cross(this->cameraUpDirection, this->cameraFrontDirection);
	}

}