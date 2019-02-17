#version 330 core

out vec4 r_color;

in vec3 o_world_pos;
in vec3 o_normal;
in vec2 o_tex_coord;



// base material
struct material {
    vec3 ka;
    vec3 kd;
    vec3 ks;

    sampler2D map_ka;                       // 需要给个默认图片
    sampler2D map_kd;
    sampler2D map_ks;

    float shininess;
};


uniform vec3        u_light_pos;
uniform vec3	    u_light_color;
uniform vec3	    u_view_pos;
uniform material    u_material;

vec3 t_light_dir;
vec3 t_view_dir;
vec3 t_normal;

void pre_cac() {
    t_light_dir = normalize(u_light_pos - o_world_pos);
    t_view_dir = normalize(u_view_pos - o_world_pos);
    t_normal = normalize(o_normal);
}
 
vec3 cac_ambient() { return vec3(1.0); }
vec3 cac_diffuse() { return max(0.0, dot(t_light_dir, t_normal)); }
vec3 cac_spcular(float shininess) {
    vec3 t_halfway = normalize(t_light_dir + t_view_dir);
    return pow(max(0.0, dot(t_normal, t_halfway)), shininess);
}

vec3 blinn_phong(vec3 normal, vec3 light_dir, vec3 view_dir, material mt, float att) {
    vec3 i_ambient = mt.ka /* texture(mt.map_ka, o_tex_coord)*/ * cac_ambient();    
    vec3 i_diffuse = mt.kd /* texture(mt.map_kd, o_tex_coord)*/ * cac_diffuse();
    vec3 i_spcular = mt.ks /* texture(mt.map_ks, o_tex_coord)*/ * cac_spcular(mt.shininess);

    return att * (i_ambient + i_diffuse + i_spcular);
}

float att_drect_light() { return 1.0; }
float att_point_light() { return 1.0; }
float att_spott_light() { return 1.0; }

void main(void) {

    pre_cac();

    vec3 t_color = u_light_color * blinn_phong(t_normal, t_light_dir, t_view_dir, u_material, 1.0);
    //t_color = vec3(1.0, 0.0, 0.0);
    r_color = vec4(t_color, 1.0);

}

/*
    o_* -- in from vertex shader
    u_* -- uniform
    r_* -- result to output
    t_* -- temp variable
*/