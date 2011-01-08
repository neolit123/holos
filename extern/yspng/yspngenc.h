#ifndef YSPNGENC_IS_INCLUDED
#define YSPNGENC_IS_INCLUDED
/* { */

#ifndef YSRESULT_IS_DEFINED
#define YSERR 0
#define YSOK 1
#endif

#ifndef YSBOOL_IS_DEFINED
#define YSBOOL_IS_DEFINED
#define YSFALSE 0
#define YSTRUE  1
#endif



class YsPngCompressorState
{
friend class YsPngCompressor;
protected:
	unsigned int bufPtr,bufBit;
	unsigned int lastByte;
	unsigned int adler32_s1,adler32_s2;
	unsigned int nByteReceived;
};

class YsPngCompressor
{
protected:
	unsigned int bufSize;
	unsigned char *buf;
	unsigned int bufPtr,bufBit;

	unsigned int windowSize;

	int verboseMode;
	unsigned int adler32_s1,adler32_s2;

	unsigned int nByteExpect;
	unsigned int nByteReceived;

public:
	YsPngCompressor();
	~YsPngCompressor();

	void SaveState(YsPngCompressorState &state);
	void RestoreState(const YsPngCompressorState &state);

	int BeginCompression(unsigned int nByte);
	int AddCompressionBlock(unsigned int nByte,unsigned char byteData[],int bFinal);
	int AddNonCompressionBlock(unsigned int nByte,unsigned char byteData[],int bFinal);
	int EndCompression(void);

	unsigned int GetCompressedLength(void) const;
	const unsigned char *GetCompressedData(void) const;

protected:
	void ClearBuffer(void);
	void AddCMFandFLG(unsigned int CMF,unsigned int FLG); // This will create first 256 bytes.
	void TestAndGrowBuffer(void);

	int GetDistCodeAndExtraBit(unsigned int &distCode,unsigned int &nExtraBit,unsigned int &distExtraBit,unsigned int backDist) const;
	int GetCopyCodeAndExtraBit(unsigned int &copyCode,unsigned int &nExtraBit,unsigned int &extraBit,unsigned int copyLength) const;
	void EncodeWithNoRepetitionReduction(
	    unsigned int &nCode,unsigned int codeArray[],unsigned int copyParamArray[],
	    unsigned int nByte,unsigned char byteData[]) const;
	void EncodeWithDumbestRepetitionReduction(
	    unsigned int &nCode,unsigned int codeArray[],unsigned int copyParamArray[],
	    unsigned int nByte,unsigned char byteData[]) const;
	void EncodeWithLazyMatchAsDescribedInRFC1951(
	    unsigned int &nCode,unsigned int codeArray[],unsigned int copyParamArray[],
	    unsigned int nByte,unsigned char byteData[]) const;

	int MakeLengthLiteral(int &hLit,unsigned int hLenLit[],int nCode,unsigned int code[]) const;
	int MakeLengthCodeLength(
	    int &hCLen,unsigned int hLenCodeLen[],
	    int nLenCode,unsigned int lenCode[],int nLenDistCode,unsigned int lenDistCode[]) const;
	int MakeLengthBackDist(int &hDist,unsigned int hLenDist[],int nCode,unsigned int copyDistArray[]) const;
	void InvertHuffmanCodeForWriting(int nCode,unsigned int bitLength[],unsigned int code[]) const;

	void BeginAdler32(void);
	void AddAdler32(unsigned char byteData);
	unsigned int GetAdler32(void);

	inline void WriteMultiBit(int nBit,unsigned int bit);
	inline void WriteByte(unsigned char byte);
	inline void FlushByte(void);
};

void YsPngCompressor::WriteMultiBit(int nBit,unsigned int bit)
{
	int i;
	unsigned int testBit=1;

	TestAndGrowBuffer();

	for(i=0; i<nBit; i++)
	{
		if(bit&testBit)
		{
			buf[bufPtr]|=bufBit;
		}
		else
		{
			buf[bufPtr]&=~bufBit;
		}

		testBit<<=1;
		bufBit<<=1;
		if(256<=bufBit)
		{
			bufPtr++;
			bufBit=1;
			TestAndGrowBuffer();
		}
	}
}

void YsPngCompressor::WriteByte(unsigned char byte)
{
	FlushByte();
	TestAndGrowBuffer();
	buf[bufPtr++]=byte;
}

void YsPngCompressor::FlushByte(void)
{
	if(1!=bufBit)
	{
		bufPtr++;
		bufBit=1;
	}
}


class YsGenericPngEncoder
{
public:
	int verboseMode;

	YsGenericPngEncoder();
protected:
	int dontCompress;

	virtual int StreamOut(int nByte,const unsigned char byteData[]) const;

	void WritePngSignature(void);
	void CalculateChunkCRC(unsigned char chunk[]);

	void MakeIHDRChunk(int &nByte,unsigned char chunk[4+4+13+4],int width,int height,int bitDepth,int colorType);
	int WriteIHDRChunk(int width,int height,int bitDepth,int colorType);

	void MakeIENDChunk(int &nByte,unsigned char chunk[4+4+4]);
	int WriteIENDChunk(void);

	int WritetEXtChunk(char keyword[],char text[]);

	int WriteIDATChunk(unsigned int nLine,unsigned int bytePerLine,unsigned char dat[]);

	unsigned int CalculateBytePerLine(int width,int bitDepth,int colorType);

public:
	int Encode(int width,int height,int bitDepth,int colorType,unsigned char dat[]);

	void SetDontCompress(int dontCompress);
};


class YsRawPngEncoder : public YsGenericPngEncoder
{
protected:
	FILE *fp;
	virtual int StreamOut(int nByte,const unsigned char byteData[]) const;

public:
	int EncodeToFile(const char fn[],int width,int height,int bitDepth,int colorType,unsigned char dat[]);
};

/* } */
#endif
