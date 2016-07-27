
import sys
import site
site.addsitedir('local/lib/python2.7/site-packages/')
# Append ITK libs
#sys.path.append('/home/lucas/Projects/ITK-Release/Wrapping/Generators/Python')
#sys.path.append('/home/lucas/Projects/ITK-Release/lib')
## Append TubeTK libs :
#sys.path.append('/home/lucas/Projects/TubeTK-Release-ITK4.9/TubeTK-build/lib/TubeTK')

import itk
from itk import TubeTKITK as itktube

#Sanity checks
if len(sys.argv) != 3:
    print("Usage: " + sys.argv[0] + " <inputImage> " + " <outputImage> ")
    sys.exit(1)

#Define types
inputImage = sys.argv[1]
outputImage = sys.argv[2]

PixelType = itk.UC
OutputPixelType=itk.UC
Dimension = 2
ImageType=itk.Image[PixelType,Dimension]


#Read output image
print("Reading input image ... ")
ReaderType = itk.ImageFileReader[ImageType]
reader = ReaderType.New()
reader.SetFileName(inputImage)
reader.Update()
imInput = reader.GetOutput()

#Crop image
print("Segment Binary Image Skeleton ... ")
BinarySkeletonSegmenterType=itktube.SegmentBinaryImageSkeleton[PixelType,Dimension]
itktubeSkeletonSegmenter=BinarySkeletonSegmenterType.New()

itktubeSkeletonSegmenter.SetInput( imInput )

itktubeSkeletonSegmenter.SetRadius(4)

itktubeSkeletonSegmenter.Update()

#Write output image
print("Writting output image ...")
WriterType = itk.ImageFileWriter[ImageType]
writer = WriterType.New()
writer.SetInput(itktubeSkeletonSegmenter.GetOutput())
writer.SetFileName(outputImage)
writer.Update()
