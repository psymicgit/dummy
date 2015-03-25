//#include "stdafx.h"
#include "base64tool.h"
#include <sstream>
using namespace std;

const string StringEncode::Base64Encode(const void *src, unsigned int bytes) {
	ostringstream outBase64Data;
	static char base64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	const unsigned char *p1 = (unsigned char *)src;
	//char *p2 = outBase64Data;
	unsigned long i = 0, v = 0, len = bytes;

	for (i = 0; i < len; i += 3) {
		switch (len - i)
		{
		case 1:
			v = (p1[i] << 16);
			outBase64Data << base64_alphabet[v >> 18]
			<< base64_alphabet[(v >> 12) & 63]
			<< base64_alphabet[64]
			<< base64_alphabet[64];
			break;

		case 2:
			v = (p1[i] << 16) | (p1[i + 1] << 8);
			outBase64Data << base64_alphabet[v >> 18]
			<< base64_alphabet[(v >> 12) & 63]
			<< base64_alphabet[(v >> 6) & 63]
			<< base64_alphabet[64];
			break;

		default:
			v = (p1[i] << 16) | (p1[i + 1] << 8) | p1[i + 2];
			outBase64Data << base64_alphabet[v >> 18]
			<< base64_alphabet[(v >> 12) & 63]
			<< base64_alphabet[(v >> 6) & 63]
			<< base64_alphabet[v & 63];
			break;
		}
	}

	//*(p2++) = '\0';
	return outBase64Data.str();
}

int StringEncode::Base64Decode(const string &str, void *outBuff, unsigned int outSize) {
	static char base64DecodeChars[] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
		-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
		-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1};
		char c1, c2, c3, c4;
		int i, len;
		len = str.length();
		//static unsigned int BufSize = 1024;
		//static char *szBuf = new char[BufSize];
		//if (BufSize < len + 1) {
		//	BufSize = (len + 1) * 1.5f;
		//	delete[] szBuf;
		//	szBuf = new char[BufSize];
		//}
		char *out = (char *)outBuff;

		i = 0;
		int iBufIdx = 0;
		//out = "";
		bool bSuss = true;
		while(i < len) {
			/* c1 */
			do {
				c1 = base64DecodeChars[str.at(i++) & 0xff];
			} while(i < len && c1 == -1);
			if(c1 == -1) {
				bSuss = false;
				break;
			}

			/* c2 */
			do {
				c2 = base64DecodeChars[str.at(i++) & 0xff];
			} while(i < len && c2 == -1);
			if(c2 == -1) {
				bSuss = false;
				break;
			}

			if (iBufIdx >=  outSize) {
				bSuss = false;
				break;
			}
			out[iBufIdx++] = char((c1 << 2) | ((c2 & 0x30) >> 4));

			/* c3 */
			do {
				c3 = str.at(i++) & 0xff;
				if(c3 == 61)
					return iBufIdx;
				c3 = base64DecodeChars[c3];
			} while(i < len && c3 == -1);
			if(c3 == -1) {
				bSuss = false;
				break;
			}

			if (iBufIdx >=  outSize) {
				bSuss = false;
				break;
			}
			out[iBufIdx++] = char(((c2 & 0XF) << 4) | ((c3 & 0x3C) >> 2));

			/* c4 */
			do {
				c4 = str.at(i++) & 0xff;
				if(c4 == 61)
					return iBufIdx;
				c4 = base64DecodeChars[c4];
			} while(i < len && c4 == -1);
			if(c4 == -1) {
				bSuss = false;
				break;
			}
			if (iBufIdx >=  outSize) {
				bSuss = false;
				break;
			}
			out[iBufIdx++] = char(((c3 & 0x03) << 6) | c4);
		}
		return bSuss ? iBufIdx : 0;
}
