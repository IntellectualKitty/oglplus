/**
 *  @file oglplus/texture.hpp
 *  @brief Texture object wrappers
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_TEXTURE_1107121519_HPP
#define OGLPLUS_TEXTURE_1107121519_HPP

#include <oglplus/error.hpp>
#include <oglplus/object.hpp>
#include <oglplus/friend_of.hpp>
#include <oglplus/compare_func.hpp>
#include <oglplus/pixel_data.hpp>
#include <oglplus/buffer.hpp>
#include <oglplus/limited_value.hpp>
#include <oglplus/image.hpp>
#include <oglplus/auxiliary/binding_query.hpp>
#include <cassert>

namespace oglplus {

#ifdef OGLPLUS_DOCUMENTATION_ONLY
/// Type for the texture unit selector (implementation-dependent limited) number
class TextureUnitSelector
 : public LimitedCount
{
public:
	TextureUnitSelector(GLuint count);
};
#else
OGLPLUS_DECLARE_LIMITED_COUNT_TYPE(
	TextureUnitSelector,
	MAX_COMBINED_TEXTURE_IMAGE_UNITS
)
#endif


/// Texture compare mode enumeration
/**
 *  @ingroup enumerations
 */
enum class TextureCompareMode : GLenum {
	/// NONE
	None = GL_NONE,
	/// COMPARE_REF_TO_TEXTURE
	CompareRefToTexture = GL_COMPARE_REF_TO_TEXTURE
};

/// Texture magnification filter enumeration
/**
 *  @ingroup enumerations
 */
enum class TextureMagFilter : GLenum {
	/// NEAREST
	Nearest = GL_NEAREST,
	/// LINEAR
	Linear = GL_LINEAR
};

/// Texture minification filter enumeration
/**
 *  @ingroup enumerations
 */
enum class TextureMinFilter : GLenum {
	/// NEAREST
	Nearest = GL_NEAREST,
	/// LINEAR
	Linear = GL_LINEAR,
	/// NEAREST_MIPMAP_NEAREST
	NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
	/// NEAREST_MIPMAP_LINEAR
	NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
	/// LINEAR_MIPMAP_NEAREST
	LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	/// LINEAR_MIPMAP_LINEAR
	LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
};

/// Texture swizzle parameter coordinate enumeration
/**
 *  @ingroup enumerations
 */
enum class TextureSwizzleCoord : GLenum {
	/// TEXTURE_SWIZZLE_R
	R = GL_TEXTURE_SWIZZLE_R,
	/// TEXTURE_SWIZZLE_G
	G = GL_TEXTURE_SWIZZLE_G,
	/// TEXTURE_SWIZZLE_B
	B = GL_TEXTURE_SWIZZLE_B,
	/// TEXTURE_SWIZZLE_A
	A = GL_TEXTURE_SWIZZLE_A,
	/// TEXTURE_SWIZZLE_RGBA
	RGBA = GL_TEXTURE_SWIZZLE_RGBA
};

/// Texture swizzle enumeration
/**
 *  @ingroup enumerations
 */
enum class TextureSwizzle : GLenum {
	/// RED
	Red = GL_RED,
	/// GREEN
	Green = GL_GREEN,
	/// BLUE
	Blue = GL_BLUE,
	/// ALPHA
	Alpha = GL_ALPHA,
	/// ZERO
	Zero = GL_ZERO,
	/// ONE
	One = GL_ONE
};

/// Texture wrap parameter coordinate enumeration
/**
 *  @ingroup enumerations
 */
enum class TextureWrapCoord : GLenum {
	/// TEXTURE_WRAP_S
	S = GL_TEXTURE_WRAP_S,
	/// TEXTURE_WRAP_T
	T = GL_TEXTURE_WRAP_T,
	/// TEXTURE_WRAP_R
	R = GL_TEXTURE_WRAP_R
};

/// Texture wrap enumeration
/**
 *  @ingroup enumerations
 */
enum class TextureWrap : GLenum {
	/// CLAMP_TO_EDGE
	ClampToEdge = GL_CLAMP_TO_EDGE,
	/// REPEAT
	Repeat = GL_REPEAT,
	/// CLAMP_TO_BORDER
	ClampToBorder = GL_CLAMP_TO_BORDER,
	/// MIRRORED_REPEAT
	MirroredRepeat = GL_MIRRORED_REPEAT
};

/// Wrapper for texture and texture unit-related operations
/** @note Do not use this class directly, use Texture instead.
 */
