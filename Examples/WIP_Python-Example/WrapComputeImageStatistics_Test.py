import sys
import site
site.addsitedir('local/lib/python2.7/site-packages/')

import itk
from itk import TubeTKITK as itktube

print("Compute Image Statistics")

#Sanity checks
if len(sys.argv) != 4:
    print("Usage: " + sys.argv[0] + " <inputImage> " + "<inputMask>" + " <outputImage> ")
    sys.exit(1)

#Define types
inputImage = sys.argv[1]
inputMask = sys.argv[2]
outputImage = sys.argv[3]

PixelType = itk.UC
Dimension = 2
MaskType=itk.Image[PixelType,Dimension]
ImageType=itk.Image[itk.F,Dimension]

#Read output image
print("Reading input Mask ... ")
MaskReaderType = itk.ImageFileReader[MaskType]
maskReader = MaskReaderType.New()
maskReader.SetFileName(inputMask)
maskReader.Update()
maskInput = maskReader.GetOutput()

#Read output image
print("Reading input image ... ")
ReaderType = itk.ImageFileReader[ImageType]
reader = ReaderType.New()
reader.SetFileName(inputImage)
reader.Update()
imInput = reader.GetOutput()


ComputeImageStatisticsType = itktube.ComputeImageStatistics[PixelType,Dimension]
computeImageStatistics=ComputeImageStatisticsType.New()

print(dir(computeImageStatistics))

computeImageStatistics.SetInput(imInput)
computeImageStatistics.SetInputMask(maskInput)
computeImageStatistics.SetQuantiles([0.25,0.5,0.75])
print(computeImageStatistics)

computeImageStatistics.Update()

computeImageStatistics.WriteCSVStatistics("data/out.csv")

#Write output image
print("Writting output image ...")
WriterType = itk.ImageFileWriter[ImageType]
writer = WriterType.New()
writer.SetInput(computeImageStatistics.GetOutput())
writer.SetFileName(outputImage)
writer.Update()
