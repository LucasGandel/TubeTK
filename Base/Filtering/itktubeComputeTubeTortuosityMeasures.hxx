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
#ifndef __itktubeComputeTubeTortuosityMeasures_hxx
#define __itktubeComputeTubeTortuosityMeasures_hxx

// TubeTK includes
#include "itktubeComputeTubeTortuosityMeasures.h"

namespace itk
{

namespace tube
{

/**
 * Constructor
 */
template< unsigned int VDimension >
ComputeTubeTortuosityMeasures< VDimension >
::ComputeTubeTortuosityMeasures()
{
  m_Input = NULL;
  m_Output = vtkSmartPointer<vtkTable>::New();

  //Metrics
  m_BasicMetrics = false;
  m_OldMetrics = false;
  m_CurvatureMetrics = false;
  m_HistogramMetrics = false;
  m_MetricFlag = 0;

  //Advanced parameters
  m_SmoothingMethod = ::tube::SMOOTH_TUBE_USING_INDEX_GAUSSIAN;
  m_SmoothingScale = 5.0;
  m_NumberOfHistogramBins = 20;
  m_HistogramMin = 0.0;
  m_HistogramMax = 1.0;


  //Initialize map
  m_MetricFlagToNameMap
    [TortuosityFilterType::AVERAGE_RADIUS_METRIC] = "AverageRadiusMetric";
  m_MetricFlagToNameMap
    [TortuosityFilterType::CHORD_LENGTH_METRIC] = "ChordLengthMetric";
  m_MetricFlagToNameMap
    [TortuosityFilterType::DISTANCE_METRIC] = "DistanceMetric";
  m_MetricFlagToNameMap
    [TortuosityFilterType::INFLECTION_COUNT_METRIC] = "InflectionCountMetric";
  m_MetricFlagToNameMap
    [TortuosityFilterType::INFLECTION_COUNT_1_METRIC]
      = "InflectionCount1Metric";
  m_MetricFlagToNameMap
    [TortuosityFilterType::INFLECTION_COUNT_2_METRIC]
      = "InflectionCount2Metric";
  m_MetricFlagToNameMap
    [TortuosityFilterType::PATH_LENGTH_METRIC] = "PathLengthMetric";
  m_MetricFlagToNameMap
    [TortuosityFilterType::PERCENTILE_95_METRIC] = "Percentile95Metric";
  m_MetricFlagToNameMap
    [TortuosityFilterType::SUM_OF_ANGLES_METRIC] = "SumOfAnglesMetric";
  m_MetricFlagToNameMap
    [TortuosityFilterType::TOTAL_CURVATURE_METRIC] = "TotalCurvatureMetric";
  m_MetricFlagToNameMap
    [TortuosityFilterType::TOTAL_SQUARED_CURVATURE_METRIC]
      = "TotalSquaredCurvatureMetric";
}

template< unsigned int VDimension >
void
ComputeTubeTortuosityMeasures< VDimension >
::Update( void )
{
  //Set metric flag
  if( m_BasicMetrics )
    {
    m_MetricFlag = m_MetricFlag
                 | TortuosityFilterType::AVERAGE_RADIUS_METRIC
                 | TortuosityFilterType::CHORD_LENGTH_METRIC
                 | TortuosityFilterType::PATH_LENGTH_METRIC;
    }

  if( m_OldMetrics )
    {
    m_MetricFlag = m_MetricFlag
                 | TortuosityFilterType::DISTANCE_METRIC
                 | TortuosityFilterType::INFLECTION_COUNT_METRIC
                 | TortuosityFilterType::INFLECTION_POINTS_METRIC
                 | TortuosityFilterType::SUM_OF_ANGLES_METRIC;
    }

  if( m_CurvatureMetrics )
    {
    m_MetricFlag = m_MetricFlag
                 | TortuosityFilterType::INFLECTION_COUNT_1_METRIC
                 | TortuosityFilterType::INFLECTION_COUNT_2_METRIC
                 | TortuosityFilterType::PERCENTILE_95_METRIC
                 | TortuosityFilterType::TOTAL_CURVATURE_METRIC
                 | TortuosityFilterType::TOTAL_SQUARED_CURVATURE_METRIC
                 | TortuosityFilterType::CURVATURE_SCALAR_METRIC
                 | TortuosityFilterType::CURVATURE_VECTOR_METRIC;
    }

  if( m_HistogramMetrics )
    {
    m_MetricFlag |= TortuosityFilterType::CURVATURE_HISTOGRAM_METRICS;
    }

  //Compute tortuosity measures
  char childName[] = "Tube";
  TubeListPointerType tubeList = m_Input->GetChildren(
    m_Input->GetMaximumDepth(), childName );

  m_TubeIdArray = vtkSmartPointer<vtkIntArray>::New();
  m_TubeIdArray->Initialize();
  m_TubeIdArray->SetName( "TubeIDs" );
  m_TubeIdArray->SetNumberOfValues( tubeList->size() );

  m_NumPointsArray = vtkSmartPointer<vtkIntArray>::New();
  m_NumPointsArray->Initialize();
  m_NumPointsArray->SetName( "NumberOfPoints" );
  m_NumPointsArray->SetNumberOfValues( tubeList->size() );

  for( int compareFlag = 0x01; compareFlag <=
    static_cast< int >( TortuosityFilterType::BITMASK_ALL_METRICS );
    compareFlag = compareFlag << 1)
    {
    // If metric is asked to print and is printable
    if( (m_MetricFlag & compareFlag &
        TortuosityFilterType::BITMASK_VESSEL_WISE_METRICS) > 0 )
      {
      vtkSmartPointer< vtkDoubleArray > metricArray =
        vtkSmartPointer< vtkDoubleArray >::New();
      metricArray->Initialize();
      metricArray->SetName( m_MetricFlagToNameMap[compareFlag].c_str() );
      metricArray->SetNumberOfValues( tubeList->size() );
      m_MetricArrayVec.push_back( metricArray );
      }
    }

  if( m_CurvatureMetrics )
    {
    vtkSmartPointer< vtkDoubleArray > tau4Array =
      vtkSmartPointer< vtkDoubleArray >::New();
    tau4Array->Initialize();
    tau4Array->SetName( "Tau4Metric" );
    tau4Array->SetNumberOfValues( tubeList->size() );
    m_MetricArrayVec.push_back( tau4Array );
    }

  if( (m_MetricFlag & TortuosityFilterType::CURVATURE_HISTOGRAM_METRICS) > 0 )
    {
    double histStep = (m_HistogramMax - m_HistogramMin)
      / m_NumberOfHistogramBins;

    for(int i = 0; i < m_NumberOfHistogramBins; i++)
      {
      std::ostringstream oss;
      oss << "Hist-Bin#" << i << ": " << i*histStep <<" - "
        << (i+1) * histStep;
      std::string binArrayName = oss.str();

      vtkSmartPointer< vtkIntArray > histArray =
        vtkSmartPointer< vtkIntArray >::New();
      histArray->Initialize();
      histArray->SetName( binArrayName.c_str() );
      histArray->SetNumberOfValues( tubeList->size() );
      m_HistogramArrays.push_back(histArray);
      }
    }

  int tubeIndex = 0;
  for( typename TubeGroupType::ChildrenListType::iterator
    itTubes = tubeList->begin(); itTubes != tubeList->end(); ++itTubes )
    {
    TubeType* curTube = dynamic_cast<TubeType*>((*itTubes).GetPointer());
    if (!curTube)
      {
      continue;
      }

    typename TortuosityFilterType::Pointer tortuosityFilter =
      TortuosityFilterType::New();
    tortuosityFilter->SetMeasureFlag( m_MetricFlag );
    tortuosityFilter->SetSmoothingScale( m_SmoothingScale );
    tortuosityFilter->SetSmoothingMethod( m_SmoothingMethod );
    tortuosityFilter->SetNumberOfBins( m_NumberOfHistogramBins );
    tortuosityFilter->SetHistogramMin( m_HistogramMin );
    tortuosityFilter->SetHistogramMax( m_HistogramMax );
    tortuosityFilter->SetInput( curTube );
    tortuosityFilter->Update();

    m_TubeIdArray->SetValue( tubeIndex, tubeIndex );
    m_NumPointsArray->SetValue( tubeIndex, curTube->GetNumberOfPoints() );

    std::cout << "vess = " << curTube->GetId() << std::endl;

    for( unsigned int i = 0; i < m_MetricArrayVec.size(); i++ )
      {
      std::string metricName = m_MetricArrayVec[i]->GetName();

      if( metricName == "AverageRadiusMetric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetAverageRadiusMetric() );
        }
      if( metricName == "ChordLengthMetric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetChordLengthMetric() );
        }
      if( metricName == "DistanceMetric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetDistanceMetric() );
        }
      if( metricName == "InflectionCountMetric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetInflectionCountMetric() );
        }
      if( metricName == "InflectionCount1Metric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetInflectionCount1Metric() );
        }
      if( metricName == "InflectionCount2Metric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetInflectionCount2Metric() );
        }
      if( metricName == "PathLengthMetric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetPathLengthMetric() );
        }
      if( metricName == "Percentile95Metric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetPercentile95Metric() );
        }
      if( metricName == "SumOfAnglesMetric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetSumOfAnglesMetric() );
        }
      if( metricName == "Tau4Metric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetTotalCurvatureMetric()
            / tortuosityFilter->GetPathLengthMetric() );
        }
      if( metricName == "TotalCurvatureMetric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetTotalCurvatureMetric() );
        }
      if( metricName == "TotalSquaredCurvatureMetric" )
        {
        m_MetricArrayVec[i]->SetValue( tubeIndex,
          tortuosityFilter->GetTotalSquaredCurvatureMetric() );
        }
      }

    if( ( m_MetricFlag & TortuosityFilterType::CURVATURE_HISTOGRAM_METRICS )
      > 0 )
      {
      // Get the histogram features
      for( int i = 0; i < m_NumberOfHistogramBins; i++ )
        {
        m_HistogramArrays[i]->SetValue( tubeIndex,
          tortuosityFilter->GetCurvatureHistogramMetric(i) );
        }
      }
    tubeIndex++;
    }

