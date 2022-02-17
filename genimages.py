from PIL import Image, ImagePalette
import os, io, random, string

IMAGE_DIR_STR = "images"
TEX_DIR = os.path.join(os.path.abspath(path="cd"), "ART", "")
#MAX_SIZE = (320, 224)
MAX_SIZE = (352, 248)
#MAX_SIZE = (160, 112)
IMAGE_PREFIX = "ART"
FORMAT = ".TGA"
SOURCE_DIR = os.path.join(os.path.abspath(path="src"))

def fullPath(file: str) -> str:
    return os.path.join(os.path.abspath(path=IMAGE_DIR_STR), file)

def saveImage(num: int, image: Image.Image) -> int:
    image.save(TEX_DIR + IMAGE_PREFIX + str(num) + FORMAT, compression=None)
    num = num + 1
    return num

def checkNotMultipleOfEight(num: int) -> bool:
    return False if num % 8 == 0 else True

def resizeImage(image: Image.Image) -> Image.Image:
    if checkNotMultipleOfEight(image.width):
        nearest = 8 * round(image.width/8)
        print("Resizing width to " + str(nearest))
        image = image.resize((nearest, image.height), Image.ANTIALIAS)
    
    if checkNotMultipleOfEight(image.height):
        nearest = 8 * round(image.height/8)
        print("Resizing height to " + str(nearest))
        image = image.resize((image.width, nearest), Image.ANTIALIAS)
    
    return image

def quantizeToPalette(image: Image.Image) -> Image.Image:
    image = image.quantize(colors=256, method=2)
    image = image.convert("P", dither=1, palette=image.palette, colors=256)
    return image

if not os.path.exists(TEX_DIR):
    os.makedirs(TEX_DIR)

images = os.listdir(path=IMAGE_DIR_STR)
im_num = 0

for image in images:
    if not image.endswith('.here'):
        print("Converting " + image + " -> " + IMAGE_PREFIX + str(im_num) + FORMAT)
        im = Image.open(fullPath(image))
        im = quantizeToPalette(im)
        im.thumbnail(MAX_SIZE, Image.ANTIALIAS)
        im = resizeImage(im)
        im_num = saveImage(im_num, im)

print("Writing images.h")
imgList = io.open(os.path.join(SOURCE_DIR, "images.h"), "w", encoding="utf-8")
imgList.write("static char* imageList[] = {")

i = 0
while i < im_num:
    imgList.write("\"" + IMAGE_PREFIX + str(i) + FORMAT + "\"")
    if i < (im_num - 1):
        imgList.write(",")
    
    i = i + 1

imgList.write("};")
imgList.close()