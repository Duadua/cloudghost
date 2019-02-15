#pragma once

#include "scenecomponent.h"
#include <string>

class MeshComponent : public SceneComponent {
	DECLARE_CLASS(MeshComponent)
public:
	MeshComponent(const std::string& m = "");
	virtual ~MeshComponent();

	virtual void draw(const std::string& shader) override;

	GET_SET(std::string, mesh);

private:
	std::string mesh;
};
DECLARE_AUTO_PTR(MeshComponent)