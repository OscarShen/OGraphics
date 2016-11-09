#include "TGALoader.h"
#include <fstream>
#include <iostream>
#include <assert.h>
/*
˵˵�������^������һ���������á�
�������㷽����һ�����������㣺
1^1=0
0^0=0
1^0=1
0^1=1

�������Ϊ0,����Ϊ1.

�������Ƿ��ֽ�������������ֵʱ���Բ��õ�����������
��a=11,b=9.�����Ƕ�����
a=a^b=1011^1001=0010;
b=b^a=1001^0010=1011;
a=a^b=0010^1011=1001;
����һ��a=9,b=13�ˡ�

��һ�����ã� ��һ����ť��������mc��λ�ÿ���������

mybt.onPress=function()
{
mc1._x=mc1._x^mc2._x;
mc2._x=mc2._x^mc1._x;
mc1._x=mc1._x^mc2._x;

mc1._y=mc1._y^mc2._y;
mc2._y=mc2._y^mc1._y;
mc1._y=mc1._y^mc2._y;
}

�����Ϳ��Բ�ͨ����ʱ�����������ˡ�

���Ҫ������ֻ������������

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
// ������    : Image::loadTGA
// ˵��      : ����TGAͼƬ
// ����      : BOOL 
// ����      : TexImage* texture
// ����      : LPCSTR filename 
// ����      : Teng
// ����ʱ��  : 2010-4-14 15:35:32
// ����޸�  : 2010-4-14
//-----------------------------------------------------------------------
bool TGAImage::loadTGA(TexImage* texture, const GLchar* filename)
{
	if (filename == nullptr)
		return false;

	Header uTGAcompare = { 0,0,2,0,0,0,0,0 };        //2Ϊ��ѹ��RGB��ʽ        3  -  δѹ���ģ��ڰ�ͼ��
	Header cTGAcompare = { 0,0,10,0,0,0,0,0 };        //10Ϊѹ��RGB��ʽ

	TGAHeader header;
	FILE* file;
	fopen_s(&file, filename, "rb");
	if (!file) {
		std::cout << "Open file \"" << filename << "\" faled!" << std::endl;
		return false;
	}

	if (fread(&header, 1, sizeof(TGAHeader), file) != sizeof(TGAHeader)) {        //��ȡTGA����ͷ�ṹ��
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


	if (memcmp(&uTGAcompare, &header.head, sizeof(header.head)) == 0) {            //δѹ��TGA
		texture->bCompressed = false;
		if (!loadUncompressedTGA(texture, file)) {
			std::cout << "Load uncompressed TGA failed!" << std::endl;
			return false;
		}
	}
	else if (memcmp(&cTGAcompare, &header.head, sizeof(header.head)) == 0) {    //ѹ��TGA
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
// ������    : Image::generateTexture
// ˵��      : ����һ������
// ����      : void 
// ����      : TexImage* texture
// ����      : BOOL bMipmap /*= TRUE*/ 
// ����      : Teng
// ����ʱ��  : 2010-5-25 17:06:42
// ����޸�  : 2010-5-25
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
// ������    : Image::loadUncompressedTGA
// ˵��      : ����δѹ��TGA����
// ����      : BOOL 
// ����      : TexImage* texture
// ����      : FILE* file        ��ǰfileָ�룬ָ��TGAͼ���һ�����ص�ַ
// ����      : Teng
// ����ʱ��  : 2010-4-14 14:39:22
// ����޸�  : 2010-4-14
//-----------------------------------------------------------------------
bool TGAImage::loadUncompressedTGA(TexImage* texture, FILE* file)
{
	assert(file != NULL && texture != NULL);

	GLuint bytePerPixel = texture->bpp / 8;
	GLuint imgSize = texture->width*texture->height*bytePerPixel;                //ͼ�����ֽ���
	texture->imageData = new GLubyte[imgSize];

	if (fread(texture->imageData, 1, imgSize, file) != imgSize)
	{
		std::cout << "Read texture imagedata failed!" << std::endl;
		return false;
	}

	//TGA��������OpenGL�ĸ�ʽ,Ҫ��BGRת����ΪRGB
	// Go through all of the pixels and swap the B and R values since TGA
	// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
	for (int i = 0; i < (int)imgSize; i += bytePerPixel) {
		/*GLushort temp = texture->imageData[i];
		texture->imageData[i] = texture->imageData[i+2];
		texture->imageData[i+2] = temp;*/
		texture->imageData[i] ^= texture->imageData[i + 2] ^= texture->imageData[i] ^= texture->imageData[i + 2];        //λ��������ٶ�,����B,R����
	}

	::fclose(file);
	return true;
}

//-----------------------------------------------------------------------
// ������    : Image::loadCompressedTGA
// ˵��      : ����ѹ��TGA����
// ����      : BOOL 
// ����      : TexImage* texture
// ����      : FILE* file 
// ����      : Teng
// ����ʱ��  : 2010-4-14 14:38:55
// ����޸�  : 2010-4-14
//-----------------------------------------------------------------------
bool TGAImage::loadCompressedTGA(TexImage* texture, FILE* file)
{
	assert(file != NULL && texture != NULL);

	GLuint bytePerPixel = texture->bpp / 8;
	GLuint imgSize = texture->width*texture->height*bytePerPixel;
	texture->imageData = new GLubyte[imgSize];

	GLuint pixelcount = texture->width * texture->height;
	GLuint currentPixel = 0;        //��ǰ���ڶ�ȡ������
	GLuint currentByte = 0;            //��ǰ������ͼ����д�������
	GLubyte *colorbuffer = (GLubyte *)malloc(bytePerPixel);    // һ�����صĴ洢�ռ�s

	do
	{
		GLubyte chunkHeader = 0;        //�洢ID��ֵ�ı���
		if (!fread(&chunkHeader, 1, sizeof(GLubyte), file)) {
			return false;
		}
		if (chunkHeader < 128)            //RAW��
		{
			chunkHeader++;                // ����ֵ��1�Ի�ȡRAW���ص�����

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
		//��һ�δ�������RLE�εġ��顱ͷ���������ǽ�chunkheader��ȥ127���õ���ȡ��һ����ɫ�ظ��Ĵ����� 
		else
		{
			chunkHeader -= 127;            //��ȥ127���ID bit ��rid    ��ʼѭ���������Ƕ�ζ����ڴ��е����أ�����RLEͷ�е�ֵ�涨��

			if (fread(colorbuffer, 1, sizeof(bytePerPixel), file) != sizeof(bytePerPixel)) {
				std::cout << "Read pixel failed!" << std::endl;
				return false;
			}

			for (int i = 0; i < chunkHeader; i++) {
				texture->imageData[currentByte] = colorbuffer[2];                // ������R���ֽ�
				texture->imageData[currentByte + 1] = colorbuffer[1];            // ������G���ֽ�
				texture->imageData[currentByte + 2] = colorbuffer[0];            // ������B���ֽ�
				if (bytePerPixel == 4)
					texture->imageData[currentByte + 3] = colorbuffer[3];            // ������A���ֽ�

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
// ������    : Image::release
// ˵��      : �ͷ���Դ
// ����      : BOOL 
// ����      : TexImage* texture 
// ����      : Teng
// ����ʱ��  : 2010-5-25 17:42:59
// ����޸�  : 2010-5-25
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