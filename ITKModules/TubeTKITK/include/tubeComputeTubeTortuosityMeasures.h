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
#ifndef __tubeComputeTubeTortuosityMeasures_h
#define __tubeComputeTubeTortuosityMeasures_h

// ITK includes
#include <itkObject.h>

// TubeTK includes
#include "itktubeComputeTubeTortuosityMeasures.h"
#include "tubeWrappingMacros.h"

namespace tube
{
/** \class ComputeTubeTortuosityMeasures
 *  \brief Computes various tortuosity measures from an input tube group
 * and outputs it as a vtkTable
 *
 *  \ingroup TubeTKITK
 */

template< unsigned int VDimension >
class ComputeTubeTortuosityMeasures:
  public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef ComputeTubeTortuosityMeasures              Self;
  typedef itk::Object                                SuperClass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer< const Self >            ConstPointer;

  typedef itk::tube::ComputeTubeTortuosityMeasures<
    VDimension >                                     FilterType;

  typedef itk::GroupSpatialObject< VDimension >      TubeGroupType;
  typedef typename TubeGroupType::Pointer            TubeGroupPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ComputeTubeTortuosityMeasures, Object);

  /** Set/Get writing of different metrics */
  tubeWrapSetMacro( BasicMetrics, bool, Filter );
  tubeWrapGetMacro( BasicMetrics, bool, Filter );
  tubeWrapSetMacro( OldMetrics, bool, Filter );
  tubeWrapGetMacro( OldMetrics, bool, Filter );
  tubeWrapSetMacro( CurvatureMetrics, bool, Filter );
  tubeWrapGetMacro( CurvatureMetrics, bool, Filter );
  tubeWrapSetMacro( HistogramMetrics, bool, Filter );
  tubeWrapGetMacro( HistogramMetrics, bool, Filter );

  /** Set/Get advanced parameters */
  tubeWrapSetMacro( SmoothingMethod, ::tube::SmoothTubeFunctionEnum, Filter );
  tubeWrapGetMacro( SmoothingMethod, ::tube::SmoothTubeFunctionEnum, Filter );
  tubeWrapSetMacro( SmoothingScale, double, Filter );
  tubeWrapGetMacro( SmoothingScale, double, Filter );
  tubeWrapSetMacro( NumberOfHistogramBins, int, Filter );
  tubeWrapGetMacro( NumberOfHistogramBins, int, Filter );
  tubeWrapSetMacro( HistogramMin, double, Filter );
  tubeWrapGetMacro( HistogramMin, double, Filter );
  tubeWrapSetMacro( HistogramMax, double, Filter );
  tubeWrapGetMacro( HistogramMax, double, Filter );

  /** Set/Get input tubes */
  tubeWrapSetObjectMacro( Input, TubeGroupType, Filter );
  tubeWrapGetConstObjectMacro( Input, TubeGroupType, Filter );

  /** Compute image similarity */
  tubeWrapUpdateMacro(Filter);

  /** Get image similarity */
  tubeWrapGetObjectMacro(Output, vtkTable, Filter);

protected:
  ComputeTubeTortuosityMeasures( void );
  ~ComputeTubeTortuosityMeasures() {}
  void PrintSelf( std::ostream & os, itk::Indent indent ) const;

private:
  /** itkComputeTubeTortuosityMeasuresFilter parameters **/
  ComputeTubeTortuosityMeasures(const Self &);
  void operator=(const Self &);

  typename FilterType::Pointer m_Filter;

};

} // End namespace tube


#ifndef ITK_MANUAL_INSTANTIATION
#include "tubeComputeTubeTortuosityMeasures.hxx"
#endif

#endif // End !defined( __tubeComputeTubeTortuosityMeasures_h )
