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

// TubeTK INCLUDES
#include "tubeMacro.h"
#include "tubeMessage.h"
#include "tubeCLIProgressReporter.h"
#include "tubeTubeMath.h"

//TubeTKITK includes
#include "tubeComputeTubeTortuosityMeasures.h"

// ITK INCLUDES
#include "itkTimeProbesCollectorBase.h"
#include "itkSpatialObjectReader.h"
#include "itkSpatialObjectWriter.h"
#include "metaScene.h"

// VTK INCLUDES
#include "vtkNew.h"
#include "vtkVersion.h"
#include "vtkDelimitedTextWriter.h"

#include "ComputeTubeTortuosityMeasuresCLP.h"

template< unsigned int VDimension >
int DoIt( int argc, char * argv[] )
{
  PARSE_ARGS;

  // Ensure that the input image dimension is valid
  // We only support 3D Images due to the
  // limitation of itkTubeSpatialObject
  if( VDimension != 3 )
    {
    tube::ErrorMessage("Error: Only 3D data is currently supported.");
    return EXIT_FAILURE;
    }

  // The timeCollector to perform basic profiling of algorithmic components
  itk::TimeProbesCollectorBase timeCollector;

  // typedefs
  typedef itk::SpatialObjectReader< VDimension >      TubesReaderType;

  typedef ::tube::ComputeTubeTortuosityMeasures< VDimension >
    TortuosityFilterType;
  typename TortuosityFilterType::Pointer tortuosityFilter =
      TortuosityFilterType::New();

  // Load TRE File
  tubeStandardOutputMacro( << "\n>> Loading TRE File" );

  timeCollector.Start( "Loading Input TRE File" );

  typename TubesReaderType::Pointer tubeFileReader = TubesReaderType::New();

  try
    {
    tubeFileReader->SetFileName( inputTREFile.c_str() );
    tubeFileReader->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    tube::ErrorMessage( "Error loading TRE File: "
                        + std::string( err.GetDescription() ) );
    timeCollector.Report();
    return EXIT_FAILURE;
    }

  tortuosityFilter->SetInput( tubeFileReader->GetGroup() );

  timeCollector.Stop( "Loading Input TRE File" );

  // Get specified smoothing method
  if(smoothingMethod == "SMOOTH_TUBE_USING_INDEX_AVERAGE" )
    {
    tortuosityFilter->SetSmoothingMethod(
      tube::SMOOTH_TUBE_USING_INDEX_AVERAGE );
    }
  else if(smoothingMethod == "SMOOTH_TUBE_USING_DISTANCE_GAUSSIAN")
    {
    tortuosityFilter->SetSmoothingMethod(
      tube::SMOOTH_TUBE_USING_DISTANCE_GAUSSIAN );
    }
  else
    {
    tortuosityFilter->SetSmoothingMethod(
      tube::SMOOTH_TUBE_USING_INDEX_GAUSSIAN );
    }

  //Set advanced parameters
  tortuosityFilter->SetSmoothingScale( smoothingScale );
  tortuosityFilter->SetNumberOfHistogramBins( numberOfHistogramBins );
  tortuosityFilter->SetHistogramMin( histogramMin );
  tortuosityFilter->SetHistogramMax( histogramMax );

  // Prepare tortuosity measure flag
  tortuosityFilter->SetBasicMetrics( basicMetrics );
  tortuosityFilter->SetOldMetrics( oldMetrics );
  tortuosityFilter->SetCurvatureMetrics( curvatureMetrics );
  tortuosityFilter->SetHistogramMetrics( histogramMetrics );


  // Run tortuosity filter
  tubeStandardOutputMacro( << "\n>> Computing tortuosity measures" );

  timeCollector.Start( "Computing tortuosity measures" );

  tortuosityFilter->Update();

  timeCollector.Stop( "Computing tortuosity measures" );

  // Write tortuosity measures to a CSV file
  tubeStandardOutputMacro( << "\n>> Writing tortuosity measures to CSV" );

  timeCollector.Start( "Writing tortuosity measures to CSV" );

  vtkNew< vtkDelimitedTextWriter > writer;
  writer->SetFileName( outputCSVFile.c_str() );
#if VTK_MAJOR_VERSION <= 5
  writer->SetInput( tortuosityFilter->GetOutput() );
#else
  writer->SetInputData( tortuosityFilter->GetOutput()  );
#endif
  writer->Write();

  timeCollector.Stop( "Writing tortuosity measures to CSV" );

  // All done
  timeCollector.Report();
  return EXIT_SUCCESS;
}

// Main
int main( int argc, char * argv[] )
{
  try
    {
    PARSE_ARGS;
    }
  catch( const std::exception & err )
    {
    tube::ErrorMessage( err.what() );
    return EXIT_FAILURE;
    }
  PARSE_ARGS;

  MetaScene *mScene = new MetaScene;
  mScene->Read( inputTREFile.c_str() );
  if( mScene->GetObjectList()->empty() )
    {
    tubeWarningMacro( << "Input TRE file has no spatial objects" );
    return EXIT_SUCCESS;
    }

  switch( mScene->GetObjectList()->front()->NDims() )
    {
    case 3:
      {
      bool result = DoIt<3>( argc, argv );
      delete mScene;
      return result;
      break;
      }
    default:
      {
      tubeErrorMacro(<< "Error: Only 3D data is currently supported.");
      delete mScene;
      return EXIT_FAILURE;
      }
    }

  delete mScene;
  return EXIT_FAILURE;
}
