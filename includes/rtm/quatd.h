#pragma once

////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Copyright (c) 2017 Nicholas Frechette & Animation Compression Library contributors
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

#include "rtm/error.h"
#include "rtm/math.h"
#include "rtm/scalard.h"
#include "rtm/vector4d.h"
#include "rtm/impl/memory_utils.h"

namespace rtm
{
	//////////////////////////////////////////////////////////////////////////
	// Setters, getters, and casts

	inline quatd quat_set(double x, double y, double z, double w) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return quatd{ _mm_set_pd(y, x), _mm_set_pd(w, z) };
#else
		return quatd{ x, y, z, w };
#endif
	}

	inline quatd quat_unaligned_load(const double* input) RTM_NO_EXCEPT
	{
		return quat_set(input[0], input[1], input[2], input[3]);
	}

	inline quatd quat_identity_64()
	{
		return quat_set(0.0, 0.0, 0.0, 1.0);
	}

	inline quatd vector_to_quat(const vector4d& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return quatd{ input.xy, input.zw };
#else
		return quatd{ input.x, input.y, input.z, input.w };
#endif
	}

	inline quatd quat_cast(const quatf& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return quatd{ _mm_cvtps_pd(input), _mm_cvtps_pd(_mm_shuffle_ps(input, input, _MM_SHUFFLE(3, 2, 3, 2))) };
#elif defined(RTM_NEON_INTRINSICS)
		return quatd{ double(vgetq_lane_f32(input, 0)), double(vgetq_lane_f32(input, 1)), double(vgetq_lane_f32(input, 2)), double(vgetq_lane_f32(input, 3)) };
#else
		return quatd{ double(input.x), double(input.y), double(input.z), double(input.w) };
#endif
	}

	inline double quat_get_x(const quatd& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return _mm_cvtsd_f64(input.xy);
#else
		return input.x;
#endif
	}

	inline double quat_get_y(const quatd& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return _mm_cvtsd_f64(_mm_shuffle_pd(input.xy, input.xy, 1));
#else
		return input.y;
#endif
	}

	inline double quat_get_z(const quatd& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return _mm_cvtsd_f64(input.zw);
#else
		return input.z;
#endif
	}

	inline double quat_get_w(const quatd& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return _mm_cvtsd_f64(_mm_shuffle_pd(input.zw, input.zw, 1));
#else
		return input.w;
#endif
	}

	inline void quat_unaligned_write(const quatd& input, double* output) RTM_NO_EXCEPT
	{
		RTM_ASSERT(rtm_impl::is_aligned(output), "Invalid alignment");
		output[0] = quat_get_x(input);
		output[1] = quat_get_y(input);
		output[2] = quat_get_z(input);
		output[3] = quat_get_w(input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Arithmetic

	inline quatd quat_conjugate(const quatd& input) RTM_NO_EXCEPT
	{
		return quat_set(-quat_get_x(input), -quat_get_y(input), -quat_get_z(input), quat_get_w(input));
	}

	// Multiplication order is as follow: local_to_world = quat_mul(local_to_object, object_to_world)
	inline quatd quat_mul(const quatd& lhs, const quatd& rhs) RTM_NO_EXCEPT
	{
		double lhs_x = quat_get_x(lhs);
		double lhs_y = quat_get_y(lhs);
		double lhs_z = quat_get_z(lhs);
		double lhs_w = quat_get_w(lhs);

		double rhs_x = quat_get_x(rhs);
		double rhs_y = quat_get_y(rhs);
		double rhs_z = quat_get_z(rhs);
		double rhs_w = quat_get_w(rhs);

		double x = (rhs_w * lhs_x) + (rhs_x * lhs_w) + (rhs_y * lhs_z) - (rhs_z * lhs_y);
		double y = (rhs_w * lhs_y) - (rhs_x * lhs_z) + (rhs_y * lhs_w) + (rhs_z * lhs_x);
		double z = (rhs_w * lhs_z) + (rhs_x * lhs_y) - (rhs_y * lhs_x) + (rhs_z * lhs_w);
		double w = (rhs_w * lhs_w) - (rhs_x * lhs_x) - (rhs_y * lhs_y) - (rhs_z * lhs_z);

		return quat_set(x, y, z, w);
	}

	inline vector4d quat_rotate(const quatd& rotation, const vector4d& vector) RTM_NO_EXCEPT
	{
		quatd vector_quat = quat_set(vector_get_x(vector), vector_get_y(vector), vector_get_z(vector), 0.0);
		quatd inv_rotation = quat_conjugate(rotation);
		return quat_to_vector(quat_mul(quat_mul(inv_rotation, vector_quat), rotation));
	}

	inline double quat_length_squared(const quatd& input) RTM_NO_EXCEPT
	{
		// TODO: Use dot instruction
		return (quat_get_x(input) * quat_get_x(input)) + (quat_get_y(input) * quat_get_y(input)) + (quat_get_z(input) * quat_get_z(input)) + (quat_get_w(input) * quat_get_w(input));
	}

	inline double quat_length(const quatd& input) RTM_NO_EXCEPT
	{
		// TODO: Use intrinsics to avoid scalar coercion
		return scalar_sqrt(quat_length_squared(input));
	}

	inline double quat_length_reciprocal(const quatd& input) RTM_NO_EXCEPT
	{
		// TODO: Use recip instruction
		return 1.0 / quat_length(input);
	}

	inline quatd quat_normalize(const quatd& input) RTM_NO_EXCEPT
	{
		// TODO: Use high precision recip sqrt function and vector_mul
		double length = quat_length(input);
		//float length_recip = quat_length_reciprocal(input);
		vector4d input_vector = quat_to_vector(input);
		//return vector_to_quat(vector_mul(input_vector, length_recip));
		return vector_to_quat(vector_div(input_vector, vector_set(length)));
	}

	inline quatd quat_lerp(const quatd& start, const quatd& end, double alpha) RTM_NO_EXCEPT
	{
		// To ensure we take the shortest path, we apply a bias if the dot product is negative
		vector4d start_vector = quat_to_vector(start);
		vector4d end_vector = quat_to_vector(end);
		double dot = vector_dot(start_vector, end_vector);
		double bias = dot >= 0.0 ? 1.0 : -1.0;
		// TODO: Test with this instead: Rotation = (B * Alpha) + (A * (Bias * (1.f - Alpha)));
		vector4d value = vector_add(start_vector, vector_mul(vector_sub(vector_mul(end_vector, bias), start_vector), alpha));
		//vector4d value = vector_add(vector_mul(end_vector, alpha), vector_mul(start_vector, bias * (1.0 - alpha)));
		return quat_normalize(vector_to_quat(value));
	}

	inline quatd quat_neg(const quatd& input) RTM_NO_EXCEPT
	{
		return vector_to_quat(vector_mul(quat_to_vector(input), -1.0));
	}

	inline quatd quat_ensure_positive_w(const quatd& input) RTM_NO_EXCEPT
	{
		return quat_get_w(input) >= 0.0 ? input : quat_neg(input);
	}

	inline quatd quat_from_positive_w(const vector4d& input) RTM_NO_EXCEPT
	{
		// Operation order is important here, due to rounding, ((1.0 - (X*X)) - Y*Y) - Z*Z is more accurate than 1.0 - dot3(xyz, xyz)
		double w_squared = ((1.0 - vector_get_x(input) * vector_get_x(input)) - vector_get_y(input) * vector_get_y(input)) - vector_get_z(input) * vector_get_z(input);
		// w_squared can be negative either due to rounding or due to quantization imprecision, we take the absolute value
		// to ensure the resulting quaternion is always normalized with a positive W component
		double w = scalar_sqrt(scalar_abs(w_squared));
		return quat_set(vector_get_x(input), vector_get_y(input), vector_get_z(input), w);
	}

	//////////////////////////////////////////////////////////////////////////
	// Conversion to/from axis/angle/euler

	inline void quat_to_axis_angle(const quatd& input, vector4d& out_axis, double& out_angle) RTM_NO_EXCEPT
	{
		constexpr double epsilon = 1.0e-8;
		constexpr double epsilon_squared = epsilon * epsilon;

		out_angle = scalar_acos(quat_get_w(input)) * 2.0;

		double scale_sq = scalar_max(1.0 - quat_get_w(input) * quat_get_w(input), 0.0);
		out_axis = scale_sq >= epsilon_squared ? vector_div(vector_set(quat_get_x(input), quat_get_y(input), quat_get_z(input)), vector_set(scalar_sqrt(scale_sq))) : vector_set(1.0, 0.0, 0.0);
	}

	inline vector4d quat_get_axis(const quatd& input) RTM_NO_EXCEPT
	{
		constexpr double epsilon = 1.0e-8;
		constexpr double epsilon_squared = epsilon * epsilon;

		double scale_sq = scalar_max(1.0 - quat_get_w(input) * quat_get_w(input), 0.0);
		return scale_sq >= epsilon_squared ? vector_div(vector_set(quat_get_x(input), quat_get_y(input), quat_get_z(input)), vector_set(scalar_sqrt(scale_sq))) : vector_set(1.0, 0.0, 0.0);
	}

	inline double quat_get_angle(const quatd& input) RTM_NO_EXCEPT
	{
		return scalar_acos(quat_get_w(input)) * 2.0;
	}

	inline quatd quat_from_axis_angle(const vector4d& axis, double angle) RTM_NO_EXCEPT
	{
		double s, c;
		scalar_sincos(0.5 * angle, s, c);

		return quat_set(s * vector_get_x(axis), s * vector_get_y(axis), s * vector_get_z(axis), c);
	}

	// Pitch is around the Y axis (right)
	// Yaw is around the Z axis (up)
	// Roll is around the X axis (forward)
	inline quatd quat_from_euler(double pitch, double yaw, double roll) RTM_NO_EXCEPT
	{
		double sp, sy, sr;
		double cp, cy, cr;

		scalar_sincos(pitch * 0.5, sp, cp);
		scalar_sincos(yaw * 0.5, sy, cy);
		scalar_sincos(roll * 0.5, sr, cr);

		return quat_set(cr * sp * sy - sr * cp * cy,
			-cr * sp * cy - sr * cp * sy,
			cr * cp * sy - sr * sp * cy,
			cr * cp * cy + sr * sp * sy);
	}

	//////////////////////////////////////////////////////////////////////////
	// Comparisons and masking

	inline bool quat_is_finite(const quatd& input) RTM_NO_EXCEPT
	{
		return scalar_is_finite(quat_get_x(input)) && scalar_is_finite(quat_get_y(input)) && scalar_is_finite(quat_get_z(input)) && scalar_is_finite(quat_get_w(input));
	}

	inline bool quat_is_normalized(const quatd& input, double threshold = 0.00001) RTM_NO_EXCEPT
	{
		double length_squared = quat_length_squared(input);
		return scalar_abs(length_squared - 1.0) < threshold;
	}

	inline bool quat_near_equal(const quatd& lhs, const quatd& rhs, double threshold = 0.00001) RTM_NO_EXCEPT
	{
		return vector_all_near_equal(quat_to_vector(lhs), quat_to_vector(rhs), threshold);
	}

	inline bool quat_near_identity(const quatd& input, double threshold_angle = 0.00284714461) RTM_NO_EXCEPT
	{
		// See the quatf version of quat_near_identity for details.
		const double positive_w_angle = scalar_acos(scalar_abs(quat_get_w(input))) * 2.0;
		return positive_w_angle < threshold_angle;
	}
}
