#include "TGALoader.h"
#include <fstream>
#include <iostream>
#include <assert.h>
/*
说说异或运算^和他的一个常用作用。
异或的运算方法是一个二进制运算：
1^1=0
0^0=0
1^0=1
0^1=1

两者相等为0,不等为1.

这样我们发现交换两个整数的值时可以不用第三个参数。
如a=11,b=9.以下是二进制
a=a^b=1011^1001=0010;
b=b^a=1001^0010=1011;
a=a^b=0010^1011=1001;
这样一来a=9,b=13了。

举一个运用， 按一个按钮交换两个mc的位置可以这样。

mybt.onPress=function()
{
mc1._x=mc1._x^mc2._x;
mc2._x=mc2._x^mc1._x;
mc1._x=mc1._x^mc2._x;

mc1._y=mc1._y^mc2._y;
mc2._y=mc2._y^mc1._y;
mc1._y=mc1._y^mc2._y;
}

这样就可以不通过监时变量来传递了。

最后要声明：只能用于整数。

vertex[0].position = vec2( -width/2, height/2 );
vertex[1].position = vec2( width/2, height/2 );
vertex[2].position = vec2( width/2, -height/2 );
vertex[3].position = vec2( -width/2, -height/2 );
*/

TGAImage::TGAImage(void)
{
}

TGAImage::~TGAImage(void)
{
}


//-----------------------------------------------------------------------
// 函数名    : Image::loadTGA
// 说明      : 加载TGA图片
// 返回      : BOOL 
// 参数      : TexImage* texture
// 参数      : LPCSTR filename 
// 作者      : Teng
// 创建时间  : 2010-4-14 15:35:32
// 最后修改  : 2010-4-14
//-----------------------------------------------------------------------
bool TGAImage::loadTGA(TexImage* texture, const GLchar* filename)
{
	if (filename == nullptr)
		return false;

	Header uTGAcompare = { 0,0,2,0,0,0,0,0 };        //2为非压缩RGB格式        3  -  未压缩的，黑白图像
	Header cTGAcompare = { 0,0,10,0,0,0,0,0 };        //10为压缩RGB格式

	TGAHeader header;
	FILE* file;
	fopen_s(&file, filename, "rb");
	if (!file) {
		std::cout << "Open file \"" << filename << "\" faled!" << std::endl;
		return false;
	}

	if (fread(&header, 1, sizeof(TGAHeader), file) != sizeof(TGAHeader)) {        //读取TGA整个头结构体
		if (file)
			fclose(file);
		std::cout << "Read data failed" << std::endl;
		return false;
	}

	texture->width = header.width;
	texture->height = header.height;
	texture->bpp = header.bpp;

	if (header.bpp == 32)
		texture->imageType = GL_RGBA;
	else if (header.bpp = 24)
		texture->imageType = GL_RGB;
	else {
		std::cout << "Image type error!" << std::endl;
		return false;
	}


	if (memcmp(&uTGAcompare, &header.head, sizeof(header.head)) == 0) {            //未压缩TGA
		texture->bCompressed = false;
		if (!loadUncompressedTGA(texture, file)) {
			std::cout << "Load uncompressed TGA failed!" << std::endl;
			return false;
		}
	}
	else if (memcmp(&cTGAcompare, &header.head, sizeof(header.head)) == 0) {    //压缩TGA
		texture->bCompressed = true;
		if (!loadCompressedTGA(texture, file)) {
			std::cout << "Load compressed TGA failed!" << std::endl;
			return false;
		}
	}
	else {
		std::cout << "Error TGA type!" << std::endl;
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------
// 函数名    : Image::generateTexture
// 说明      : 生成一张纹理
// 返回      : void 
// 参数      : TexImage* texture
// 参数      : BOOL bMipmap /*= TRUE*/ 
// 作者      : Teng
// 创建时间  : 2010-5-25 17:06:42
// 最后修改  : 2010-5-25
//-----------------------------------------------------------------------
//void TGAImage::generateTexture(TexImage * texture, bool bMipmap)
//{
//	//Bulid a textue from the data.
//	glGenTextures(1, &texture->texID);
//
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_texWrap);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_texWrap);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_texFilter);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_texFilter);
//	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_texMode);
//
//	glBindTexture(GL_TEXTURE_2D, texture->texID);
//
//	if (bMipmap) {
//		gluBuild2DMipmaps(GL_TEXTURE_2D, texture->bpp / 8, texture->width, texture->height, texture->imageType, GL_UNSIGNED_BYTE, texture->imageData);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	}
//	else {
//		glTexImage2D(GL_TEXTURE_2D, 0, texture->bpp / 8, texture->width, texture->height, 0, texture->imageType, GL_UNSIGNED_BYTE, texture->imageData);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	}
//}

