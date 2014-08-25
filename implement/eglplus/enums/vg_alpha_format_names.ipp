/*
 *  .file eglplus/enums/vg_alpha_format_names.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/eglplus/vg_alpha_format.txt' instead.
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	VGAlphaFormat*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_VGALPHAFORMAT)
#define EGLPLUS_IMPL_EVN_VGALPHAFORMAT
{
switch(value)
{
#if defined EGL_VG_ALPHA_FORMAT_PRE
	case EGL_VG_ALPHA_FORMAT_PRE: return StrCRef("VG_ALPHA_FORMAT_PRE");
#endif
#if defined EGL_VG_ALPHA_FORMAT_NONPRE
	case EGL_VG_ALPHA_FORMAT_NONPRE: return StrCRef("VG_ALPHA_FORMAT_NONPRE");
#endif
	default:;
}
EGLPLUS_FAKE_USE(value);
return StrCRef();
}
#else
;
#endif
} // namespace enums
