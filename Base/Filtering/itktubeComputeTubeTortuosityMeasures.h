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

#ifndef __itktubeComputeTubeTortuosityMeasures_h
#define __itktubeComputeTubeTortuosityMeasures_h

//TubeTK includes
#include "itktubeTortuositySpatialObjectFilter.h"

// ITK includes
#include "itkGroupSpatialObject.h"
#include "itkVesselTubeSpatialObject.h"

// std includes
#include <sstream>
#include <string.h>
#include <vector>
#include <map>
#include <string>

namespace itk
{

namespace tube
{

/** \class ComputeTubeTortuosityMeasures
 * \brief Computes various tortuosity measures from an input tube group
 * and outputs it in a csv file thanks to the WriteCSVfile() method.
 * The GetOutput() method returns a table containing all the measures.
 */

template< unsigned int VDimension >
class ComputeTubeTortuosityMeasures
  : public Object
{
public:

  /** Standard class typedefs. */
  typedef ComputeTubeTortuosityMeasures   Self;
  typedef Object                          Superclass;
  typedef SmartPointer< Self >            Pointer;
  typedef SmartPointer< const Self >      ConstPointer;

  /** custom typedefs */
  typedef itk::VesselTubeSpatialObject< VDimension >  TubeType;
  typedef itk::GroupSpatialObject< VDimension >       TubeGroupType;
  typedef typename TubeGroupType::ChildrenListPointer TubeListPointerType;

  typedef itk::tube::TortuositySpatialObjectFilter< TubeType >
    TortuosityFilterType;


  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( ComputeTubeTortuosityMeasures, Object );

  /** Set/Get input tubes (TubeGroup) */
  itkSetObjectMacro( Input, TubeGroupType );
  itkGetObjectMacro( Input, TubeGroupType );

  /** Set/Get output table of measures */
  itkSetMacro( Output, std::vector< itk::Array<double> > );
  itkGetMacro( Output, std::vector< itk::Array<double> > );

  /** Set/Get output metrics name */
  itkSetMacro( MetricsLabels, std::vector< std::string > );
  itkGetMacro( MetricsLabels, std::vector< std::string > );


  /** Set/Get writing of different metrics */
  itkSetMacro( BasicMetrics, bool );
  itkGetMacro( BasicMetrics, bool );
  itkSetMacro( OldMetrics, bool );
  itkGetMacro( OldMetrics, bool );
  itkSetMacro( CurvatureMetrics, bool );
  itkGetMacro( CurvatureMetrics, bool );
  itkSetMacro( HistogramMetrics, bool );
  itkGetMacro( HistogramMetrics, bool );

  /** Set/Get advanced parameters */
  itkSetMacro( SmoothingMethod, ::tube::SmoothTubeFunctionEnum );
  itkGetMacro( SmoothingMethod, ::tube::SmoothTubeFunctionEnum );
  itkSetMacro( SmoothingScale, double );
  itkGetMacro( SmoothingScale, double );
  itkSetMacro( NumberOfHistogramBins, int );
  itkGetMacro( NumberOfHistogramBins, int );
  itkSetMacro( HistogramMin, double );
  itkGetMacro( HistogramMin, double );
  itkSetMacro( HistogramMax, double );
  itkGetMacro( HistogramMax, double );

  void Update(void);
  void WriteCSVfile( std::string csvStatisticsFile ) const;

protected:

  ComputeTubeTortuosityMeasures( void );
  ~ComputeTubeTortuosityMeasures( void ) {};

  void PrintSelf(std::ostream& os, Indent indent) const;

private:

  //Input tube group
  typename TubeGroupType::Pointer       m_Input;

  //Output table of measures
  std::vector< itk::Array<double> >     m_Output;
  std::vector< std::string >            m_MetricsLabels;

  //Metrics type to write
  bool                                  m_BasicMetrics;
  bool                                  m_OldMetrics;
  bool                                  m_CurvatureMetrics;
  bool                                  m_HistogramMetrics;
  int                                   m_MetricFlag;
  std::map<int, std::string>            m_MetricFlagToNameMap;

  //Metrics arrays
  itk::Array<double>                    m_TubeIdArray;
  itk::Array<double>                    m_NumPointsArray;
  std::vector< itk::Array<double>  >    m_MetricArrayVec;
  std::vector< itk::Array<double>  >    m_HistogramArrays;

  //Advanced parameters
  ::tube::SmoothTubeFunctionEnum        m_SmoothingMethod;
  double                                m_SmoothingScale;
  int                                   m_NumberOfHistogramBins;
  double                                m_HistogramMin;
  double                                m_HistogramMax;

}; // End class ComputeTubeTortuosityMeasures

} // End namespace tube

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubeComputeTubeTortuosityMeasures.hxx"
#endif

#endif // End !defined(__itktubeComputeTubeTortuosityMeasures_h)
