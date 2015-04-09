#include <sdstring.h>   

INT32 Base64Decode(UINT8 * out, const CHAR *in, INT32 out_length);

CHAR *Base64Encode(CHAR * buf, INT32 buf_len, const UINT8 * src, INT32 len);
    
CHAR GetBase64Value(CHAR ch);