class TextureOps
 : public Named
 , public FriendOf<Buffer>
{
protected:
	static void _init(GLsizei count, GLuint& _name)
	{
		::glGenTextures(count, &_name);
		ThrowOnError(OGLPLUS_ERROR_INFO(GenTextures));
	}

	static void _cleanup(GLsizei count, GLuint& _name)
	{
		assert(_name != 0);
		::glDeleteTextures(count, &_name);
	}

	static GLboolean _is_x(GLuint _name)
	{
		assert(_name != 0);
		return ::glIsTexture(_name);
	}

	friend class FriendOf<TextureOps>;
public:
	/// Texture bind and image specification targets
	/** @note Not all of the values enumerated here are valid
	 *  bind targets. Some of them are just image specification
	 *  targets.
	 */
	enum class Target : GLenum {
		/// TEXTURE_1D
		_1D = GL_TEXTURE_1D,
		/// TEXTURE_2D
		_2D = GL_TEXTURE_2D,
		/// TEXTURE_3D
		_3D = GL_TEXTURE_3D,
		/// TEXTURE_1D_ARRAY
		_1DArray = GL_TEXTURE_1D_ARRAY,
		/// TEXTURE_2D_ARRAY
		_2DArray = GL_TEXTURE_2D_ARRAY,
		/// TEXTURE_RECTANGLE
		Rectangle = GL_TEXTURE_RECTANGLE,
		/// TEXTURE_BUFFER
		Buffer = GL_TEXTURE_BUFFER,
		/// TEXTURE_CUBE_MAP
		CubeMap = GL_TEXTURE_CUBE_MAP,
		/// TEXTURE_CUBE_MAP_ARRAY
		CubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
		/// TEXTURE_2D_MULTISAMPLE
		_2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
		/// TEXTURE_2D_MULTISAMPLE_ARRAY
		_2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
		/// TEXTURE_CUBE_MAP_POSITIVE_X
		CubeMapPositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		/// TEXTURE_CUBE_MAP_NEGATIVE_X
		CubeMapNegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		/// TEXTURE_CUBE_MAP_POSITIVE_Y
		CubeMapPositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		/// TEXTURE_CUBE_MAP_NEGATIVE_Y
		CubeMapNegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		/// TEXTURE_CUBE_MAP_POSITIVE_Z
		CubeMapPositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		/// TEXTURE_CUBE_MAP_NEGATIVE_Z
		CubeMapNegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

protected:
	static GLenum _binding_query(Target target)
	{
		switch(GLenum(target))
		{
			case GL_TEXTURE_1D:
				return GL_TEXTURE_BINDING_1D;
			case GL_TEXTURE_2D:
				return GL_TEXTURE_BINDING_2D;
			case GL_TEXTURE_3D:
				return GL_TEXTURE_BINDING_3D;
			case GL_TEXTURE_1D_ARRAY:
				return GL_TEXTURE_BINDING_1D_ARRAY;
			case GL_TEXTURE_2D_ARRAY:
				return GL_TEXTURE_BINDING_2D_ARRAY;
			case GL_TEXTURE_RECTANGLE:
				return GL_TEXTURE_BINDING_RECTANGLE;
			case GL_TEXTURE_BUFFER:
				return GL_TEXTURE_BINDING_BUFFER;
			case GL_TEXTURE_CUBE_MAP:
				return GL_TEXTURE_BINDING_CUBE_MAP;
			case GL_TEXTURE_CUBE_MAP_ARRAY:
				return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY;
			case GL_TEXTURE_2D_MULTISAMPLE:
				return GL_TEXTURE_BINDING_2D_MULTISAMPLE;
			case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
				return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
			case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
				return GL_TEXTURE_BINDING_CUBE_MAP;
			case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
				return GL_TEXTURE_BINDING_CUBE_MAP;
			case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
				return GL_TEXTURE_BINDING_CUBE_MAP;
			case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
				return GL_TEXTURE_BINDING_CUBE_MAP;
			case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
				return GL_TEXTURE_BINDING_CUBE_MAP;
			case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
				return GL_TEXTURE_BINDING_CUBE_MAP;
			default:;
		}
		return 0;
	}
	friend class BindingQuery<TextureOps>;
public:

	/// Specify active texture unit for subsequent commands
	/**
	 *  @throws Error
	 *
	 *  @see Bind
	 */
	static void Active(TextureUnitSelector index)
	{
		::glActiveTexture(GLenum(GL_TEXTURE0 + GLuint(index)));
		AssertNoError(OGLPLUS_ERROR_INFO(ActiveTexture));
	}

	/// Bind the texture to the target on the Active unit
	/**
	 *  @throws Error
	 *
	 *  @see Active
	 *  @see Unbind
	 */
	void Bind(Target target) const
	{
		assert(_name != 0);
		::glBindTexture(GLenum(target), _name);
		AssertNoError(OGLPLUS_OBJECT_ERROR_INFO(
			BindTexture,
			Texture,
			_name
		));
	}

	/// Unbinds the current texture from the target on the Active unit
	/**
	 *  @throws Error
	 *
	 *  @see Active
	 *  @see Bind
	 */
	static void Unbind(Target target)
	{
		::glBindTexture(GLenum(target), 0);
		AssertNoError(OGLPLUS_ERROR_INFO(BindTexture));
	}

	static GLint GetIntParam(Target target, GLint level, GLenum query)
	{
		GLint result = 0;
		::glGetTexLevelParameteriv(
			GLenum(target),
			level,
			query,
			&result
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			GetTexLevelParameteriv,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
		return result;
	}

	/// Returns the width of the texture as it was specified by *Image*D
	/**
	 *  @see Height
	 *  @see Depth
	 */
	static GLsizei Width(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(target, level, GL_TEXTURE_WIDTH));
	}

	/// Returns the height of the texture as it was specified by *Image*D
	/**
	 *  @see Width
	 *  @see Depth
	 */
	static GLsizei Height(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(target, level, GL_TEXTURE_HEIGHT));
	}

	/// Returns the depth of the texture as it was specified by *Image*D
	/**
	 *  @see Width
	 *  @see Height
	 */
	static GLsizei Depth(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(target, level, GL_TEXTURE_DEPTH));
	}

	/// Returns the data type used to store the RED component
	static PixelDataType RedType(Target target, GLint level = 0)
	{
		return PixelDataType(GetIntParam(
			target,
			level,
			GL_TEXTURE_RED_TYPE
		));
	}

	/// Returns the data type used to store the GREEN component
	static PixelDataType GreenType(Target target, GLint level = 0)
	{
		return PixelDataType(GetIntParam(
			target,
			level,
			GL_TEXTURE_GREEN_TYPE
		));
	}

	/// Returns the data type used to store the BLUE component
	static PixelDataType BlueType(Target target, GLint level = 0)
	{
		return PixelDataType(GetIntParam(
			target,
			level,
			GL_TEXTURE_BLUE_TYPE
		));
	}

	/// Returns the data type used to store the ALPHA component
	static PixelDataType AlphaType(Target target, GLint level = 0)
	{
		return PixelDataType(GetIntParam(
			target,
			level,
			GL_TEXTURE_ALPHA_TYPE
		));
	}

	/// Returns the data type used to store the DEPTH component
	static PixelDataType DepthType(Target target, GLint level = 0)
	{
		return PixelDataType(GetIntParam(
			target,
			level,
			GL_TEXTURE_DEPTH_TYPE
		));
	}

	/// Returns the actual resolution of the RED component
	static GLsizei RedSize(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(
			target,
			level,
			GL_TEXTURE_RED_SIZE
		));
	}

	/// Returns the actual resolution of the GREEN component
	static GLsizei GreenSize(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(
			target,
			level,
			GL_TEXTURE_GREEN_SIZE
		));
	}

	/// Returns the actual resolution of the BLUE component
	static GLsizei BlueSize(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(
			target,
			level,
			GL_TEXTURE_BLUE_SIZE
		));
	}

	/// Returns the actual resolution of the ALPHA component
	static GLsizei AlphaSize(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(
			target,
			level,
			GL_TEXTURE_ALPHA_SIZE
		));
	}

	/// Returns the actual resolution of the DEPTH component
	static GLsizei DepthSize(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(
			target,
			level,
			GL_TEXTURE_DEPTH_SIZE
		));
	}

	/// Returns the actual resolution of the STENCIL component
	static GLsizei StencilSize(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(
			target,
			level,
			GL_TEXTURE_STENCIL_SIZE
		));
	}

	static GLsizei SharedSize(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(
			target,
			level,
			GL_TEXTURE_SHARED_SIZE
		));
	}

	/// Returns the size (in bytes) of the image array if it is compressed
	static GLsizei CompressedImageSize(Target target, GLint level = 0)
	{
		return GLsizei(GetIntParam(
			target,
			level,
			GL_TEXTURE_COMPRESSED_IMAGE_SIZE
		));
	}

	/// Returns the internal data format of the image array
	static PixelDataInternalFormat InternalFormat(
		Target target,
		GLint level = 0
	)
	{
		return PixelDataInternalFormat(GetIntParam(
			target,
			level,
			GL_TEXTURE_INTERNAL_FORMAT
		));
	}

	/// Specifies a three dimensional texture image
	static void Image3D(
		Target target,
		GLint level,
		PixelDataInternalFormat internal_format,
		GLsizei width,
		GLsizei height,
		GLsizei depth,
		GLint border,
		PixelDataFormat format,
		PixelDataType type,
		const void* data
	)
	{
		::glTexImage3D(
			GLenum(target),
			level,
			GLint(internal_format),
			width,
			height,
			depth,
			border,
			GLenum(format),
			GLenum(type),
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexImage3D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a three dimensional texture image
	template <typename T>
	static void Image3D(
		Target target,
		const Image<T>& image,
		GLint level = 0,
		GLint border = 0
	)
	{
		::glTexImage3D(
			GLenum(target),
			level,
			GLint(image.InternalFormat()),
			image.Width(),
			image.Height(),
			image.Depth(),
			border,
			GLenum(image.Format()),
			GLenum(image.Type()),
			image.RawData()
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexImage3D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a three dimensional texture sub image
	static void SubImage3D(
		Target target,
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		GLsizei width,
		GLsizei height,
		GLsizei depth,
		PixelDataFormat format,
		PixelDataType type,
		const void* data
	)
	{
		::glTexSubImage3D(
			GLenum(target),
			level,
			xoffs,
			yoffs,
			zoffs,
			width,
			height,
			depth,
			GLenum(format),
			GLenum(type),
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexSubImage3D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a three dimensional texture sub image
	template <typename T>
	static void SubImage3D(
		Target target,
		const Image<T>& image,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		GLint level = 0
	)
	{
		::glTexSubImage3D(
			GLenum(target),
			level,
			xoffs,
			yoffs,
			zoffs,
			image.Width(),
			image.Height(),
			image.Depth(),
			GLenum(image.Format()),
			GLenum(image.Type()),
			image.RawData()
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexSubImage3D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a two dimensional texture image
	static void Image2D(
		Target target,
		GLint level,
		PixelDataInternalFormat internal_format,
		GLsizei width,
		GLsizei height,
		GLint border,
		PixelDataFormat format,
		PixelDataType type,
		const void* data
	)
	{
		::glTexImage2D(
			GLenum(target),
			level,
			GLint(internal_format),
			width,
			height,
			border,
			GLenum(format),
			GLenum(type),
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexImage2D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a two dimensional texture image
	template <typename T>
	static void Image2D(
		Target target,
		const Image<T>& image,
		GLint level = 0,
		GLint border = 0
	)
	{
		::glTexImage2D(
			GLenum(target),
			level,
			GLint(image.InternalFormat()),
			image.Width(),
			image.Height(),
			border,
			GLenum(image.Format()),
			GLenum(image.Type()),
			image.RawData()
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexImage2D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a two dimensional texture sub image
	static void SubImage2D(
		Target target,
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLsizei width,
		GLsizei height,
		PixelDataFormat format,
		PixelDataType type,
		const void* data
	)
	{
		::glTexSubImage2D(
			GLenum(target),
			level,
			xoffs,
			yoffs,
			width,
			height,
			GLenum(format),
			GLenum(type),
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexSubImage2D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a two dimensional texture sub image
	template <typename T>
	static void SubImage2D(
		Target target,
		const Image<T>& image,
		GLint xoffs,
		GLint yoffs,
		GLint level = 0
	)
	{
		::glTexSubImage2D(
			GLenum(target),
			level,
			xoffs,
			yoffs,
			image.Width(),
			image.Height(),
			GLenum(image.Format()),
			GLenum(image.Type()),
			image.RawData()
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexSubImage2D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a one dimensional texture image
	static void Image1D(
		Target target,
		GLint level,
		PixelDataInternalFormat internal_format,
		GLsizei width,
		GLsizei height,
		GLint border,
		PixelDataFormat format,
		PixelDataType type,
		const void* data
	)
	{
		::glTexImage1D(
			GLenum(target),
			level,
			GLint(internal_format),
			width,
			border,
			GLenum(format),
			GLenum(type),
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexImage1D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a one dimensional texture image
	template <typename T>
	static void Image1D(
		Target target,
		const Image<T>& image,
		GLint level = 0,
		GLint border = 0
	)
	{
		::glTexImage1D(
			GLenum(target),
			level,
			GLint(image.InternalFormat()),
			image.Width(),
			border,
			GLenum(image.Format()),
			GLenum(image.Type()),
			image.RawData()
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexImage1D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a one dimensional texture sub image
	static void SubImage1D(
		Target target,
		GLint level,
		GLint xoffs,
		GLsizei width,
		PixelDataFormat format,
		PixelDataType type,
		const void* data
	)
	{
		::glTexSubImage1D(
			GLenum(target),
			level,
			xoffs,
			width,
			GLenum(format),
			GLenum(type),
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexSubImage1D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a two dimensional texture sub image
	template <typename T>
	static void SubImage1D(
		Target target,
		const Image<T>& image,
		GLint xoffs,
		GLint level = 0
	)
	{
		::glTexSubImage1D(
			GLenum(target),
			level,
			xoffs,
			image.Width(),
			GLenum(image.Format()),
			GLenum(image.Type()),
			image.RawData()
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexSubImage1D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Copies a two dimensional texture image from the framebuffer
	static void CopyImage2D(
		Target target,
		GLint level,
		PixelDataInternalFormat internal_format,
		GLint x,
		GLint y,
		GLsizei width,
		GLsizei height,
		GLint border
	)
	{
		::glCopyTexImage2D(
			GLenum(target),
			level,
			GLint(internal_format),
			x,
			y,
			width,
			height,
			border
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CopyTexImage2D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Copies a one dimensional texture image from the framebuffer
	static void CopyImage1D(
		Target target,
		GLint level,
		PixelDataInternalFormat internal_format,
		GLint x,
		GLint y,
		GLsizei width,
		GLint border
	)
	{
		::glCopyTexImage1D(
			GLenum(target),
			level,
			GLint(internal_format),
			x,
			y,
			width,
			border
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CopyTexImage1D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Copies a three dimensional texture sub image from the framebuffer
	static void CopySubImage3D(
		Target target,
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		GLint x,
		GLint y,
		GLsizei width,
		GLsizei height
	)
	{
		::glCopyTexSubImage3D(
			GLenum(target),
			level,
			xoffs,
			yoffs,
			zoffs,
			x,
			y,
			width,
			height
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CopyTexSubImage3D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Copies a two dimensional texture sub image from the framebuffer
	static void CopySubImage2D(
		Target target,
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint x,
		GLint y,
		GLsizei width,
		GLsizei height
	)
	{
		::glCopyTexSubImage2D(
			GLenum(target),
			level,
			xoffs,
			yoffs,
			x,
			y,
			width,
			height
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CopyTexSubImage2D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Copies a one dimensional texture sub image from the framebuffer
	static void CopySubImage1D(
		Target target,
		GLint level,
		GLint xoffs,
		GLint x,
		GLint y,
		GLsizei width
	)
	{
		::glCopyTexSubImage1D(
			GLenum(target),
			level,
			xoffs,
			x,
			y,
			width
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CopyTexSubImage1D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a three dimensional compressed texture image
	static void CompressedImage3D(
		Target target,
		GLint level,
		PixelDataInternalFormat internal_format,
		GLsizei width,
		GLsizei height,
		GLsizei depth,
		GLint border,
		GLsizei image_size,
		const void* data
	)
	{
		::glCompressedTexImage3D(
			GLenum(target),
			level,
			GLint(internal_format),
			width,
			height,
			depth,
			border,
			image_size,
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CompressedTexImage3D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a two dimensional compressed texture image
	static void CompressedImage2D(
		Target target,
		GLint level,
		PixelDataInternalFormat internal_format,
		GLsizei width,
		GLsizei height,
		GLint border,
		GLsizei image_size,
		const void* data
	)
	{
		::glCompressedTexImage2D(
			GLenum(target),
			level,
			GLint(internal_format),
			width,
			height,
			border,
			image_size,
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CompressedTexImage2D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a one dimensional compressed texture image
	static void CompressedImage1D(
		Target target,
		GLint level,
		PixelDataInternalFormat internal_format,
		GLsizei width,
		GLint border,
		GLsizei image_size,
		const void* data
	)
	{
		::glCompressedTexImage1D(
			GLenum(target),
			level,
			GLint(internal_format),
			width,
			border,
			image_size,
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CompressedTexImage1D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a three dimensional compressed texture sub image
	static void CompressedSubImage3D(
		Target target,
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		GLsizei width,
		GLsizei height,
		GLsizei depth,
		PixelDataFormat format,
		GLsizei image_size,
		const void* data
	)
	{
		::glCompressedTexSubImage3D(
			GLenum(target),
			level,
			xoffs,
			yoffs,
			zoffs,
			width,
			height,
			depth,
			GLint(format),
			image_size,
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CompressedTexSubImage3D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a two dimensional compressed texture sub image
	static void CompressedSubImage2D(
		Target target,
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLsizei width,
		GLsizei height,
		PixelDataFormat format,
		GLsizei image_size,
		const void* data
	)
	{
		::glCompressedTexSubImage2D(
			GLenum(target),
			level,
			xoffs,
			yoffs,
			width,
			height,
			GLint(format),
			image_size,
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CompressedTexSubImage2D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Specifies a one dimensional compressed texture sub image
	static void CompressedSubImage1D(
		Target target,
		GLint level,
		GLint xoffs,
		GLsizei width,
		PixelDataFormat format,
		GLsizei image_size,
		const void* data
	)
	{
		::glCompressedTexSubImage1D(
			GLenum(target),
			level,
			xoffs,
			width,
			GLint(format),
			image_size,
			data
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			CompressedTexSubImage1D,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets-up a three dimensional multisample texture
	static void Image3DMultisample(
		Target target,
		GLsizei samples,
		PixelDataInternalFormat internal_format,
		GLsizei width,
		GLsizei height,
		GLsizei depth,
		bool fixed_sample_locations
	)
	{
		::glTexImage3DMultisample(
			GLenum(target),
			samples,
			GLint(internal_format),
			width,
			height,
			depth,
			fixed_sample_locations ? GL_TRUE : GL_FALSE
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexImage3DMultisample,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets-up a two dimensional multisample texture
	static void Image2DMultisample(
		Target target,
		GLsizei samples,
		PixelDataInternalFormat internal_format,
		GLsizei width,
		GLsizei height,
		bool fixed_sample_locations
	)
	{
		::glTexImage2DMultisample(
			GLenum(target),
			samples,
			GLint(internal_format),
			width,
			height,
			fixed_sample_locations ? GL_TRUE : GL_FALSE
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexImage2DMultisample,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Assigns a buffer storing the texel data to the texture
	static void SetBuffer(
		Target target,
		PixelDataInternalFormat internal_format,
		const Buffer& buffer
	)
	{
		::glTexBuffer(
			GLenum(target),
			GLenum(internal_format),
			FriendOf<Buffer>::GetName(buffer)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexBuffer,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the texture base level (TEXTURE_BASE_LEVEL)
	static void BaseLevel(Target target, GLuint level)
	{
		::glTexParameteri(
			GLenum(target),
			GL_TEXTURE_BASE_LEVEL,
			level
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameteri,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the texture border color (TEXTURE_BORDER_COLOR)
	static void BorderColor(Target target, Vector<GLfloat, 4> color)
	{
		::glTexParameterfv(
			GLenum(target),
			GL_TEXTURE_BORDER_COLOR,
			Data(color)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameterfv,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the texture border color (TEXTURE_BORDER_COLOR)
	static void BorderColor(Target target, Vector<GLint, 4> color)
	{
		::glTexParameterIiv(
			GLenum(target),
			GL_TEXTURE_BORDER_COLOR,
			Data(color)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameterIiv,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the texture border color (TEXTURE_BORDER_COLOR)
	static void BorderColor(Target target, Vector<GLuint, 4> color)
	{
		::glTexParameterIuiv(
			GLenum(target),
			GL_TEXTURE_BORDER_COLOR,
			Data(color)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameterIuiv,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the compare mode (TEXTURE_COMPARE_MODE)
	static void CompareMode(Target target, TextureCompareMode mode)
	{
		::glTexParameteri(
			GLenum(target),
			GL_TEXTURE_COMPARE_MODE,
			GLenum(mode)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameteri,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the compare function (TEXTURE_COMPARE_FUNC)
	static void CompareFunc(Target target, CompareFunction func)
	{
		::glTexParameteri(
			GLenum(target),
			GL_TEXTURE_COMPARE_FUNC,
			GLenum(func)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameteri,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the LOD bias value (TEXTURE_LOD_BIAS)
	static void LODBias(Target target, GLfloat value)
	{
		::glTexParameterf(
			GLenum(target),
			GL_TEXTURE_LOD_BIAS,
			value
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameterf,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the magnification filter (TEXTURE_MAG_FILTER)
	static void MagFilter(Target target, TextureMagFilter filter)
	{
		::glTexParameteri(
			GLenum(target),
			GL_TEXTURE_MAG_FILTER,
			GLenum(filter)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameteri,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the minnification filter (TEXTURE_MAG_FILTER)
	static void MinFilter(Target target, TextureMinFilter filter)
	{
		::glTexParameteri(
			GLenum(target),
			GL_TEXTURE_MIN_FILTER,
			GLenum(filter)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameteri,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets minimal LOD value (TEXTURE_MIN_LOD)
	static void MinLOD(Target target, GLfloat value)
	{
		::glTexParameterf(
			GLenum(target),
			GL_TEXTURE_MIN_LOD,
			value
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameterf,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets maximal LOD value (TEXTURE_MAX_LOD)
	static void MaxLOD(Target target, GLfloat value)
	{
		::glTexParameterf(
			GLenum(target),
			GL_TEXTURE_MAX_LOD,
			value
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameterf,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the swizzle parameter (TEXTURE_SWIZZLE_*)
	static void Swizzle(
		Target target,
		TextureSwizzleCoord coord,
		TextureSwizzle mode
	)
	{
		::glTexParameteri(
			GLenum(target),
			GLenum(coord),
			GLenum(mode)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameteri,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the swizzle parameter (TEXTURE_SWIZZLE_R)
	static void SwizzleR(Target target, TextureSwizzle mode)
	{
		Swizzle(target, TextureSwizzleCoord::R, mode);
	}

	/// Sets the swizzle parameter (TEXTURE_SWIZZLE_G)
	static void SwizzleG(Target target, TextureSwizzle mode)
	{
		Swizzle(target, TextureSwizzleCoord::G, mode);
	}

	/// Sets the swizzle parameter (TEXTURE_SWIZZLE_B)
	static void SwizzleB(Target target, TextureSwizzle mode)
	{
		Swizzle(target, TextureSwizzleCoord::B, mode);
	}

	/// Sets the swizzle parameter (TEXTURE_SWIZZLE_A)
	static void SwizzleA(Target target, TextureSwizzle mode)
	{
		Swizzle(target, TextureSwizzleCoord::A, mode);
	}

	/// Sets the swizzle parameter (TEXTURE_SWIZZLE_RGBA)
	static void SwizzleRGBA(Target target, TextureSwizzle mode)
	{
		Swizzle(target, TextureSwizzleCoord::RGBA, mode);
	}

	/// Sets the wrap parameter (TEXTURE_WRAP_*)
	static void Wrap(
		Target target,
		TextureWrapCoord coord,
		TextureWrap mode
	)
	{
		::glTexParameteri(
			GLenum(target),
			GLenum(coord),
			GLenum(mode)
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			TexParameteri,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}

	/// Sets the wrap parameter (TEXTURE_WRAP_S)
	static void WrapS(Target target, TextureWrap mode)
	{
		Wrap(target, TextureWrapCoord::S, mode);
	}

	/// Sets the wrap parameter (TEXTURE_WRAP_T)
	static void WrapT(Target target, TextureWrap mode)
	{
		Wrap(target, TextureWrapCoord::T, mode);
	}

	/// Sets the wrap parameter (TEXTURE_WRAP_R)
	static void WrapR(Target target, TextureWrap mode)
	{
		Wrap(target, TextureWrapCoord::R, mode);
	}

	/// Generate mipmap for the texture bound to the specified target
	static void GenerateMipmap(Target target)
	{
		::glGenerateMipmap(GLenum(target));
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			GenerateMipmap,
			Texture,
			BindingQuery<TextureOps>::QueryBinding(target)
		));
	}
};

#ifdef OGLPLUS_DOCUMENTATION_ONLY
/// An @ref oglplus_object encapsulating the OpenGL texture functionality
/**
 *  @ingroup objects
 */
class Texture
 : public TextureOps
{ };
#else
typedef Object<TextureOps, true> Texture;
#endif

} // namespace oglplus

#endif // include guard
