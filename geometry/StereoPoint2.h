/*
 * StereoPoint2.h
 *
 *  Created on: Jan 26, 2010
 *      Author: dellaert
 */

#ifndef STEREOPOINT2_H_
#define STEREOPOINT2_H_

#include <iostream>
#include <gtsam/base/Vector.h>
#include <gtsam/base/Testable.h>
#include <gtsam/base/Lie.h>
#include <gtsam/geometry/Point2.h>

namespace gtsam {

	/**
	 * A 2D stereo point, v will be same for rectified images
	 */
	class StereoPoint2: Testable<StereoPoint2>, Lie<StereoPoint2> {
	public:
		static const size_t dimension = 3;
	private:
		double uL_, uR_, v_;

	public:

		/** default constructor */
		StereoPoint2() :
			uL_(0), uR_(0), v_(0) {
		}

		/** constructor */
		StereoPoint2(double uL, double uR, double v) :
			uL_(uL), uR_(uR), v_(v) {
		}

		/** print */
		void print(const std::string& s) const {
			std::cout << s << "(" << uL_ << ", " << uR_ << ", " << v_ << ")"
					<< std::endl;
		}

		/** equals */
		bool equals(const StereoPoint2& q, double tol) const {
			return (fabs(uL_ - q.uL_) < tol && fabs(uR_ - q.uR_) < tol && fabs(v_
					- q.v_) < tol);
		}

	    /** dimension of the variable - used to autodetect sizes */
	    inline static size_t Dim() { return dimension; }

		/** Lie requirements */
		inline size_t dim() const { return dimension; }

		/** convert to vector */
		Vector vector() const {
			return Vector_(3, uL_, uR_, v_);
		}

		/** add two stereo points */
		StereoPoint2 operator+(const StereoPoint2& b) const {
			return StereoPoint2(uL_ + b.uL_, uR_ + b.uR_, v_ + b.v_);
		}

		/** subtract two stereo points */
		StereoPoint2 operator-(const StereoPoint2& b) const {
			return StereoPoint2(uL_ - b.uL_, uR_ - b.uR_, v_ - b.v_);
		}

		/*
		 * convenient function to get a Point2 from the left image
		 */
		inline Point2 point2(){
			return Point2(uL_, v_);
		}

		/** "Compose", just adds the coordinates of two points. */
		inline StereoPoint2 compose(const StereoPoint2& p1) const {
			return *this + p1;
		}

		/** inverse */
		inline StereoPoint2 inverse() const {
			return StereoPoint2()- (*this);
		}

		/** Exponential map around identity - just create a Point2 from a vector */
		static inline StereoPoint2 Expmap(const Vector& d) {
			return StereoPoint2(d(0), d(1), d(2));
		}

		/** Log map around identity - just return the Point2 as a vector */
		static inline Vector Logmap(const StereoPoint2& p) {
			return p.vector();
		}
	};

}

#endif /* STEREOPOINT2_H_ */
