#pragma once

#include "reflex.h"
#include "singleton.h"
#include <map>
#include <string>

#include "tools/loader.h"

PRE_DECLARE_CLASS(Mesh)
USING_SPTR(Mesh)
PRE_DECLARE_CLASS(SkeletalMesh)
USING_SPTR(SkeletalMesh)
PRE_DECLARE_CLASS(Skeleton)
USING_SPTR(Skeleton)
PRE_DECLARE_CLASS(AnimSequence)
USING_SPTR(AnimSequence)
PRE_DECLARE_CLASS(Shader)
USING_SPTR(Shader)
PRE_DECLARE_CLASS(Material)
USING_SPTR(Material)
PRE_DECLARE_CLASS(Texture2D)
USING_SPTR(Texture2D)
PRE_DECLARE_CLASS(Texture3D)
USING_SPTR(Texture3D)

#define GL_RGB				0x1907
#define GL_BGRA				0x80E1
#define GL_UNSIGNED_BYTE	0x1401
#define GL_TEXTURE_2D		0x0DE1

SINGLETON_CLASS(AssetManager) {
	SINGLETON(AssetManager)
public:
	std::map<std::string, SPTR_Shader> map_shaders;
	std::map<std::string, SPTR_Mesh> map_meshs;
	std::map<std::string, SPTR_SkeletalMesh> map_skeletalmeshs;
	std::map<std::string, SPTR_Skeleton> map_skeletons;				// 骨骼 也作为资源 -- like ue4
	std::map<std::string, SPTR_AnimSequence> map_anim_sequence;		// 动画序列 资源 -- like ue4
	std::map<std::string, SPTR_Material> map_materials;
	std::map<std::string, SPTR_Texture2D> map_textures;
	std::map<std::string, SPTR_Texture3D> map_texture3Ds;			// cube texture map 

public:
	SPTR_Shader load_shader(const std::string& key, const std::string& v_path, const std::string& f_path, const std::string& g_path = "", SourceType source_type = SourceType::BY_FILE);
	SPTR_Shader get_shader(const std::string& key);
	bool delete_shader(const std::string& key);
	bool clear_shaders();

	SPTR_Mesh load_mesh(const std::string& key, const std::string& src, SourceType source_type = SourceType::BY_FILE);
	SPTR_Mesh load_mesh_x(const std::string& key, const std::string& path);	// load mesh by assimp
	SPTR_Mesh get_mesh_o(const std::string& key);			// 获得原始 mesh
	SPTR_Mesh get_mesh(const std::string& key);				// 获得 mesh 实例 -- 深度复制

	SPTR_SkeletalMesh load_mesh_skeletal(const std::string& key, const std::string& path);
	SPTR_SkeletalMesh get_mesh_skeletal_o(const std::string& key);			// 获得原始 skeleton
	SPTR_SkeletalMesh get_mesh_skeletal(const std::string& key);

	SPTR_Skeleton get_skeleton(const std::string& key);

	bool load_anim_sequences(const std::string& path, const std::string& skeleton_name);
	SPTR_AnimSequence get_anim_sequence(const std::string& key);	// 获得动画序列

	bool load_materials(const std::string& src, SourceType source_type = SourceType::BY_FILE);
	SPTR_Material get_material_o(const std::string& key);
	SPTR_Material get_material(const std::string& key);

	bool load_texture(const std::string& path, SourceType source_type = SourceType::BY_FILE, bool b_srgb = true);
	bool load_texture_x(const std::string& path, bool b_srgb = true);			// load texture by stbi
	SPTR_Texture2D get_texture(const std::string& key);
	SPTR_Texture2D gen_blank_texture(const std::string& key, uint width, uint heigh, uint internal_format = GL_RGB, uint format = GL_RGB, uint data_type = GL_UNSIGNED_BYTE, uint type = GL_TEXTURE_2D);		// 创建空白的 texutre2D -- for RT

	bool load_texture_3d(const std::string& path, bool b_srgb = true);			// 需传入目录 -- 包含6张有命名规则的贴图
	SPTR_Texture3D get_texture3D(const std::string& key);
};
SINGLETON_X(AssetManager)


