/**
 *  .file devel/test01.cpp
 *  Development / testing file.
 *  NOTE. this file is here for feature development / testing purposes only
 *  and its source code, input, output can change witout prior notice.
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/sphere.hpp>
#include <oglplus/shapes/torus.hpp>
#include <oglplus/bound/buffer.hpp>
#include <oglplus/bound/texture.hpp>

#include <iostream>

#include <oglplus/images/random.hpp>
#include <oglplus/images/newton.hpp>
#include <oglplus/images/load.hpp>
#include <oglplus/images/normal_map.hpp>

#include <cmath>

#include "test.hpp"

namespace oglplus {
namespace images {

} // namespace images


class Test01 : public Test
{
private:
	typedef shapes::Cube Shape;
	//typedef shapes::Sphere Shape;
	//typedef shapes::Torus Shape;
	//
	Shape shape;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader vs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// A vertex array object for the rendered shape
	VertexArray vao;

	// VBOs for the shape's vertices and normals
	Buffer verts;
	Buffer normals;
	Buffer tangents;
	Buffer texcoords;

	// textures for the shape
	Texture color_tex, bump_tex;
public:
	Test01(void)
	{
		// Set the vertex shader source
		vs.Source(
			"#version 330\n"
			"uniform mat4 projectionMatrix, cameraMatrix, modelMatrix;"
			"in vec4 vertex;"
			"in vec3 normal;"
			"in vec3 tangent;"
			"in vec2 texcoord;"
			"out vec3 fragLight;"
			"out vec2 fragTexCoord;"
			"out mat3 normalMatrix;"
			"out vec2 fragView;"
			"const vec4 lightPos = vec4(1.0, 2.0, 3.0, 1.0);"
			"void main(void)"
			"{"
			"	vec3 fragTangent = ("
			"		modelMatrix *"
			"		vec4(tangent, 0.0)"
			"	).xyz;"
			"	vec3 fragNormal = ("
			"		modelMatrix *"
			"		vec4(normal, 0.0)"
			"	).xyz;"
			"	normalMatrix[0] = fragTangent;"
			"	normalMatrix[1] = cross(fragNormal, fragTangent);"
			"	normalMatrix[2] = fragNormal;"
			"	fragLight = ("
			"		lightPos-"
			"		modelMatrix*vertex"
			"	).xyz;"
			"	fragTexCoord = texcoord;"
			"	fragView = mat3x2(normalMatrix[0], normalMatrix[1]) * -cameraMatrix[2].xyz;"
			"	gl_Position = "
			"		projectionMatrix *"
			"		cameraMatrix *"
			"		modelMatrix *"
			"		vertex;"
			"}"
		);
		// compile it
		vs.Compile();

		// set the fragment shader source
		// (uses the absolute value of normal as color)
		fs.Source(
			"#version 330\n"
			"uniform sampler2D colorTex;"
			"uniform sampler2D bumpTex;"
			"in vec2 fragTexCoord;"
			"in vec3 fragLight;"
			"in mat3 normalMatrix;"
			"in vec2 fragView;"
			"out vec4 fragColor;"
			"const float holeDepth = 0.05;"
			"void main(void)"
			"{"
			"	vec2 stOffs = fragView*holeDepth;"
/*
			"	if(texture2D(bumpTex, fragTexCoord+stOffs).w > 0.25)"
			"		stOffs *= texture2D(bumpTex, fragTexCoord).w;"
*/
			"	vec2 stCoord = fragTexCoord + stOffs;"
			"	vec3 finalNormal = normalMatrix * texture2D(bumpTex, stCoord).xyz;"
			"	vec3 c = texture2D(colorTex, stCoord).rgb;"
			"	float l = dot(fragLight, fragLight);"
			"	float i = (l > 0)?"
			"		dot(finalNormal, fragLight)/l:"
			"		0.0;"
			"	fragColor = vec4(c*i, 1.0);"
/*
			"	vec4 bmap = ;"
			"	float l = dot(fragLight, fragLight);"
			"	vec3 finalNormal = normalMatrix * bm.xyz;"
			"	float i = (l > 0)?"
			"		dot(finalNormal, fragLight)/l:"
			"		0.0;"
			"	vec2 st = fragTexCoord + ;"
			"	vec3 c = texture2D(colorTex, st).rgb;"
			"	fragColor = vec4(c*(3.0*i+0.2), 1.0);"
*/
			"}"
		);
		// compile it
		fs.Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the shape
		vao.Bind();

		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = shape.Vertices(data);
			Bind(verts, Buffer::Target::Array).Data(data);
			// setup the vertex attribs array for the vertices
			VertexAttribArray attr(prog, "vertex");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}

		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = shape.Normals(data);
			Bind(normals, Buffer::Target::Array).Data(data);
			// setup the vertex attribs array for the normals
			VertexAttribArray attr(prog, "normal");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}

		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = shape.Tangents(data);
			Bind(tangents, Buffer::Target::Array).Data(data);
			VertexAttribArray attr(prog, "tangent");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}

		// bind the VBO for the shape tex-coords
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = shape.TexCoordinates(data);
			Bind(texcoords, Buffer::Target::Array).Data(data);
			//
			VertexAttribArray attr(prog, "texcoord");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}

		// setup the textures
		{
			Texture::Active(0);
			Uniform(prog, "colorTex").Set(0);
			auto bound_tex = Bind(color_tex, Texture::Target::_2D);
			bound_tex.Image2D(images::LoadTexture("wooden_crate"));
			bound_tex.GenerateMipmap();
			bound_tex.MinFilter(TextureMinFilter::LinearMipmapLinear);
			bound_tex.MagFilter(TextureMagFilter::Linear);
			bound_tex.WrapS(TextureWrap::Repeat);
			bound_tex.WrapT(TextureWrap::Repeat);
		}
		{
			Texture::Active(1);
			Uniform(prog, "bumpTex").Set(1);
			auto bound_tex = Bind(bump_tex, Texture::Target::_2D);
			bound_tex.Image2D(
				images::NormalMap<GLubyte>(images::LoadTexture("wooden_crate-hmap"))
			);
			bound_tex.GenerateMipmap();
			bound_tex.MinFilter(TextureMinFilter::LinearMipmapLinear);
			bound_tex.MagFilter(TextureMagFilter::Linear);
			bound_tex.WrapS(TextureWrap::Repeat);
			bound_tex.WrapT(TextureWrap::Repeat);
		}

		// set the projection matrix fov = 24 deg. aspect = 1.25
		Uniform(prog, "projectionMatrix").SetMatrix(
			CamMatrixf::Perspective(Degrees(24), 1.25, 1, 100)
		);
		//
		VertexArray::Unbind();
		gl.ClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		//
		gl.FrontFace(shape.FaceWinding());
		gl.CullFace(Face::Back);
		gl.Enable(Capability::CullFace);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		// set the matrix for camera orbiting the origin
		Uniform(prog, "cameraMatrix").SetMatrix(
			CamMatrixf::Orbiting(
				Vec3f(),
				//4.5 + std::sin(time)*3.0,
				1.5,
				Degrees(time * 90),
				Degrees(std::sin(time * 0.1) * 70)
			)
		);

		Uniform(prog, "modelMatrix").SetMatrix(Matrix4f());

		vao.Bind();
		// This is not very effective
		shape.Instructions().Draw(shape.Indices());
	}

	bool Continue(double time)
	{
		return time < 30.0;
	}
};

std::unique_ptr<Test> makeTest(void)
{
	return std::unique_ptr<Test>(new Test01);
}

} // namespace oglplus
