#pragma once

////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Copyright (c) 2019 Nicholas Frechette & Realtime Math contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Because this library is made entirely of headers, we have no control over the
// compilation flags used. However, in some cases, certain options must be forced.
// To do this, every header is wrapped in two macros to push and pop the necessary
// pragmas.
//////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) && !defined(__clang__)
	#define RTM_IMPL_FILE_PRAGMA_PUSH \
		/* Disable fast math, it can hurt precision for little to no performance gain due to the heavy usage of intrinsics. */ \
		__pragma(float_control(precise, on, push))

	#define RTM_IMPL_FILE_PRAGMA_POP \
		__pragma(float_control(pop))
#else
	#define RTM_IMPL_FILE_PRAGMA_PUSH
	#define RTM_IMPL_FILE_PRAGMA_POP
#endif

//////////////////////////////////////////////////////////////////////////
// Force inline macros for when it is necessary.
//////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) && !defined(__clang__)
	#define RTM_FORCE_INLINE __forceinline
#elif defined(__GNUG__) || defined(__clang__)
	#define RTM_FORCE_INLINE __attribute__((always_inline)) inline
#else
	#define RTM_FORCE_INLINE inline
#endif

//////////////////////////////////////////////////////////////////////////
// Force no-inline macros for when it is necessary.
//////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) && !defined(__clang__)
	#define RTM_FORCE_NOINLINE __declspec(noinline)
#elif defined(__GNUG__) || defined(__clang__)
	#define RTM_FORCE_NOINLINE __attribute__((noinline))
#else
	#define RTM_FORCE_NOINLINE
#endif

//////////////////////////////////////////////////////////////////////////
// Macro to identity GCC
//////////////////////////////////////////////////////////////////////////
#if defined(__GNUG__) && !defined(__clang__)
	#define RTM_COMPILER_GCC
#endif

//////////////////////////////////////////////////////////////////////////
// Macro to identity Clang
//////////////////////////////////////////////////////////////////////////
#if defined(__clang__)
	#define RTM_COMPILER_CLANG
#endif

//////////////////////////////////////////////////////////////////////////
// Macro to identity MSVC
//////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) && !defined(__clang__)
	#define RTM_COMPILER_MSVC
#endif