//-----------------------------------------------------------------------
// 函数名    : Image::loadUncompressedTGA
// 说明      : 加载未压缩TGA纹理
// 返回      : BOOL 
// 参数      : TexImage* texture
// 参数      : FILE* file        当前file指针，指向TGA图像第一个像素地址
// 作者      : Teng
// 创建时间  : 2010-4-14 14:39:22
// 最后修改  : 2010-4-14
//-----------------------------------------------------------------------
bool TGAImage::loadUncompressedTGA(TexImage* texture, FILE* file)
{
	assert(file != NULL && texture != NULL);

	GLuint bytePerPixel = texture->bpp / 8;
	GLuint imgSize = texture->width*texture->height*bytePerPixel;                //图像总字节数
	texture->imageData = new GLubyte[imgSize];

	if (fread(texture->imageData, 1, imgSize, file) != imgSize)
	{
		std::cout << "Read texture imagedata failed!" << std::endl;
		return false;
	}

	//TGA采用了逆OpenGL的格式,要将BGR转换成为RGB
	// Go through all of the pixels and swap the B and R values since TGA
	// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
	for (int i = 0; i < (int)imgSize; i += bytePerPixel) {
		/*GLushort temp = texture->imageData[i];
		texture->imageData[i] = texture->imageData[i+2];
		texture->imageData[i+2] = temp;*/
		texture->imageData[i] ^= texture->imageData[i + 2] ^= texture->imageData[i] ^= texture->imageData[i + 2];        //位操作提高速度,更换B,R分量
	}

	::fclose(file);
	return true;
}

//-----------------------------------------------------------------------
// 函数名    : Image::loadCompressedTGA
// 说明      : 加载压缩TGA纹理
// 返回      : BOOL 
// 参数      : TexImage* texture
// 参数      : FILE* file 
// 作者      : Teng
// 创建时间  : 2010-4-14 14:38:55
// 最后修改  : 2010-4-14
//-----------------------------------------------------------------------
bool TGAImage::loadCompressedTGA(TexImage* texture, FILE* file)
{
	assert(file != NULL && texture != NULL);

	GLuint bytePerPixel = texture->bpp / 8;
	GLuint imgSize = texture->width*texture->height*bytePerPixel;
	texture->imageData = new GLubyte[imgSize];

	GLuint pixelcount = texture->width * texture->height;
	GLuint currentPixel = 0;        //当前正在读取的像素
	GLuint currentByte = 0;            //当前正在向图像中写入的像素
	GLubyte *colorbuffer = (GLubyte *)malloc(bytePerPixel);    // 一个像素的存储空间s

	do
	{
		GLubyte chunkHeader = 0;        //存储ID块值的变量
		if (!fread(&chunkHeader, 1, sizeof(GLubyte), file)) {
			return false;
		}
		if (chunkHeader < 128)            //RAW块
		{
			chunkHeader++;                // 变量值加1以获取RAW像素的总数

			for (int i = 0; i < chunkHeader; i++) {
				if (fread(colorbuffer, 1, sizeof(bytePerPixel), file) != sizeof(bytePerPixel)) {
					std::cout << "Read pixel failed!" << std::endl;
					return false;
				}
				texture->imageData[currentByte] = colorbuffer[2];
				texture->imageData[currentByte + 1] = colorbuffer[1];
				texture->imageData[currentByte + 2] = colorbuffer[0];
				if (bytePerPixel == 4)
					texture->imageData[currentByte + 3] = colorbuffer[3];

				currentPixel++;
				currentByte += bytePerPixel;
			}
		}
		//下一段处理描述RLE段的“块”头。首先我们将chunkheader减去127来得到获取下一个颜色重复的次数。 
		else
		{
			chunkHeader -= 127;            //减去127获得ID bit 的rid    开始循环拷贝我们多次读到内存中的像素，这由RLE头中的值规定。

			if (fread(colorbuffer, 1, sizeof(bytePerPixel), file) != sizeof(bytePerPixel)) {
				std::cout << "Read pixel failed!" << std::endl;
				return false;
			}

			for (int i = 0; i < chunkHeader; i++) {
				texture->imageData[currentByte] = colorbuffer[2];                // 拷贝“R”字节
				texture->imageData[currentByte + 1] = colorbuffer[1];            // 拷贝“G”字节
				texture->imageData[currentByte + 2] = colorbuffer[0];            // 拷贝“B”字节
				if (bytePerPixel == 4)
					texture->imageData[currentByte + 3] = colorbuffer[3];            // 拷贝“A”字节

				currentPixel++;
				currentByte += bytePerPixel;
			}
		}
	} while (currentPixel < pixelcount);

	free(colorbuffer);
	::fclose(file);
	return true;
}


//-----------------------------------------------------------------------
// 函数名    : Image::release
// 说明      : 释放资源
// 返回      : BOOL 
// 参数      : TexImage* texture 
// 作者      : Teng
// 创建时间  : 2010-5-25 17:42:59
// 最后修改  : 2010-5-25
//-----------------------------------------------------------------------
bool TGAImage::release(TexImage* texture)
{
	if (!texture)
		return false;

	if (texture->imageData)
		delete[] texture->imageData;
	glDeleteTextures(1, &texture->texID);
	return true;
}