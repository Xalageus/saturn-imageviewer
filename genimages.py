from PIL import Image
import os, io, random, string

IMAGE_DIR_STR = "images"
TEX_DIR = os.path.join(os.path.abspath(path="cd"), "ART", "")
#MAX_SIZE = (320, 224)
MAX_SIZE = (352, 248)
IMAGE_PREFIX = "ART"
FORMAT = ".TGA"
SOURCE_DIR = os.path.join(os.path.abspath(path="src"))

def fullPath(file):
    return os.path.join(os.path.abspath(path=IMAGE_DIR_STR), file)

#def saveImage(num, image: Image.Image, filename):
def saveImage(num, image: Image.Image):
    image.save(TEX_DIR + IMAGE_PREFIX + str(num) + FORMAT)
    #image.save(TEX_DIR + filename + FORMAT)
    num = num + 1
    return num

def checkNotMultipleOfEight(num: int):
    return False if num % 8 == 0 else True

def resizeImage(image: Image.Image):
    if checkNotMultipleOfEight(image.width):
        nearest = 8 * round(image.width/8)
        print("Resizing width to " + str(nearest))
        image = image.resize((nearest, image.height), Image.ANTIALIAS)
    
    if checkNotMultipleOfEight(image.height):
        nearest = 8 * round(image.height/8)
        print("Resizing height to " + str(nearest))
        image = image.resize((image.width, nearest), Image.ANTIALIAS)
    
    return image

def randomEightString(prefixes):
    letters = string.ascii_uppercase
    result = ''.join(random.choice(letters) for i in range(8))
    prefixes.append(result)
    return prefixes

#image_prefixes = []

if not os.path.exists(TEX_DIR):
    os.makedirs(TEX_DIR)

images = os.listdir(path=IMAGE_DIR_STR)
im_num = 0

for image in images:
    if not image.endswith('.here'):
        #image_prefixes = randomEightString(image_prefixes)
        print("Converting " + image + " -> " + IMAGE_PREFIX + str(im_num) + FORMAT)
        #print("Converting " + image + " -> " + image_prefixes[im_num] + FORMAT)
        im = Image.open(fullPath(image))
        #im = im.convert("P")
        #im = im.quantize(colors=256, method=2)
        im.thumbnail(MAX_SIZE, Image.ANTIALIAS)
        im = resizeImage(im)
        im_num = saveImage(im_num, im)
        #im_num = saveImage(im_num, im, image_prefixes[im_num])

print("Writing images.h")
imgList = io.open(os.path.join(SOURCE_DIR, "images.h"), "w", encoding="utf-8")
imgList.write("static char* imageList[] = {")

i = 0
while i < im_num:
    imgList.write("\"" + IMAGE_PREFIX + str(i) + FORMAT + "\"")
    #imgList.write("\"" + image_prefixes[i] + FORMAT + "\"")
    if i < (im_num - 1):
        imgList.write(",")
    
    i = i + 1

imgList.write("};")
imgList.close()