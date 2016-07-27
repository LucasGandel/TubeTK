
import sys
import site
site.addsitedir('local/lib/python2.7/site-packages/')
## Append ITK libs
#sys.path.append('/home/lucas/Projects/ITK-Release/Wrapping/Generators/Python')
#sys.path.append('/home/lucas/Projects/ITK-Release/Modules/ThirdParty/VNL/src/vxl/lib')
## Append TubeTK libs :
#sys.path.append('/home/lucas/Projects/TubeTK-Release-ITK4.9/TubeTK-build/lib/TubeTK')
## VTK libs :
#sys.path.append('/home/lucas/Projects/Slicer/Slicer-superbuild/VTKv6-build/lib')
#sys.path.append('/home/lucas/Projects/Slicer/Slicer-superbuild/VTKv6-build/Wrapping/Python')

#import vtk

import itk
from itk import TubeTKITK as itktube

#Sanity checks
if len(sys.argv) != 3:
    print("Usage: " + sys.argv[0] + " <inputTubes> " + " <outputCSVFile> ")
    sys.exit(1)

#Define types
inputTubes = sys.argv[1]
outputCSVFile= sys.argv[2]

#Read output image
print("Reading input image ... ")
ReaderType = itk.SpatialObjectReader[3]
reader = ReaderType.New()
reader.SetFileName(inputTubes)
reader.Update()
inTubes = reader.GetGroup()

#Crop image
print("Compute tortuosity ... ")
ComputeTubeTortuosityMeasuresType=itktube.ComputeTubeTortuosityMeasures[3]
tortuosityFilter=ComputeTubeTortuosityMeasuresType.New()
tortuosityFilter.SetInput( reader.GetGroup() )

#Set advanced parameters
tortuosityFilter.SetSmoothingScale( 5 );
#tortuosityFilter.SetSmoothingMethod( SmoothTubeFunctionEnum_SMOOTH_TUBE_USING_INDEX_GAUSSIAN );
tortuosityFilter.SetNumberOfHistogramBins( 20 );
tortuosityFilter.SetHistogramMin( 0 );
tortuosityFilter.SetHistogramMax( 1 );

#Prepare tortuosity measure flag
tortuosityFilter.SetBasicMetrics( True );
tortuosityFilter.SetOldMetrics( True );
tortuosityFilter.SetCurvatureMetrics( True );
tortuosityFilter.SetHistogramMetrics( True );

tortuosityFilter.Update()
a =tortuosityFilter.GetOutput()
#Write output image
print a
print("Writting output image ...")
tortuosityFilter.WriteCSVfile( outputCSVFile )
