#pragma once

#include "cmath.h"
#include "reflex.h"
#include <map>
#include <string>
#include <memory>

PRE_DECLARE_CLASS(Shader)
USING_SPTR(Shader)
PRE_DECLARE_CLASS(ShaderStack)
USING_SPTR(ShaderStack)
PRE_DECLARE_CLASS(RenderTarget)
USING_SPTR(RenderTarget)
PRE_DECLARE_CLASS(GameObject)
USING_SPTR(GameObject)
PRE_DECLARE_CLASS(CameraObject)
USING_SPTR(CameraObject)
PRE_DECLARE_CLASS(DirectLightObject)
USING_SPTR(DirectLightObject)
PRE_DECLARE_CLASS(PointLightObject)
USING_SPTR(PointLightObject)
PRE_DECLARE_CLASS(SpotLightObject)
USING_SPTR(SpotLightObject)
PRE_DECLARE_CLASS(SkyBox)
USING_SPTR(SkyBox)
PRE_DECLARE_CLASS(Texture2D)
USING_SPTR(Texture2D)
PRE_DECLARE_CLASS(Texture3D)
USING_SPTR(Texture3D)
PRE_DECLARE_CLASS(UniformBuffer)
USING_SPTR(UniformBuffer)


#define GL_LESS					0x0201
#define GL_TRUE					1
#define GL_EQUAL				0x0202
#define GL_KEEP					0x1E00
#define GL_SRC_ALPHA			0x0302
#define GL_ONE_MINUS_SRC_ALPHA	0x0303
#define GL_POINT				0x1B00
#define GL_LINE					0x1B01
#define GL_FILL					0x1B02
#define GL_BACK					0x0405
#define GL_CCW					0x0901

enum class PostProcessType {
	NOPE,					// 原色
	GRAY,					// 灰度
	INVERS,					// 反相
	BLUR,					// 模糊
	SHARPEN,				// 锐化
	EDGE_DET,				// 边缘检测
	REAL_RB					// 真_红蓝3D
	
};

enum class HDR_Type {
	NOPE,					// 没有HDR
	REINHARD,				// Reinhard 算法的 HDR
	EXPOSURE				// Exposure 曝光

};

struct ViewportInfo {		// 视口信息
	uint width;
	uint heigh;
	ViewportInfo(uint w = 800, uint h = 600) : width(w), heigh(h){}
};

class GameManagerImpl {
public:
	virtual ~GameManagerImpl() {}

	virtual void load_asset() = 0;							// 加载资源
	virtual void begin_play() = 0;							// 设置模型等
	virtual void tick(float time) = 0;						// 每帧变化的参数
	virtual void map_input() = 0;							// 绑定输入

	virtual SPTR_CameraObject	set_main_camera() = 0;		// 设置主相机 
};

// 这是一个子类代理单例 -- 所以不能继承 Singleton
class GameManager : public GameManagerImpl {

public:
	void init();
	void draw();

	void pre_init(uint w = 800, uint h = 600);
	void resize(uint w, uint h);
	void exit();

public:
	virtual void load_asset() override {}
	virtual void begin_play() override {}
	virtual void tick(float time) override { time; }
	virtual void map_input() override {}

	virtual SPTR_CameraObject	set_main_camera() override;

public:
	GameManager();
    virtual ~GameManager() override;
	static GameManager* get_instance();

	void add_game_object(const std::string& key, SPTR_GameObject value);
	void add_direct_light(const std::string& key, SPTR_DirectLightObject value);
	void add_point_light(const std::string& key, SPTR_PointLightObject value);
	void add_spots_light(const std::string& key, SPTR_SpotLightObject value);


public:									// used for qt ui
	SPTR_Texture3D get_skybox();
	void set_skybox(SPTR_Texture3D tex);

    GET_SET(CColor, background_color)
    GET_SET(CColor, border_color)
    GET_SET(uint, front_polygon_mode)
    GET_SET(uint, back_polygon_mode)
    GET_SET(bool, b_use_vr)
    GET_SET(float, vr_delta)
    GET_SET(PostProcessType, pp_type)
	GET_SET(bool, b_gamma)
	GET_SET(float, v_gamma)
    GET_SET(bool, b_use_shader_toy)
	GET_SET(bool, b_normal_visual)
	GET_SET(bool, b_explode)
	GET_SET(bool, b_msaa)
	GET_SET(bool, b_hdr)
	GET_SET(HDR_Type, hdr_type)
	GET_SET(float, hdr_exposure)
	GET_SET(bool, b_skybox)
	GET_SET(bool, b_depth)
	GET_SET(bool, b_shadow)

protected:
	static GameManager* instance;

	virtual void _init();			// 构造时 init
	
    GET(ViewportInfo, viewport_info)

	SPTR_CameraObject main_camera;

	SPTR_ShaderStack stack_shaders;
	SPTR_UniformBuffer ub_matrices;
	
	SPTR_SkyBox sky_box;

	std::map<std::string, SPTR_DirectLightObject>	map_direct_lights;
	std::map<std::string, SPTR_PointLightObject>	map_point_lights;
	std::map<std::string, SPTR_SpotLightObject>		map_spots_lights;

	// ui shading setting
	CColor background_color;
	CColor border_color;			// 物体边框颜色

	uint front_polygon_mode;
	uint back_polygon_mode;

