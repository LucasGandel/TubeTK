
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
OutputImageType=itk.Image[OutputPixelType,Dimension]

#Read output image
print("Reading input image ... ")
ReaderType = itk.ImageFileReader[ImageType]
reader = ReaderType.New()
reader.SetFileName(inputImage)
reader.Update()
imInput = reader.GetOutput()

#Crop image
print("Crop image ... ")
itktubeCropImageType=itktube.CropImage[ImageType,OutputImageType]
itktubecropImage=itktubeCropImageType.New()

itktubecropImage.SetInput( imInput )

mini = [50,50]
maxi = [250,250]
itktubecropImage.SetMin(mini)
itktubecropImage.SetMax(maxi)

itktubecropImage.Update()

#Write output image
print("Writting output image ...")
WriterType = itk.ImageFileWriter[OutputImageType]
writer = WriterType.New()
writer.SetInput(itktubecropImage.GetOutput())
writer.SetFileName(outputImage)
writer.Update()
