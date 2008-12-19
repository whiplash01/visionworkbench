// __BEGIN_LICENSE__
// 
// Copyright (C) 2006 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration
// (NASA).  All Rights Reserved.
// 
// Copyright 2006 Carnegie Mellon University. All rights reserved.
// 
// This software is distributed under the NASA Open Source Agreement
// (NOSA), version 1.3.  The NOSA has been approved by the Open Source
// Initiative.  See the file COPYING at the top of the distribution
// directory tree for the complete NOSA document.
// 
// THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY
// KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT
// LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO
// SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
// A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT
// THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT
// DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE.
// 
// __END_LICENSE__

// IntegralImage.h
//
// Provides support for algorithms that evaluate with Integral Images
// such as SURF
#ifndef __VW_INTERESTPOINT_INTEGRALIMAGE_H__
#define __VW_INTERESTPOINT_INTEGRALIMAGE_H__

#include <vw/Image/ImageView.h>
#include <vw/Math.h>

namespace vw {
namespace ip {

  /// Creates Integral Image
  template <class ViewT>
  inline ImageView<double> IntegralImage( ImageViewBase<ViewT> const& source ) {
    
    // Allocating space for integral image
    vw::ImageView<double> integral( source.impl().cols()+1, source.impl().rows()+1 );
    
    // Performing cumulative sum in the x direction
    for( signed y = 1; y < integral.rows(); ++y ) {
      integral(0,y) = 0;  // This is only need if the integral is not zero in the beginning
      for( signed x = 1; x < integral.cols(); ++x ) {
	integral(x,y) = integral(x-1,y) + pixel_cast<PixelGray<double> >(source.impl()(x-1,y-1)).v();
      }
    }

    // Performing cumulative sum in the y direction
    for( int x = 1; x < integral.cols(); ++x ) {
      integral(x,0) = 0;
      for ( int y = 1; y < integral.rows(); ++y)
	integral(x,y) += integral(x,y-1);
    }

    return integral;
  }

  /// Integral Block Evaluation
  ///
  /// This is for summing an area of pixels described by integral
  inline double IntegralBlock( ImageView<double> const& integral, 
			       Vector2i const& top_left,
			       Vector2i const& bottom_right ) {
    VW_DEBUG_ASSERT(top_left.x() > bottom_right.x() && top_left.y() > bottom_right.y(), 
		    vw::ArgumentErr() << "Incorrect input for IntegralBlock.\n");
    VW_DEBUG_ASSERT(top_left.x() < (unsigned)integral.cols(), 
		    vw::ArgumentErr() << "x0 out of bounds. "<< integral.cols() <<" : "
		    << top_left << bottom_right << "\n");
    VW_DEBUG_ASSERT(bottom_right.x() < (unsigned)integral.cols(), 
		    vw::ArgumentErr() << "x1 out of bounds. "<< integral.cols() <<" : "
		    << top_left << bottom_right << "\n");
    VW_DEBUG_ASSERT(top_left.y() < (unsigned)integral.rows(), 
		    vw::ArgumentErr() << "y0 out of bounds. "<< integral.rows() <<" : "
		    << top_left << bottom_right << "\n");
    VW_DEBUG_ASSERT(bottom_right.y() < (unsigned)integral.rows(), 
		    vw::ArgumentErr() << "y1 out of bounds. "<< integral.rows() <<" : "
		    << top_left << bottom_right << "\n");
    
    double result;
    result = integral( top_left.x(), top_left.y() );
    result += integral( bottom_right.x(), bottom_right.y() );
    result -= integral( top_left.x(), bottom_right.y() );
    result -= integral( bottom_right.x(), top_left.y() );

    result /= (top_left.x() - bottom_right.x())*(top_left.y() - bottom_right.y());

    return result;
  }

  /// X First Derivative
  /// - x,y         = location to center the calculation on
  /// - filter_size = size of window for calculation
  inline float XFirstDerivative( ImageView<double> const& integral,
				 int const& x, int const& y,
				 unsigned const& filter_size ) {
    float derivative = 0;
    return derivative;
  }

  /// X Second Derivative
  /// - x,y         = location to center the calculation on
  /// - filter_size = size of window for calculation
  inline float XSecondDerivative( ImageView<double> const& integral,
				  int const& x, int const& y,
				  unsigned const& filter_size ) {
    unsigned lobe = filter_size / 3;
    unsigned half_lobe = floor( float(lobe) / 2.0 );
    float derivative;

    // Adding positive left;
    derivative = IntegralBlock( integral,
				Vector2i( x - lobe - half_lobe, y - lobe + 1 ),
				Vector2i( x - half_lobe, y + lobe ) );

    // Adding negative middle;
    derivative -= 2.0*IntegralBlock( integral,
				     Vector2i( x - half_lobe, y - lobe + 1 ),
				     Vector2i( x + half_lobe + 1, y + lobe ) );

    // Adding positive right;
    derivative += IntegralBlock( integral,
				 Vector2i( x + half_lobe + 1, y - lobe + 1 ),
				 Vector2i( x + half_lobe + lobe + 1, y + lobe ) );
    
    return derivative;
  }

