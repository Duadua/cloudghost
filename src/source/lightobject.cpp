#include "lightobject.h"
#include "lightcomponent.h"
#include "meshcomponent.h"
#include "assetmanager.h"
#include "mesh.h"
#include "shader.h"

bool LightObject::use(SPTR_Shader shader) {
	auto t_lc = std::dynamic_pointer_cast<LightComponent>(root_component);
	if (t_lc == nullptr) return false;
	return t_lc->use(shader);
}

// ======================================================================

IMPLEMENT_CLASS(DirectLightObject)

DirectLightObject::DirectLightObject() {
	light_type = LightType::DIRECT_LIGHT;
	root_component = CREATE_CLASS(DirectLightComponent);

    m_mesh_component = CREATE_CLASS(MeshComponent);
    m_mesh_component->attach_to(root_component);
    auto t_m = AssetManager_ins().get_mesh("sphere");
    m_mesh_component->set_mesh(t_m);
}

void DirectLightObject::draw(SPTR_Shader shader)
{
    if (nullptr != shader)
    {
        CVector4D t_color = CVector4D(get_light_component()->get_color() * 20.0f, 1.0f);
        shader->set_vec4("u_solid_color", t_color);
    }
}
// ======================================================================

IMPLEMENT_CLASS(PointLightObject)

PointLightObject::PointLightObject() {
	light_type = LightType::POINT_LIGHT;
	root_component = CREATE_CLASS(PointLightComponent);

    m_mesh_component = CREATE_CLASS(MeshComponent);
	m_mesh_component->attach_to(root_component);
    auto t_m = AssetManager_ins().get_mesh("cube");
    m_mesh_component->set_mesh(t_m);
}

void PointLightObject::draw(SPTR_Shader shader) 
{
	if (nullptr != shader)
	{
		CVector4D t_color = CVector4D(get_light_component()->get_color() * 10.0f, 1.0f);
        shader->set_vec4("u_solid_color", t_color);
	}
}

// ======================================================================

IMPLEMENT_CLASS(SpotLightObject)

SpotLightObject::SpotLightObject() {
	light_type = LightType::SPOT_LIGHT;
	root_component = CREATE_CLASS(SpotLightComponent);
}

// ======================================================================

IMPLEMENT_CLASS(SkyLightObject)

SkyLightObject::SkyLightObject() {
	light_type = LightType::SKY_LIGHT;
	root_component = CREATE_CLASS(SkyLightComponent);
}

