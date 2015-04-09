#ifndef QDES_H_
#define QDES_H_

#include "stdio.h"
#include "memory.h"
#include "time.h"
#include "stdlib.h"

#ifdef  __cplusplus
extern "C"
{
#endif
int ByteToBit(char ch, char bit[8]);
int BitToByte(char bit[8], char *ch);
int Char8ToBit64(char ch[8], char bit[64]);
int Bit64ToChar8(char bit[64], char ch[8]);
int DES_MakeSubKeys(char key[64], char subKeys[16][48]);
int DES_PC1_Transform(char key[64], char tempbts[56]);
int DES_PC2_Transform(char key[56], char tempbts[48]);
int DES_ROL(char data[56], int time);
int DES_IP_Transform(char data[64]);
int DES_IP_1_Transform(char data[64]);
int DES_E_Transform(char data[48]);
int DES_P_Transform(char data[32]);
int DES_SBOX(char data[48]);
int DES_XOR(char R[48], char L[48], int count);
int DES_Swap(char left[32], char right[32]);
int QDES(char *key, char *s_text, char *d_text);
int _QDES(char *key, char *s_text, char *d_text);

int OXQDES(char *key, char*s_text, char *d_text);
int _OXQDES(char *key, char *s_text, char *d_text);
int FQDES(char *key, char *s_text, int s_len, char *d_text, int *d_len); //¼ÓÃÜÎÄ¼þ

#ifdef  __cplusplus
}
#endif
#endif /* QDES_H_ */
