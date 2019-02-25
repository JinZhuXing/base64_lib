// base64.h

#ifndef __BASE64_H_
#define	__BASE64_H_


// global defines -----------------------------------------------------
// return codes
#define	RT_BASE64_OK							0
#define	RT_BASE64_FAIL							1
#define RT_BASE64_INVALID_PARAM					2


// exported functions -------------------------------------------------
int base64_encode(unsigned char *p_pSrcData, unsigned int p_nSrcLen, unsigned char *p_szDstBase64);
int base64_decode(unsigned char *p_pSrcBase64, unsigned int p_nSrcLen, unsigned char *p_szDstData);
int base64_encode_size(unsigned int p_nSrcLen, unsigned int *p_pnDstLen);
int base64_decode_size(unsigned char *p_pSrcBase64, unsigned int p_nSrcLen, unsigned int *p_pnDstLen);
int base64_check_encoded_file(unsigned char *p_pSrcBase64, unsigned int *p_nSrcLen);


#endif // !__BASE64_H_
