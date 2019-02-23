// base64.cpp

// includes -----------------------------------------------------------
#include "pch.h"
#include "base64.h"


// private function prototypes ----------------------------------------
bool is_base64(unsigned char p_nChar);
int findidx_base64(unsigned char p_nChar);


// private variables --------------------------------------------------
static const unsigned char m_pBase64_Chars[] = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ\
abcdefghijklmnopqrstuvwxyz\
0123456789+/";


// exported functions -------------------------------------------------
int base64_encode(unsigned char *p_pSrcData, unsigned int p_nSrcLen, unsigned char *p_szDstBase64)
{
	int i = 0;
	int j = 0;
	unsigned char w_pCharArray_3[3];
	unsigned char w_pCharArray_4[4];
	unsigned int w_nDstLen;

	// check parameters
	if ((p_pSrcData == 0) || (p_nSrcLen == 0) || (p_szDstBase64 == 0))
		return RT_BASE64_INVALID_PARAM;

	w_nDstLen = 0;
	while (p_nSrcLen--)
	{
		w_pCharArray_3[i++] = *(p_pSrcData++);
		if (i == 3)
		{
			w_pCharArray_4[0] = (w_pCharArray_3[0] & 0xFC) >> 2;
			w_pCharArray_4[1] = ((w_pCharArray_3[0] & 0x03) << 4) + ((w_pCharArray_3[1] & 0xF0) >> 4);
			w_pCharArray_4[2] = ((w_pCharArray_3[1] & 0x0F) << 2) + ((w_pCharArray_3[2] & 0xC0) >> 6);
			w_pCharArray_4[3] = w_pCharArray_3[2] & 0x3F;

			for (i = 0; (i < 4); i++)
				p_szDstBase64[w_nDstLen++] = m_pBase64_Chars[w_pCharArray_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			w_pCharArray_3[j] = '\0';

		w_pCharArray_4[0] = (w_pCharArray_3[0] & 0xfc) >> 2;
		w_pCharArray_4[1] = ((w_pCharArray_3[0] & 0x03) << 4) + ((w_pCharArray_3[1] & 0xf0) >> 4);
		w_pCharArray_4[2] = ((w_pCharArray_3[1] & 0x0f) << 2) + ((w_pCharArray_3[2] & 0xc0) >> 6);

		for (j = 0; (j < i + 1); j++)
			p_szDstBase64[w_nDstLen++] = m_pBase64_Chars[w_pCharArray_4[j]];

		while ((i++ < 3))
			p_szDstBase64[w_nDstLen++] = '=';
	}

	return RT_BASE64_OK;
}

int base64_decode(unsigned char *p_pSrcBase64, unsigned int p_nSrcLen, unsigned char *p_szDstData)
{
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char w_pCharArray_3[3];
	unsigned char w_pCharArray_4[4];
	unsigned int w_nDstLen;

	// check parameters
	if ((p_pSrcBase64 == 0) || (p_nSrcLen == 0) || (p_szDstData == 0))
		return RT_BASE64_INVALID_PARAM;

	w_nDstLen = 0;
	while (p_nSrcLen-- && (p_pSrcBase64[in_] != '=') && is_base64(p_pSrcBase64[in_]))
	{
		w_pCharArray_4[i++] = p_pSrcBase64[in_];
		in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				w_pCharArray_4[i] = findidx_base64(w_pCharArray_4[i]);

			w_pCharArray_3[0] = (w_pCharArray_4[0] << 2) + ((w_pCharArray_4[1] & 0x30) >> 4);
			w_pCharArray_3[1] = ((w_pCharArray_4[1] & 0xf) << 4) + ((w_pCharArray_4[2] & 0x3c) >> 2);
			w_pCharArray_3[2] = ((w_pCharArray_4[2] & 0x3) << 6) + w_pCharArray_4[3];

			for (i = 0; (i < 3); i++)
				p_szDstData[w_nDstLen++] = w_pCharArray_3[i];
			i = 0;
		}
	}

	if (i)
	{
		for (j = 0; j < i; j++)
			w_pCharArray_4[j] = findidx_base64(w_pCharArray_4[j]);

		w_pCharArray_3[0] = (w_pCharArray_4[0] << 2) + ((w_pCharArray_4[1] & 0x30) >> 4);
		w_pCharArray_3[1] = ((w_pCharArray_4[1] & 0xf) << 4) + ((w_pCharArray_4[2] & 0x3c) >> 2);

		for (j = 0; (j < i - 1); j++)
			p_szDstData[w_nDstLen++] = w_pCharArray_3[j];
	}

	return RT_BASE64_OK;
}

int base64_encode_size(unsigned int p_nSrcLen, unsigned int *p_pnDstLen)
{
	// check parameter
	if ((p_nSrcLen == 0) || (p_pnDstLen == 0))
		return RT_BASE64_INVALID_PARAM;

	// set encoded size
	*p_pnDstLen = ((p_nSrcLen / 3) + 1) * 4;

	return RT_BASE64_OK;
}

int base64_decode_size(unsigned char *p_pSrcBase64, unsigned int p_nSrcLen, unsigned int *p_pnDstLen)
{
	unsigned int w_nPaddingCnt;

	// check parameter
	if ((p_nSrcLen == 0) || (p_pnDstLen == 0))
		return RT_BASE64_INVALID_PARAM;

	// check base64 data
	if (p_pSrcBase64[p_nSrcLen - 3] == '=')
		return RT_BASE64_INVALID_PARAM;

	// get padding count
	if ((p_pSrcBase64[p_nSrcLen - 2] == '=') && (p_pSrcBase64[p_nSrcLen - 1] == '='))
		w_nPaddingCnt = 2;
	else if (p_pSrcBase64[p_nSrcLen - 1] == '=')
		w_nPaddingCnt = 1;
	else
		w_nPaddingCnt = 0;

	// set decoded size
	*p_pnDstLen = (p_nSrcLen / 4) * 3 - w_nPaddingCnt;

	return RT_BASE64_OK;
}


// private functions --------------------------------------------------
bool is_base64(unsigned char p_nChar)
{
	if ((p_nChar == '+') || (p_nChar == '/'))
		return true;

	if ((p_nChar >= 'A') && (p_nChar <= 'Z'))
		return true;

	if ((p_nChar >= 'a') && (p_nChar <= 'z'))
		return true;

	if ((p_nChar >= '0') && (p_nChar <= '9'))
		return true;

	return false;
}

int findidx_base64(unsigned char p_nChar)
{
	int i;

	for (i = 0; i < sizeof(m_pBase64_Chars); i++)
	{
		if (m_pBase64_Chars[i] == p_nChar)
			break;
	}

	return i;
}
