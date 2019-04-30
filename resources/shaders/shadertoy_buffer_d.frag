#version 330 core

// ================================================================================
// define

#define samplerXX sampler2D

// ================================================================================
// in & out

out vec4 r_color;

// ================================================================================
// uniform

// shadertoy 着色器输入
uniform vec3		iResolution;			// viewport resolution (in pixels)
uniform float		iTime;					// shader playback time (in seconds)
uniform float		iTimeDelta;				// render time (in seconds)
uniform int			iFrame;					// shader playback frame
uniform float		iChannelTime[4];		// channel playback time (in seconds)
uniform vec3		iChannelResolution[4];	// channel resolution (in pixels)
uniform vec4		iMouse;					// mouse pixel coords. xy: current (if MLB down), zw: click
uniform samplerXX iChannel0;				// input channel. XX = 2D/Cube
uniform samplerXX iChannel1;				// input channel. XX = 2D/Cube
uniform samplerXX iChannel2;				// input channel. XX = 2D/Cube
uniform samplerXX iChannel3;				// input channel. XX = 2D/Cube
uniform vec4      iDate;					// (year, month, day, time in seconds)
uniform float     iSampleRate;				// sound sample rate (i.e., 44100)

// ================================================================================
// shader_toy_begin

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    fragColor = vec4(0.0,0.0,1.0,1.0);
}

// shader_toy_end
// ================================================================================

// ================================================================================
// template

void main() {
	mainImage(r_color, gl_FragCoord.xy);
}

// ================================================================================