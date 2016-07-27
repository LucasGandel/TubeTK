print("Minimal path extraction test")

import sys
import site
site.addsitedir('local/lib/python2.7/site-packages/')
#sys.path.append('/home/lucas/Projects/ITK-Release/Wrapping/Generators/Python')
#sys.path.append('/home/lucas/Projects/ITK-Release/lib')
#sys.path.append('/home/lucas/Projects/TubeTK-Release-ITK4.9/TubeTK-build/lib/TubeTK')

import itk
from itk import MinimalPathExtraction as MPE

if len(sys.argv) != 3:
    print("Usage: " + sys.argv[0] + " <inputImage> " + " <outputImage> ")
    sys.exit(1)

inputImage = sys.argv[1]
outputImage = sys.argv[2]

#typedef
PixelType = itk.F
OutputPixelType=itk.UC
Dimension = 2
ImageType=itk.Image[PixelType,Dimension]
OutputImageType=itk.Image[OutputPixelType,Dimension]
PointType=itk.Point[itk.D,Dimension]
PathType=itk.PolyLineParametricPath[Dimension]

#SpeedFunction
print("Reading input image ... ")
ReaderType = itk.ImageFileReader[ImageType]
reader = ReaderType.New()
reader.SetFileName(inputImage)
reader.Update()
speed=reader.GetOutput()
speed.DisconnectPipeline()

#Interpolator
InterpolatorType=itk.LinearInterpolateImageFunction[ImageType,itk.D]
interp=InterpolatorType.New()

#Cost Function
CostFunctionType = itk.SingleImageCostFunction[ImageType]
cost=CostFunctionType.New()
cost.SetInterpolator(interp)

#Gradient Descent Optimizer
OptimizerType=itk.GradientDescentOptimizer
optimizer=OptimizerType.New()
optimizer.SetNumberOfIterations(2000)
optimizer.SetLearningRate(250)


#Path Filter
PathFilterType=MPE.SpeedFunctionToPathFilter[ImageType,PathType]
pathFilter=PathFilterType.New()
pathFilter.SetInput(speed)
pathFilter.SetCostFunction(cost)
pathFilter.SetOptimizer(optimizer)
pathFilter.SetTerminationValue(0.1)

#Path Information
# ** Path for MinimalPathExtraction_Test01.png **
start = [10,240]
end = [250,10]
way1 = [160,150]

# ** Path for MinimalPathExtraction_Test02.png **
#start = [216,118]
#end = [466,118]

PathInfoType=MPE.SpeedFunctionPathInformation[PointType]
info=PathInfoType.New()
info.SetStartPoint(start)
info.SetEndPoint(end)
info.AddWayPoint(way1) #Only for MinimalPathExtraction_Test01.png

pathFilter.AddPathInformation(info)

#Compute Minimal Path
print("Computing minimal path ...")
pathFilter.Update()
path=pathFilter.GetOutput(0)
vertexList=path.GetVertexList()
for i in range(0,vertexList.Size()-1) :
  continuousIndex=vertexList.GetElement(i)
  point=speed.TransformContinuousIndexToPhysicalPoint(continuousIndex)
  index=speed.TransformPhysicalPointToIndex(point)
  speed.SetPixel(index, itk.NumericTraits[PixelType].min() )
  i=i+1

#Cast ImageType
CastImageFilterType = itk.CastImageFilter[ImageType, OutputImageType]
castImageFilter = CastImageFilterType.New()
castImageFilter.SetInput(speed)

#Write output image
print("Writting output image ...")
WriterType = itk.ImageFileWriter[OutputImageType]
writer = WriterType.New()
writer.SetInput(castImageFilter.GetOutput())
writer.SetFileName(outputImage)
writer.Update()

print("End of minimal path test")
