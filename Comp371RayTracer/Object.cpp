#include "Object.h"

Object::Object(glm::vec3 position, Material * material) : Position(position), ObjectMaterial(material)
{
}

Object::~Object()
{
	SAFE_DELETE(ObjectMaterial);
}
