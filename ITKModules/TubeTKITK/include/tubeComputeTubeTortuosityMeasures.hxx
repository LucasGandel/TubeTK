/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#ifndef __tubeComputeTubeTortuosityMeasures_hxx
#define __tubeComputeTubeTortuosityMeasures_hxx

#include "tubeComputeTubeTortuosityMeasures.h"

namespace tube
{

template< unsigned int VDimension >
ComputeTubeTortuosityMeasures< VDimension >
::ComputeTubeTortuosityMeasures( void )
{
  m_Filter = FilterType::New();
}

template< unsigned int VDimension >
void
ComputeTubeTortuosityMeasures< VDimension >
::WriteCSVfile( std::string csvFileName ) const
{
  m_Filter->WriteCSVfile( csvFileName );
}

template< unsigned int VDimension >
void
ComputeTubeTortuosityMeasures< VDimension >
::PrintSelf( std::ostream & os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
  os << "Write basic metrics: " << this->GetBasicMetrics() << std::endl;
  os << "Write old metrics: " << this->GetOldMetrics() << std::endl;
  os << "Write curvature metrics: " << this->GetCurvatureMetrics() << std::endl;
  os << "Write histogram metrics: " << this->GetHistogramMetrics() << std::endl;
  os << "Smoothing method: " << this->GetSmoothingMethod() << std::endl;
  os << "Smoothing scale: " << this->GetSmoothingScale() << std::endl;
  os << "Number of histogram bins: " << this->GetNumberOfHistogramBins() << std::endl;
  os << "Histogram Min: " << this->GetHistogramMin() << std::endl;
  os << "Histogram Max: " << this->GetHistogramMax() << std::endl;
}

} // End namespace tubetk

#endif