  // Write tortuosity measures to a CSV file
  m_Output->AddColumn( m_TubeIdArray );
  m_Output->AddColumn( m_NumPointsArray );

  for( unsigned int i = 0; i < m_MetricArrayVec.size(); i++ )
    {
    m_Output->AddColumn( m_MetricArrayVec[i] );
    }

  if( ( m_MetricFlag & TortuosityFilterType::CURVATURE_HISTOGRAM_METRICS )
    > 0 )
    {
    // Get the histogram features
    for( int i = 0; i < m_NumberOfHistogramBins; i++ )
      {
      m_Output->AddColumn( m_HistogramArrays[i] );
      }
    }

}

template< unsigned int VDimension >
void
ComputeTubeTortuosityMeasures< VDimension >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "Input: " << m_Input << std::endl;
  os << "Write basic metrics: " << m_BasicMetrics << std::endl;
  os << "Write old metrics: " << m_OldMetrics << std::endl;
  os << "Write curvature metrics: " << m_CurvatureMetrics << std::endl;
  os << "Write histogram metrics: " << m_HistogramMetrics << std::endl;
  os << "Smoothing method: " << m_SmoothingMethod << std::endl;
  os << "Smoothing scale: " << m_SmoothingScale << std::endl;
  os << "Number of histogram bins: " << m_NumberOfHistogramBins << std::endl;
  os << "Histogram Min: " << m_HistogramMin << std::endl;
  os << "Histogram Max: " << m_HistogramMax << std::endl;

}

} // End namespace tube

} // End namespace itk

#endif
