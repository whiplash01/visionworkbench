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

/// \file Camera.h
/// 
/// A convenience header that includes the header files in vw/Camera.
/// 
#ifndef __VW_CAMERA_H__
#define __VW_CAMERA_H__

#include <vw/config.h>

#include <vw/Camera/CameraModel.h>
#include <vw/Camera/CAHVModel.h>
#include <vw/Camera/CAHVORModel.h>
#include <vw/Camera/LensDistortion.h>
#include <vw/Camera/PinholeModel.h>
#include <vw/Camera/PinholeModelCalibrate.h>
#include <vw/Camera/CameraTransform.h>
#include <vw/Camera/BayerFilter.h>
#include <vw/Camera/Exif.h>
#include <vw/Camera/ExifData.h>
#include <vw/Camera/BundleAdjust.h>
#include <vw/Camera/ControlNetwork.h>

#if defined(VW_HAVE_PKG_LAPACK) && VW_HAVE_PKG_LAPACK==1
#include <vw/Camera/Extrinsics.h>
#include <vw/Camera/LinescanModel.h>
#include <vw/Camera/LinearPushbroomModel.h>
#include <vw/Camera/OrbitingPushbroomModel.h>
#endif

#endif // __VW_CAMERA_H__
 
