/**
 * @file    Point2.h
 * @brief   2D Point
 * @author  Frank Dellaert
 */

#pragma once

#include <boost/serialization/nvp.hpp>
#include <gtsam/base/Vector.h>
#include <gtsam/base/Matrix.h>
#include <gtsam/base/Testable.h>
#include <gtsam/base/Lie.h>

namespace gtsam {

  /**
   * A 2D point
   * Derived from testable so has standard print and equals, and assert_equals works
   * Functional, so no set functions: once created, a point is constant.
   */
  class Point2: Testable<Point2>, public Lie<Point2> {
  public:
	  /// dimension of the variable - used to autodetect sizes
	  static const size_t dimension = 2;
  private:
    double x_, y_;
		
  public:
    Point2(): x_(0), y_(0) {}
    Point2(const Point2 &p) : x_(p.x_), y_(p.y_) {}
    Point2(double x, double y): x_(x), y_(y) {}
    Point2(const Vector& v) : x_(v(0)), y_(v(1)) {}

    /** dimension of the variable - used to autodetect sizes */
    inline static size_t Dim() { return dimension; }

    /** print with optional string */
    void print(const std::string& s = "") const;

    /** equals with an tolerance, prints out message if unequal*/
    bool equals(const Point2& q, double tol = 1e-9) const;

    /** Lie requirements */

    /** Size of the tangent space of the Lie type */
    inline size_t dim() const { return dimension; }

    /** "Compose", just adds the coordinates of two points. */
    Point2 compose(const Point2& p1) const { return *this+p1; }

    /** "Inverse" - negates each coordinate such that compose(p,inverse(p))=Point2() */
    Point2 inverse() const { return Point2(-x_, -y_); }

    /** Exponential map around identity - just create a Point2 from a vector */
    static inline Point2 Expmap(const Vector& v) { return Point2(v); }

    /** Log map around identity - just return the Point2 as a vector */
    static inline Vector Logmap(const Point2& dp) { return Vector_(2, dp.x(), dp.y()); }

    /** get functions for x, y */
    double x() const {return x_;}
    double y() const {return y_;}

    /** return vectorized form (column-wise) */
    Vector vector() const { return Vector_(2, x_, y_); }

    /** operators */
    inline Point2 operator- () const {return Point2(-x_,-y_);}
    inline bool   operator ==(const Point2& q) const {return x_==q.x_ && q.y_==q.y_;}
    inline Point2 operator + (const Point2& q) const {return Point2(x_+q.x_,y_+q.y_);}
    inline Point2 operator - (const Point2& q) const {return Point2(x_-q.x_,y_-q.y_);}
    inline Point2 operator * (double s) const {return Point2(x_*s,y_*s);}
    inline Point2 operator / (double q) const {return Point2(x_/q,y_/q);}

    /** norm of point */
    double norm() const;

    /** distance between two points */
    inline double dist(const Point2& p2) const {
			return (p2 - *this).norm();
		}

  private:
    /** Serialization function */
    friend class boost::serialization::access;
    template<class Archive>
      void serialize(Archive & ar, const unsigned int version)
    {
      ar & BOOST_SERIALIZATION_NVP(x_);
      ar & BOOST_SERIALIZATION_NVP(y_);
    }
  };

  /** Lie group functions */

  /** "Compose", just adds the coordinates of two points. */
  inline Point2 compose(const Point2& p1, const Point2& p2,
      boost::optional<Matrix&> H1, boost::optional<Matrix&> H2=boost::none) {
    if(H1) *H1 = eye(2);
    if(H2) *H2 = eye(2);
    return compose(p1, p2);
  }
  inline Matrix Dcompose1(const Point2& p1, const Point2& p0) {
    return Matrix_(2,2,
        1.0, 0.0,
        0.0, 1.0); }
  inline Matrix Dcompose2(const Point2& p1, const Point2& p0) {
    return Matrix_(2,2,
        1.0, 0.0,
        0.0, 1.0); }

  /** "Between", subtracts point coordinates */
  inline Point2 between(const Point2& p1, const Point2& p2) { return p2-p1; }
  inline Point2 between(const Point2& p1, const Point2& p2,
      boost::optional<Matrix&> H1, boost::optional<Matrix&> H2=boost::none) {
    if(H1) *H1 = -eye(2);
    if(H2) *H2 = eye(2);
    return between(p1, p2);
  }

  /** multiply with scalar */
  inline Point2 operator*(double s, const Point2& p) {return p*s;}
}