  /// Y First Derivative
  /// - x,y         = location to center the calculation on
  /// - filter_size = size of window for calculation
  inline float YFirstDerivative( ImageView<double> const& integral,
				 int const& x, int const& y,
				 unsigned const& filter_size ) {
    float derivative = 0;

    return derivative;
  }

  /// Y Second Derivative
  /// - x,y         = location to center the calculation on
  /// - filter_size = size of window for calculation
  inline float YSecondDerivative( ImageView<double> const& integral,
				  int const& x, int const& y,
				  unsigned const& filter_size ) {
    unsigned lobe = filter_size / 3;
    unsigned half_lobe = floor( float(lobe) / 2.0 );
    float derivative;

    // Adding positive top;
    derivative = IntegralBlock( integral,
				Vector2i( x - lobe + 1, y - lobe - half_lobe ),
				Vector2i( x + lobe, y - half_lobe ) );

    // Adding negative middle;
    derivative -= 2.0*IntegralBlock( integral,
				     Vector2i( x - lobe + 1, y - half_lobe ),
				     Vector2i( x + lobe, y + half_lobe + 1 ) );

    // Adding positive bottom;
    derivative += IntegralBlock( integral,
				 Vector2i( x - lobe + 1, y + half_lobe + 1 ),
				 Vector2i( x + lobe, y + half_lobe + lobe + 1 ) );

    return derivative;
  }

  /// XY Derivative
  /// - x,y         = location to center the calculation on
  /// - filter_size = size of window for calculation
  inline float XYDerivative( ImageView<double> const& integral,
			     int const& x, int const& y,
			     unsigned const& filter_size ) {

    unsigned lobe = filter_size / 3;
    float derivative;

    // Adding positive top left
    derivative = IntegralBlock( integral,
				Vector2i( x - lobe, y - lobe ),
				Vector2i( x, y ) );
    
    // Adding negative top right
    derivative -= IntegralBlock( integral,
				 Vector2i( x + 1, y - lobe ),
				 Vector2i( x + lobe + 1, y ) );

    // Adding negative bottom left
    derivative -= IntegralBlock( integral,
				 Vector2i( x - lobe, y + 1 ),
				 Vector2i( x, y + lobe + 1 ) );

    // Adding positve bottom right
    derivative += IntegralBlock( integral,
				 Vector2i( x + 1, y + 1 ),
				 Vector2i( x + 1 + lobe, y + 1 + lobe ) );

    return derivative;
  }

  // Horizontal Wavelet
  // - integral  = Integral used for calculations
  // - ix        = x location to evaluate at
  // - iy        = y location to evaluate at
  // - size      = side of the square used for evaluate
  inline float HHaarWavelet( vw::ImageView<double> const& integral, signed const& ix,
			signed const& iy, unsigned const& size ) {
    // This should only take 6 operations (nm the multiplication)
    float response = 0;
    unsigned half_size = size>>1;
    response = -integral(ix-half_size,iy-half_size);
    response += 2*integral(ix,iy-half_size);
    response -= integral(ix+half_size,iy-half_size);
    response += integral(ix-half_size,iy+half_size);
    response -= 2*integral(ix,iy+half_size);
    response += integral(ix+half_size,iy+half_size);

    //response /= size*size;

    return response;
  }

  // Vertical Wavelet
  // - integral  = Integral used for calculations
  // - ix        = x location to evaluate at
  // - iy        = y location to evaluate at
  // - size      = side of the square used for evaluate
  inline float VHaarWavelet( vw::ImageView<double> const& integral, signed const& ix,
		      signed const& iy, unsigned const& size ) {
    // This should only take 6 operations (nm the multiplication)
    float response = 0;
    unsigned half_size = size>>1;
    response = -integral(ix-half_size,iy-half_size);
    response += integral(ix+half_size,iy-half_size);
    response += 2*integral(ix-half_size,iy);
    response -= 2*integral(ix+half_size,iy);
    response -= integral(ix-half_size,iy+half_size);
    response += integral(ix+half_size,iy+half_size);

    //response /= size*size;

    return response;
  }

}} // end namespace vw::ip

#endif // __VW_INTERESTPOINT_INTEGRALIMAGE_H__