////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Copyright (c) 2018 Nicholas Frechette & Animation Compression Library contributors
// Copyright (c) 2018 Nicholas Frechette & Realtime Math contributors
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

#include "test_vector4_impl.h"

TEST_CASE("vector4 64 math", "[math][vector4]")
{
	test_vector4_impl<vector4d, quatd, double>(vector_zero_64(), quat_identity_64(), 1.0e-9);

	const vector4d src = vector_set(-2.65, 2.996113, 0.68123521, -5.9182);
	const vector4f dst = vector_cast(src);
	REQUIRE(scalar_near_equal(vector_get_x(dst), -2.65f, 1.0e-6f));
	REQUIRE(scalar_near_equal(vector_get_y(dst), 2.996113f, 1.0e-6f));
	REQUIRE(scalar_near_equal(vector_get_z(dst), 0.68123521f, 1.0e-6f));
	REQUIRE(scalar_near_equal(vector_get_w(dst), -5.9182f, 1.0e-6f));
}