	bool b_use_vr;
	float vr_delta;

	PostProcessType pp_type;		// 当前后处理效果

	bool b_use_shader_toy;

	// 几何着色器效果
	bool b_normal_visual;			// 法线是否可视化
	bool b_explode;					// 爆炸效果

	bool b_msaa;					// 是否开启多重采样抗锯齿
	bool b_msaa_custom;				// 是否使用自定义的抗锯齿算法

	bool b_gamma;					// 是否进行 gamma 校正
	float v_gamma;					// gamma 校正系数 -- 默认 2.2

	bool b_hdr;						// 是否开启 HDR
	HDR_Type hdr_type;				// HDR 算法类型 -- 默认为 R
	float hdr_exposure;				// 曝光参数 -- 仅在 EXPOSURE 算法下有效

	bool b_skybox;					// 是否渲染天空盒

	bool b_depth;					// 是否渲染当前相机视图下的深度图
	bool b_shadow;					// 是否显示阴影

	// gl state
	void set_depth_test(bool enable = true, uint depth_func = GL_LESS, uint depth_mask = GL_TRUE);
	void set_stencil_test(bool enable = false, uint func = GL_EQUAL, uint ref = 1, uint mask = 0xff, uint fail = GL_KEEP, uint zfail = GL_KEEP, uint zpass = GL_KEEP);
	void set_blend(bool enable, uint sfactor = GL_SRC_ALPHA, uint dfactor = GL_ONE_MINUS_SRC_ALPHA);
	void set_polygon_mode(uint front_mode = GL_FILL, uint back_mode = GL_LINE);
	void set_cull_face(bool enable = false, uint mode = GL_BACK, uint front_face = GL_CCW);

private:
	void main_bind_input();
	void main_begin_play();
	void main_tick(float time);
	void main_draw(SPTR_Shader shader);

	// default init
	void map_input_default();
	void bind_input_default();
	
	// render pipe
	SPTR_RenderTarget scene_rt;
	SPTR_RenderTarget depth_rt;
	SPTR_RenderTarget depth_msaa_rt;
	SPTR_RenderTarget pp_rt;
	SPTR_RenderTarget gamma_rt;
	SPTR_RenderTarget pick_rt;
	SPTR_RenderTarget border_depth_rt;
	SPTR_RenderTarget border_rt;

	SPTR_RenderTarget shader_toy_rt;
	SPTR_RenderTarget shader_toy_buffer_rts[4];

	SPTR_RenderTarget msaa_rt;

	SPTR_RenderTarget hdr_rt;
	SPTR_RenderTarget hdr_scene_rt;
	SPTR_RenderTarget hdr_pp_rt;
	SPTR_RenderTarget hdr_msaa_rt;
	
	SPTR_RenderTarget vr_rt;
	SPTR_RenderTarget vr_msaa_rt;							// 3d 模式下的 msaa 
	SPTR_RenderTarget vr_hdr_rt;
	SPTR_RenderTarget vr_hdr_msaa_rt;						
	SPTR_RenderTarget vr_mix_rt;
	SPTR_RenderTarget vr_hdr_mix_rt;
	SPTR_RenderTarget vr_depth_rt;
	SPTR_RenderTarget vr_depth_msaa_rt;
	SPTR_RenderTarget vr_depth_mix_rt;
	SPTR_RenderTarget vr_border_depth_rt;
	SPTR_RenderTarget vr_border_mix_rt;
	SPTR_RenderTarget vr_border_rt;

	std::vector<SPTR_RenderTarget> direct_light_shadow_rts;	// 阴影专用
	std::vector<SPTR_RenderTarget> point_light_shadow_rts;	// 点光源阴影

	SPTR_Texture2D scene_texture;
	SPTR_Texture2D depth_texture;							// 当前相机视图下的深度图

	void scene_pass(SPTR_Texture2D tex);					// 根据传入的贴图绘制屏幕四边形
	void depth_pass();										// 获得当前相机视图下的深度图
	void shadow_pass();
	void pick_pass();										// 拾取阶段
	void base_pass();
	void border_pass();										// 加轮廓
	void post_process_pass();
	void hdr_pass();										// hdr
	void gamma_pass();										// gamma 校正
	void normal_visual_pass();								// 法线可视化
	void shader_toy_pass();

	void vr_depth_pass();
	void vr_base_pass();
	void vr_border_pass();

	void init_rt();
	void init_pick_rt();
	void init_vr_rt();
	void init_shader_toy_rt();
	void init_msaa_rt();
	void init_hdr_rt();
	void init_shadow_rt();

	void draw_scene(SPTR_Shader shader);
	void draw_all_objs(SPTR_Shader shader);
	void draw_border(SPTR_Shader shader);
	void draw_lights(SPTR_Shader shader);
	void draw_init();

	void draw_skybox(SPTR_Shader shader);

private:
	ViewportInfo viewport_info;

	std::map<std::string, SPTR_GameObject> game_objects;

private:
	// 鼠标事件 --拾取用
	bool b_mouse_clicked;
	uint mouse_clicked_x;
	uint mouse_clicked_y;
	uint cur_pick_object_id;
	uint cur_pick_component_id;
	void mouse_released();

};

inline GameManager& GameManager_ins() { return (*GameManager::get_instance()); }