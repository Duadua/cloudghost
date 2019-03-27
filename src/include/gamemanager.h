#pragma once

#include "cmath.h"
#include "reflex.h"
#include <map>
#include <string>
#include <memory>

PRE_DECLARE_CLASS(Shader)
USING_SPTR(Shader)
PRE_DECLARE_CLASS(RenderTarget)
USING_SPTR(RenderTarget)
PRE_DECLARE_CLASS(GameObject)
USING_SPTR(GameObject)
PRE_DECLARE_CLASS(CameraObject)
USING_SPTR(CameraObject)
PRE_DECLARE_CLASS(Texture2D)
USING_SPTR(Texture2D)


#define GL_LESS					0x0201
#define GL_TRUE					1
#define GL_EQUAL				0x0202
#define GL_KEEP					0x1E00
#define GL_SRC_ALPHA			0x0302
#define GL_ONE_MINUS_SRC_ALPHA	0x0303
#define GL_FILL					0x1B02
#define GL_LINE					0x1B01
#define GL_BACK					0x0405
#define GL_CCW					0x0901

enum PostProcessType {
	NOPE,					// 原色
	GRAY,					// 灰度
	INVERS,					// 反相
	BLUR,					// 模糊
	SHARPEN,				// 锐化
	EDGE_DET,				// 边缘检测
	
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
	virtual SPTR_Shader			set_main_shader() = 0;		// 设置主Shader
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
	virtual SPTR_Shader			set_main_shader() override;

public:
	GameManager();
    virtual ~GameManager() override;
	static GameManager* get_instance();

	void add_game_object(const std::string& key, SPTR_GameObject value);

public:									// used for qt ui
    GET_SET(CColor, background_color)
    GET_SET(CColor, border_color)
    GET_SET(uint, front_polygon_mode)
    GET_SET(uint, back_polygon_mode)
    GET_SET(bool, b_use_vr)
    GET_SET(float, vr_delta)
    GET_SET(PostProcessType, pp_type)
    GET_SET(bool, b_use_shader_toy)

protected:
	static GameManager* instance;
	
    GET(ViewportInfo, viewport_info)

	SPTR_CameraObject main_camera;
	SPTR_Shader main_shader;

	// ui shading setting
	CColor background_color;
	CColor border_color;			// 物体边框颜色

	uint front_polygon_mode;
	uint back_polygon_mode;

	bool b_use_vr;
	float vr_delta;

	PostProcessType pp_type;		// 当前后处理效果

	bool b_use_shader_toy;

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
	void main_draw(const std::string& shader);

	// default init
	void map_input_default();
	void bind_input_default();
	
	// render pipe
	SPTR_RenderTarget scene_rt;
	SPTR_RenderTarget pp_rt;
	SPTR_RenderTarget pick_rt;
	SPTR_RenderTarget vr_rt;
	SPTR_RenderTarget vr_rt_mix;
	SPTR_RenderTarget shader_toy_rt;
	SPTR_RenderTarget shader_toy_buffer_rts[4];
	SPTR_Texture2D scene_texture;
	void scene_pass();
	void pick_pass();					// 拾取阶段
	void base_pass();
	void post_process_pass();
	void vr_pass();
	void shader_toy_pass();

	void init_rt();
	void init_pick_rt();
	void init_vr_rt();
	void init_shader_toy_rt();

	void draw_scene(const std::string& shader);
	void draw_all_objs(const std::string& shader);
	void draw_border(const std::string& shader);

	void draw_init();

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