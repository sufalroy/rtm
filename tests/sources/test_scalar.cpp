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

#include <catch.hpp>

#include <rtm/scalar_32.h>
#include <rtm/scalar_64.h>

#include <limits>

using namespace rtm;

template<typename FloatType>
static void test_scalar_impl(const FloatType pi, const FloatType threshold)
{
	const FloatType half_pi = pi * FloatType(0.5);
	const FloatType two_pi = pi * FloatType(2.0);

	REQUIRE(rtm::floor(FloatType(0.0)) == FloatType(0.0));
	REQUIRE(rtm::floor(FloatType(0.5)) == FloatType(0.0));
	REQUIRE(rtm::floor(FloatType(2.5)) == FloatType(2.0));
	REQUIRE(rtm::floor(FloatType(3.0)) == FloatType(3.0));
	REQUIRE(rtm::floor(FloatType(-0.5)) == FloatType(-1.0));
	REQUIRE(rtm::floor(FloatType(-2.5)) == FloatType(-3.0));
	REQUIRE(rtm::floor(FloatType(-3.0)) == FloatType(-3.0));

	REQUIRE(rtm::ceil(FloatType(0.0)) == FloatType(0.0));
	REQUIRE(rtm::ceil(FloatType(0.5)) == FloatType(1.0));
	REQUIRE(rtm::ceil(FloatType(2.5)) == FloatType(3.0));
	REQUIRE(rtm::ceil(FloatType(3.0)) == FloatType(3.0));
	REQUIRE(rtm::ceil(FloatType(-0.5)) == FloatType(0.0));
	REQUIRE(rtm::ceil(FloatType(-2.5)) == FloatType(-2.0));
	REQUIRE(rtm::ceil(FloatType(-3.0)) == FloatType(-3.0));

	REQUIRE(clamp(FloatType(0.5), FloatType(0.0), FloatType(1.0)) == FloatType(0.5));
	REQUIRE(clamp(FloatType(-0.5), FloatType(0.0), FloatType(1.0)) == FloatType(0.0));
	REQUIRE(clamp(FloatType(1.5), FloatType(0.0), FloatType(1.0)) == FloatType(1.0));

	REQUIRE(rtm::abs(FloatType(0.0)) == FloatType(0.0));
	REQUIRE(rtm::abs(FloatType(2.0)) == FloatType(2.0));
	REQUIRE(rtm::abs(FloatType(-2.0)) == FloatType(2.0));

	REQUIRE(scalar_near_equal(FloatType(1.0), FloatType(1.0), FloatType(0.00001)) == true);
	REQUIRE(scalar_near_equal(FloatType(1.0), FloatType(1.000001), FloatType(0.00001)) == true);
	REQUIRE(scalar_near_equal(FloatType(1.0), FloatType(0.999999), FloatType(0.00001)) == true);
	REQUIRE(scalar_near_equal(FloatType(1.0), FloatType(1.001), FloatType(0.00001)) == false);
	REQUIRE(scalar_near_equal(FloatType(1.0), FloatType(0.999), FloatType(0.00001)) == false);

	REQUIRE(rtm::sqrt(FloatType(0.0)) == FloatType(0.0));
	REQUIRE(scalar_near_equal(rtm::sqrt(FloatType(0.5)), std::sqrt(FloatType(0.5)), threshold));
	REQUIRE(scalar_near_equal(rtm::sqrt(FloatType(32.5)), std::sqrt(FloatType(32.5)), threshold));

	REQUIRE(scalar_near_equal(rtm::sqrt_reciprocal(FloatType(0.5)), FloatType(1.0) / std::sqrt(FloatType(0.5)), threshold));
	REQUIRE(scalar_near_equal(rtm::sqrt_reciprocal(FloatType(32.5)), FloatType(1.0) / std::sqrt(FloatType(32.5)), threshold));

	REQUIRE(scalar_near_equal(rtm::reciprocal(FloatType(0.5)), FloatType(1.0 / 0.5), threshold));
	REQUIRE(scalar_near_equal(rtm::reciprocal(FloatType(32.5)), FloatType(1.0 / 32.5), threshold));
	REQUIRE(scalar_near_equal(rtm::reciprocal(FloatType(-0.5)), FloatType(1.0 / -0.5), threshold));
	REQUIRE(scalar_near_equal(rtm::reciprocal(FloatType(-32.5)), FloatType(1.0 / -32.5), threshold));

	const FloatType angles[] = { FloatType(0.0), pi, -pi, half_pi, -half_pi, FloatType(0.5), FloatType(32.5), FloatType(-0.5), FloatType(-32.5) };

	for (const FloatType angle : angles)
	{
		REQUIRE(scalar_near_equal(rtm::sin(angle), std::sin(angle), threshold));
		REQUIRE(scalar_near_equal(rtm::cos(angle), std::cos(angle), threshold));

		FloatType sin_result;
		FloatType cos_result;
		rtm::sincos(angle, sin_result, cos_result);
		REQUIRE(scalar_near_equal(sin_result, std::sin(angle), threshold));
		REQUIRE(scalar_near_equal(cos_result, std::cos(angle), threshold));
	}

	REQUIRE(scalar_near_equal(rtm::acos(FloatType(-1.0)), std::acos(FloatType(-1.0)), threshold));
	REQUIRE(scalar_near_equal(rtm::acos(FloatType(-0.75)), std::acos(FloatType(-0.75)), threshold));
	REQUIRE(scalar_near_equal(rtm::acos(FloatType(-0.5)), std::acos(FloatType(-0.5)), threshold));
	REQUIRE(scalar_near_equal(rtm::acos(FloatType(-0.25)), std::acos(-FloatType(0.25)), threshold));
	REQUIRE(scalar_near_equal(rtm::acos(FloatType(0.0)), std::acos(FloatType(0.0)), threshold));
	REQUIRE(scalar_near_equal(rtm::acos(FloatType(0.25)), std::acos(FloatType(0.25)), threshold));
	REQUIRE(scalar_near_equal(rtm::acos(FloatType(0.5)), std::acos(FloatType(0.5)), threshold));
	REQUIRE(scalar_near_equal(rtm::acos(FloatType(0.75)), std::acos(FloatType(0.75)), threshold));
	REQUIRE(scalar_near_equal(rtm::acos(FloatType(1.0)), std::acos(FloatType(1.0)), threshold));

	REQUIRE(scalar_near_equal(rtm::atan2(FloatType(-2.0), FloatType(-2.0)), std::atan2(FloatType(-2.0), FloatType(-2.0)), threshold));
	REQUIRE(scalar_near_equal(rtm::atan2(FloatType(-1.0), FloatType(-2.0)), std::atan2(FloatType(-1.0), FloatType(-2.0)), threshold));
	REQUIRE(scalar_near_equal(rtm::atan2(FloatType(-2.0), FloatType(-1.0)), std::atan2(FloatType(-2.0), FloatType(-1.0)), threshold));
	REQUIRE(scalar_near_equal(rtm::atan2(FloatType(2.0), FloatType(2.0)), std::atan2(FloatType(2.0), FloatType(2.0)), threshold));
	REQUIRE(scalar_near_equal(rtm::atan2(FloatType(1.0), FloatType(2.0)), std::atan2(FloatType(1.0), FloatType(2.0)), threshold));
	REQUIRE(scalar_near_equal(rtm::atan2(FloatType(2.0), FloatType(1.0)), std::atan2(FloatType(2.0), FloatType(1.0)), threshold));

	REQUIRE(rtm::min(FloatType(-0.5), FloatType(1.0)) == FloatType(-0.5));
	REQUIRE(rtm::min(FloatType(1.0), FloatType(-0.5)) == FloatType(-0.5));
	REQUIRE(rtm::min(FloatType(1.0), FloatType(1.0)) == FloatType(1.0));

	REQUIRE(rtm::max(FloatType(-0.5), FloatType(1.0)) == FloatType(1.0));
	REQUIRE(rtm::max(FloatType(1.0), FloatType(-0.5)) == FloatType(1.0));
	REQUIRE(rtm::max(FloatType(1.0), FloatType(1.0)) == FloatType(1.0));

	REQUIRE(deg2rad(FloatType(0.0)) == FloatType(0.0));
	REQUIRE(scalar_near_equal(deg2rad(FloatType(90.0)), half_pi, threshold));
	REQUIRE(scalar_near_equal(deg2rad(FloatType(-90.0)), -half_pi, threshold));
	REQUIRE(scalar_near_equal(deg2rad(FloatType(180.0)), pi, threshold));
	REQUIRE(scalar_near_equal(deg2rad(FloatType(-180.0)), -pi, threshold));
	REQUIRE(scalar_near_equal(deg2rad(FloatType(360.0)), two_pi, threshold));
	REQUIRE(scalar_near_equal(deg2rad(FloatType(-360.0)), -two_pi, threshold));

	REQUIRE(rtm::is_finite(FloatType(0.0)) == true);
	REQUIRE(rtm::is_finite(FloatType(32.0)) == true);
	REQUIRE(rtm::is_finite(FloatType(-32.0)) == true);
	REQUIRE(rtm::is_finite(std::numeric_limits<FloatType>::infinity()) == false);
	REQUIRE(rtm::is_finite(std::numeric_limits<FloatType>::quiet_NaN()) == false);
	REQUIRE(rtm::is_finite(std::numeric_limits<FloatType>::signaling_NaN()) == false);

	REQUIRE(symmetric_round(FloatType(-1.75)) == FloatType(-2.0));
	REQUIRE(symmetric_round(FloatType(-1.5)) == FloatType(-2.0));
	REQUIRE(symmetric_round(FloatType(-1.4999)) == FloatType(-1.0));
	REQUIRE(symmetric_round(FloatType(-0.5)) == FloatType(-1.0));
	REQUIRE(symmetric_round(FloatType(-0.4999)) == FloatType(0.0));
	REQUIRE(symmetric_round(FloatType(0.0)) == FloatType(0.0));
	REQUIRE(symmetric_round(FloatType(0.4999)) == FloatType(0.0));
	REQUIRE(symmetric_round(FloatType(0.5)) == FloatType(1.0));
	REQUIRE(symmetric_round(FloatType(1.4999)) == FloatType(1.0));
	REQUIRE(symmetric_round(FloatType(1.5)) == FloatType(2.0));
	REQUIRE(symmetric_round(FloatType(1.75)) == FloatType(2.0));

	REQUIRE(fraction(FloatType(0.0)) == FloatType(0.0));
	REQUIRE(fraction(FloatType(1.0)) == FloatType(0.0));
	REQUIRE(fraction(FloatType(-1.0)) == FloatType(0.0));
	REQUIRE(scalar_near_equal(fraction(FloatType(0.25)), FloatType(0.25), threshold));
	REQUIRE(scalar_near_equal(fraction(FloatType(0.5)), FloatType(0.5), threshold));
	REQUIRE(scalar_near_equal(fraction(FloatType(0.75)), FloatType(0.75), threshold));
}

TEST_CASE("scalar 32 math", "[math][scalar]")
{
	test_scalar_impl<float>(rtm::k_pi, 1.0e-6f);
}

TEST_CASE("scalar 64 math", "[math][scalar]")
{
	test_scalar_impl<double>(rtm::k_pi, 1.0e-9);
}
