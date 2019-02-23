// base64_lib.cpp

// includes -----------------------------------------------------------
#include "pch.h"
#include <stdio.h>
#include <memory.h>
#include "base64.h"


// private variables --------------------------------------------------
const unsigned char g_szSampleBase64[] = "VGhpcyBpcyBiYXNlNjQgZGVjcnB5dCBzYW1wbGUuCkl0IHNob3dzIHNhbXBsZSB0ZXh0Lg==";


int main()
{
	unsigned char *w_pSrcBase64;
	unsigned int w_nSrcLen;
	unsigned char *w_pDstData;
	unsigned int w_nDstLen;
	int i;

	// read base64
	w_nSrcLen = sizeof(g_szSampleBase64) - 1;
	w_pSrcBase64 = new unsigned char[w_nSrcLen];
	memcpy(w_pSrcBase64, g_szSampleBase64, w_nSrcLen);

	// get real data
	base64_decode_size(w_pSrcBase64, w_nSrcLen, &w_nDstLen);
	w_pDstData = new unsigned char[w_nDstLen + 3];
	memset(w_pDstData, 0x00, w_nDstLen);
	base64_decode(w_pSrcBase64, w_nSrcLen, w_pDstData);

	// show decoded data
	printf_s("Decoded Data :");
	for (i = 0; i < (int)w_nDstLen; i++)
	{
		if (i % 16 == 0)
			printf_s("\r\n");
		else if (i % 8 == 0)
			printf_s(" ");

		printf_s("%02X ", w_pDstData[i]);
	}

	printf_s("\r\n");

	delete[] w_pSrcBase64;
	delete[] w_pDstData;

	return 0;
}
