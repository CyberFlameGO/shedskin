/* Copyright 2005-2011 Mark Dufour and contributors; License Expat (See LICENSE) */

#include "binascii.hpp"
#include <climits>

namespace __binascii__ {

str *__name__;

void * default_4;
__ss_bool  default_1;
__ss_bool  default_0;
__ss_bool  default_3;
__ss_bool  default_2;
void * default_5;

// XXX
#define PY_SSIZE_T_MAX INT_MAX 

/**
class Error
*/

class_ *cl_Error;

/**
class Incomplete
*/

class_ *cl_Incomplete;

bytes *hexlify(bytes *data) {
    // output will be twice as long
    __ss_int len = data->__len__();
    __GC_STRING hexstr = __GC_STRING(data->unit);
    hexstr.reserve(len<<1);
    hexstr.resize(len<<1);
    bytes *hex = new bytes(hexstr);

    char * curdata = &data->unit[0];
    char * curhex = &hex->unit[0];
    char * end = curdata+len;
    char c;
    // from python's implementation (2.7.1, if it matters)
    while(curdata <= end)
    {
        c = (*curdata>>4) & 0xf;
        c = (c>9) ? c+'a'-10 : c + '0';
        *(curhex++) = c;
        c = (*curdata) & 0xf;
        c = (c>9) ? c+'a'-10 : c + '0';
        *(curhex++) = c;
        curdata++;
    }
    return hex;
}

/*
The following table generated with:
lst = [i-ord('0')
    if (i>=ord('0') and i<=ord('9'))
    else (i-ord('a')+10
        if (i>=ord('a') and i<=ord('f'))
        else (i-ord('A')+10
            if (i>=ord('A') and i<=ord('F'))
            else -1))
    for i in xrange(256)]

for i in xrange(0,256,16):
    # lst[i:i+16] lines
    sys.stdout.write('    ')
    for j in xrange(i,i+16,4):
        # lst[j:j+4] groups
        for k in xrange(j,j+4):
            # lst[k] numbers
            sys.stdout.write(str(lst[k]).rjust(2)+',')
        if j+4!=i+16: # if not last group
            sys.stdout.write(' ')
    sys.stdout.write('\n')

*/

static char table_a2b_hex[] = {
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
     0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,
    -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
};


bytes *unhexlify(bytes *hex) {
    // output will be half as long
    __ss_int len = hex->__len__();
    if ( len&1 ) throw new Error(0); //new str("Odd-length string"));
    bytes *data = new bytes("",len>>1);

    char * curdata = &data->unit[0];
    char * curhex = &hex->unit[0];
    char * end = curhex+len;
    char top,bot;
    // from python's implementation (2.7.1, if it matters), but way better :)
    while(curhex <= end-2) // must be two characters left
    {
        top = table_a2b_hex[(int)*(curhex++)];
        bot = table_a2b_hex[(int)*(curhex++)];
        if (top==-1 || bot==-1)
            throw new Error(0); //new str("Invalid hex"));
        *(curdata++) = (top<<4) + bot;
    }
    return data;
}



// from python 2.7.1
bytes *a2b_uu(bytes *string) {
    __ss_int ascii_len = string->__len__();
    char * ascii_data = &string->unit[0];

    __ss_int bin_len = (*ascii_data++ - ' ') & 077;
    bytes *binary = new bytes("",bin_len);
    char * bin_data = &binary->unit[0];
    unsigned char this_ch;
    __ss_int leftchar=0, leftbits=0;

    ascii_len--;
    for( ; bin_len > 0 ; ascii_len--, ascii_data++ ) {
        /* XXX is it really best to add NULs if there's no more data */
        this_ch = (ascii_len > 0) ? *ascii_data : 0;
        if ( this_ch == '\n' || this_ch == '\r' || ascii_len <= 0) {
            /*
            ** Whitespace. Assume some spaces got eaten at
            ** end-of-line. (We check this later)
            */
            this_ch = 0;
        } else {
            /* Check the character for legality
            ** The 64 in stead of the expected 63 is because
            ** there are a few uuencodes out there that use
            ** '`' as zero instead of space.
            */
            if ( this_ch < ' ' || this_ch > (' ' + 64)) {
                throw new Error(0);
            }
            this_ch = (this_ch - ' ') & 077;
        }
        /*
        ** Shift it in on the low end, and see if there's
        ** a byte ready for output.
        */
        leftchar = (leftchar << 6) | (this_ch);
        leftbits += 6;
        if ( leftbits >= 8 ) {
            leftbits -= 8;
            *bin_data++ = (leftchar >> leftbits) & 0xff;
            leftchar &= ((1 << leftbits) - 1);
            bin_len--;
        }
    }


    /*
    ** Finally, check that if there's anything left on the line
    ** that it's whitespace only.
    */
    while( ascii_len-- > 0 ) {
        this_ch = *ascii_data++;
        /* Extra '`' may be written as padding in some cases */
        if ( this_ch != ' ' && this_ch != ' '+64 &&
             this_ch != '\n' && this_ch != '\r' ) {
            throw new Error(0);
        }
    }

    return binary;
}

bytes *b2a_uu(bytes *binary) {
    __ss_int bin_len = binary->__len__();
    if ( bin_len > 45 ) {
        /* The 45 is a limit that appears in all uuencode's */
        throw new Error(0); //At most 45 bytes at once
    }

    char * bin_data = &binary->unit[0];

    /* We're lazy and allocate too much (fixed up later) */
    __ss_int ascii_len = 2 + (bin_len+2)/3*4;
    bytes * ascii = new bytes("",ascii_len);
    char * ascii_data = &ascii->unit[0];
    char * ascii_start = ascii_data;
    unsigned char this_ch;
    __ss_int leftchar=0, leftbits=0;

    /* Store the length */
    *ascii_data++ = ' ' + (bin_len & 077);

    for( ; bin_len > 0 || leftbits != 0 ; bin_len--, bin_data++ ) {
        /* Shift the data (or padding) into our buffer */
        if ( bin_len > 0 )              /* Data */
            leftchar = (leftchar << 8) | *bin_data;
        else                            /* Padding */
            leftchar <<= 8;
        leftbits += 8;

        /* See if there are 6-bit groups ready */
        while ( leftbits >= 6 ) {
            this_ch = (leftchar >> (leftbits-6)) & 0x3f;
            leftbits -= 6;
            *ascii_data++ = this_ch + ' ';
        }
    }
    *ascii_data++ = '\n';       /* Append a courtesy newline */

    ascii->unit.resize( (ascii_data - ascii_start) );

    return ascii;
}

static char table_a2b_base64[] = {
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,62, -1,-1,-1,63,
    52,53,54,55, 56,57,58,59, 60,61,-1,-1, -1, 0,-1,-1, /* Note PAD->0 */
    -1, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,-1, -1,-1,-1,-1,
    -1,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,-1, -1,-1,-1,-1
};

#define BASE64_PAD '='

/* Max binary chunk size; limited only by available memory */
#define BASE64_MAXBIN (PY_SSIZE_T_MAX/2 - sizeof(PyStringObject) - 3)

static unsigned char table_b2a_base64[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";



int find_valid(char *s, __ss_int slen, int num)
{
    /* Finds & returns the (num+1)th
    ** valid character for base64, or -1 if none.
    */

    int ret = -1;
    unsigned char c, b64val;

    while ((slen > 0) && (ret == -1)) {
        c = *s;
        b64val = table_a2b_base64[c & 0x7f];
        if ( ((c <= 0x7f) && (b64val != (unsigned char)-1)) ) {
            if (num == 0)
                ret = *s;
            num--;
        }

        s++;
        slen--;
    }
    return ret;
}

// from python 2.7.1
bytes *a2b_base64(bytes *pascii) {
    char * ascii_data = &pascii->unit[0];
    __ss_int ascii_len = pascii->__len__();
    if (ascii_len > PY_SSIZE_T_MAX-3) {
        throw new Error(0);
    }

    int quad_pos = 0;
    int leftbits = 0;
    unsigned char this_ch;
    unsigned int leftchar = 0;

    __ss_int bin_len = ((ascii_len+3)/4)*3; /* Upper bound, corrected later */
    bytes *binary = new bytes("",bin_len);
    char * bin_data = &binary->unit[0];
    bin_len = 0;

    for( ; ascii_len > 0; ascii_len--, ascii_data++) {
        this_ch = *ascii_data;

        if (this_ch > 0x7f ||
            this_ch == '\r' || this_ch == '\n' || this_ch == ' ')
            continue;

        /* Check for pad sequences and ignore
        ** the invalid ones.
        */
        if (this_ch == BASE64_PAD) {
            if ( (quad_pos < 2) ||
                 ((quad_pos == 2) &&
                  (find_valid(ascii_data, ascii_len, 2)
                   != BASE64_PAD)) )
            {
                continue;
            }
            else {
                /* A pad sequence means no more input.
                ** We've already interpreted the data
                ** from the quad at this point.
                */
                leftbits = 0;
                break;
            }
        }

        this_ch = table_a2b_base64[*ascii_data];
        if ( this_ch == (unsigned char) -1 )
            continue;

        /*
        ** Shift it in on the low end, and see if there's
        ** a byte ready for output.
        */
        quad_pos = (quad_pos + 1) & 0x03;
        leftchar = (leftchar << 6) | (this_ch);
        leftbits += 6;

        if ( leftbits >= 8 ) {
            leftbits -= 8;
            *bin_data++ = (leftchar >> leftbits) & 0xff;
            bin_len++;
            leftchar &= ((1 << leftbits) - 1);
        }
    }

    if (leftbits != 0) {
        throw new Error(0); // "Incorrect padding"
    }

    /* And set string size correctly. If the result string is empty
    ** (because the input was all invalid) return the shared empty
    ** string instead; _PyString_Resize() won't do this for us.
    */
    if (bin_len > 0) {
        binary->unit.resize(bin_len);
    } else {
        // reset binary to ""
        binary = new bytes("");
    }
    return binary;
}

bytes *b2a_base64(bytes *binary) {

    __ss_int bin_len = binary->__len__();
/*    if (bin_len > BASE64_MAXBIN) {
        throw new Error(0); //Too much data for base64 line
    } XXX */ 
    char * bin_data = &binary->unit[0];

    __ss_int leftbits = 0,leftchar = 0;
    char this_ch;

    bytes *ascii = new bytes("",bin_len*2 + 3);
    char * ascii_data = &ascii->unit[0];
    char * ascii_start = ascii_data;
    
    for( ; bin_len > 0 ; bin_len--, bin_data++ ) {
        /* Shift the data into our buffer */
        leftchar = (leftchar << 8) | *bin_data;
        leftbits += 8;

        /* See if there are 6-bit groups ready */
        while ( leftbits >= 6 ) {
            this_ch = (leftchar >> (leftbits-6)) & 0x3f;
            leftbits -= 6;
            *ascii_data++ = table_b2a_base64[this_ch];
        }
    }
    if ( leftbits == 2 ) {
        *ascii_data++ = table_b2a_base64[(leftchar&3) << 4];
        *ascii_data++ = BASE64_PAD;
        *ascii_data++ = BASE64_PAD;
    } else if ( leftbits == 4 ) {
        *ascii_data++ = table_b2a_base64[(leftchar&0xf) << 2];
        *ascii_data++ = BASE64_PAD;
    }
    *ascii_data++ = '\n';       /* Append a courtesy newline */

    // resize to ascii_data - start
    ascii->unit.resize(ascii_data-ascii_start);
    return ascii;
}

bytes *a2b_qp(bytes *pdata, __ss_bool header) {
    // from python 2.7.1
    __ss_int datalen = pdata->__len__();
    char * data = &pdata->unit[0];

    /* We allocate the output same size as input, this is overkill.
     * The previous implementation used calloc() so we'll zero out the
     * memory here too, since PyMem_Malloc() does not guarantee that.
     */
    bytes *outdata = new bytes("",datalen);
    char * odata = &outdata->unit[0];
    memset(odata,0,datalen);

    __ss_int in,out;
    char hexval,top,bot;
    in = out = 0;
    while (in < datalen) {
        if (data[in] == '=') {
            in++;
            if (in >= datalen) break;
            /* Soft line breaks */
            if ((data[in] == '\n') || (data[in] == '\r')) {
                if (data[in] != '\n') {
                    while (in < datalen && data[in] != '\n') in++;
                }
                if (in < datalen) in++;
            }
            else if (data[in] == '=') {
                /* broken case from broken python qp */
                odata[out++] = '=';
                in++;
            }
            // [fahhem] replacing hexval with table_a2b_hex
            else {
                top = table_a2b_hex[data[in]];
                bot = table_a2b_hex[data[in+1]];
                if (top==-1 || bot==-1)
                    odata[out++] = '=';
                else
                {
                    in+=2;
                    odata[out++] = (top<<4)|bot;
                }
            }
        }
        else if (header && data[in] == '_') {
            odata[out++] = ' ';
            in++;
        }
        else {
            odata[out] = data[in];
            in++;
            out++;
        }
    }
    outdata->unit.resize(out);
    return outdata;
}

#define MAXLINESIZE 76

static unsigned char table_b2a_hex[] = "0123456789ABCDEF";

void to_hex (unsigned char ch, unsigned char *s)
{
    unsigned int uvalue = ch;

    s[1] = table_b2a_hex[uvalue & 0x0F];
    uvalue >>= 4;
    s[0] = table_b2a_hex[uvalue & 0x0F];
}


/* XXX: This is ridiculously complicated to be backward compatible
 * (mostly) with the quopri module.  It doesn't re-create the quopri
 * module bug where text ending in CRLF has the CR encoded */
bytes *b2a_qp(bytes *pdata, __ss_bool quotetabs, __ss_bool istext, __ss_bool header) {
    __ss_int datalen = pdata->__len__();
    char * data = &pdata->unit[0];
    char * p = (char *) memchr(data, '\n', datalen);
    int crlf = 0;

    __ss_int in, out, odatalen=0;
    int linelen=0;
    char ch;
    

    /* See if this string is using CRLF line ends */
    /* XXX: this function has the side effect of converting all of
     * the end of lines to be the same depending on this detection
     * here */
    if ((p != NULL) && (p > data) && (*(p-1) == '\r'))
        crlf = 1;

    /* First, scan to see how many characters need to be encoded */
    in = 0;
    while (in < datalen) {
        if ((data[in] > 126) ||
            (data[in] == '=') ||
            (header && data[in] == '_') ||
            ((data[in] == '.') && (linelen == 0) &&
             (data[in+1] == '\n' || data[in+1] == '\r' || data[in+1] == 0)) ||
            (!istext && ((data[in] == '\r') || (data[in] == '\n'))) ||
            ((data[in] == '\t' || data[in] == ' ') && (in + 1 == datalen)) ||
            ((data[in] < 33) &&
             (data[in] != '\r') && (data[in] != '\n') &&
             (quotetabs ||
            (!quotetabs && ((data[in] != '\t') && (data[in] != ' '))))))
        {
            if ((linelen + 3) >= MAXLINESIZE) {
                linelen = 0;
                if (crlf)
                    odatalen += 3;
                else
                    odatalen += 2;
            }
            linelen += 3;
            odatalen += 3;
            in++;
        }
        else {
            if (istext &&
                ((data[in] == '\n') ||
                 ((in+1 < datalen) && (data[in] == '\r') &&
                 (data[in+1] == '\n'))))
            {
                linelen = 0;
                /* Protect against whitespace on end of line */
                if (in && ((data[in-1] == ' ') || (data[in-1] == '\t')))
                    odatalen += 2;
                if (crlf)
                    odatalen += 2;
                else
                    odatalen += 1;
                if (data[in] == '\r')
                    in += 2;
                else
                    in++;
            }
            else {
                if ((in + 1 != datalen) &&
                    (data[in+1] != '\n') &&
                    (linelen + 1) >= MAXLINESIZE) {
                    linelen = 0;
                    if (crlf)
                        odatalen += 3;
                    else
                        odatalen += 2;
                }
                linelen++;
                odatalen++;
                in++;
            }
        }
    }

    /* We allocate the output same size as input, this is overkill.
     * The previous implementation used calloc() so we'll zero out the
     * memory here too, since PyMem_Malloc() does not guarantee that.
     */
    bytes *outdata = new bytes("",odatalen);
    unsigned char * odata = (unsigned char *)&outdata->unit[0];
    memset(odata, 0, odatalen);

    in = out = linelen = 0;
    while (in < datalen) {
        if ((data[in] > 126) ||
            (data[in] == '=') ||
            (header && data[in] == '_') ||
            ((data[in] == '.') && (linelen == 0) &&
             (data[in+1] == '\n' || data[in+1] == '\r' || data[in+1] == 0)) ||
            (!istext && ((data[in] == '\r') || (data[in] == '\n'))) ||
            ((data[in] == '\t' || data[in] == ' ') && (in + 1 == datalen)) ||
            ((data[in] < 33) &&
             (data[in] != '\r') && (data[in] != '\n') &&
             (quotetabs ||
            (!quotetabs && ((data[in] != '\t') && (data[in] != ' '))))))
        {
            if ((linelen + 3 )>= MAXLINESIZE) {
                odata[out++] = '=';
                if (crlf) odata[out++] = '\r';
                odata[out++] = '\n';
                linelen = 0;
            }
            odata[out++] = '=';
            to_hex(data[in], &odata[out]);
            out += 2;
            in++;
            linelen += 3;
        }
        else {
            if (istext &&
                ((data[in] == '\n') ||
                 ((in+1 < datalen) && (data[in] == '\r') &&
                 (data[in+1] == '\n'))))
            {
                linelen = 0;
                /* Protect against whitespace on end of line */
                if (out && ((odata[out-1] == ' ') || (odata[out-1] == '\t'))) {
                    ch = odata[out-1];
                    odata[out-1] = '=';
                    to_hex(ch, &odata[out]);
                    out += 2;
                }

                if (crlf) odata[out++] = '\r';
                odata[out++] = '\n';
                if (data[in] == '\r')
                    in += 2;
                else
                    in++;
            }
            else {
                if ((in + 1 != datalen) &&
                    (data[in+1] != '\n') &&
                    (linelen + 1) >= MAXLINESIZE) {
                    odata[out++] = '=';
                    if (crlf) odata[out++] = '\r';
                    odata[out++] = '\n';
                    linelen = 0;
                }
                linelen++;
                if (header && data[in] == ' ') {
                    odata[out++] = '_';
                    in++;
                }
                else {
                    odata[out++] = data[in++];
                }
            }
        }
    }
    outdata->unit.resize(out);
    return outdata;
}


/*
** hqx lookup table, ascii->binary.
*/

#define DONE 0x7F
#define SKIP 0x7E
#define FAIL 0x7D

static unsigned char table_a2b_hqx[256] = {
/*       ^@    ^A    ^B    ^C    ^D    ^E    ^F    ^G   */
/* 0*/  FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
/*       \b    \t    \n    ^K    ^L    \r    ^N    ^O   */
/* 1*/  FAIL, FAIL, SKIP, FAIL, FAIL, SKIP, FAIL, FAIL,
/*       ^P    ^Q    ^R    ^S    ^T    ^U    ^V    ^W   */
/* 2*/  FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
/*       ^X    ^Y    ^Z    ^[    ^\    ^]    ^^    ^_   */
/* 3*/  FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
/*              !     "     #     $     %     &     '   */
/* 4*/  FAIL, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
/*        (     )     *     +     ,     -     .     /   */
/* 5*/  0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, FAIL, FAIL,
/*        0     1     2     3     4     5     6     7   */
/* 6*/  0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, FAIL,
/*        8     9     :     ;     <     =     >     ?   */
/* 7*/  0x14, 0x15, DONE, FAIL, FAIL, FAIL, FAIL, FAIL,
/*        @     A     B     C     D     E     F     G   */
/* 8*/  0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
/*        H     I     J     K     L     M     N     O   */
/* 9*/  0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, FAIL,
/*        P     Q     R     S     T     U     V     W   */
/*10*/  0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, FAIL,
/*        X     Y     Z     [     \     ]     ^     _   */
/*11*/  0x2C, 0x2D, 0x2E, 0x2F, FAIL, FAIL, FAIL, FAIL,
/*        `     a     b     c     d     e     f     g   */
/*12*/  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, FAIL,
/*        h     i     j     k     l     m     n     o   */
/*13*/  0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, FAIL, FAIL,
/*        p     q     r     s     t     u     v     w   */
/*14*/  0x3D, 0x3E, 0x3F, FAIL, FAIL, FAIL, FAIL, FAIL,
/*        x     y     z     {     |     }     ~    ^?   */
/*15*/  FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
/*16*/  FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
    FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
};


tuple2<bytes *, __ss_int> *a2b_hqx(bytes *pascii) {
    __ss_int len = pascii->__len__();
    if (len > PY_SSIZE_T_MAX - 2){
        throw new Error(0); //No memory
    }
    unsigned char * ascii_data = (unsigned char *)&pascii->unit[0];
    unsigned char * ascii_start = ascii_data;
    int done = 0, leftbits=0, leftchar=0;
    unsigned char this_ch;

    /* Allocate a string that is too big (fixed later)
       Add two to the initial length to prevent interning which
       would preclude subsequent resizing.  */
    bytes *outdata = new bytes("",len+2);
    unsigned char * bin_data = (unsigned char *)&outdata->unit[0];
    unsigned char * bin_start = bin_data;


    for( ; len > 0 ; len--, ascii_data++ ) {
        /* Get the byte and look it up */
        this_ch = table_a2b_hqx[*ascii_data];
        if ( this_ch == SKIP )
            continue;
        if ( this_ch == FAIL )
            throw new Error(0); //Illegal char

        if ( this_ch == DONE ) {
            /* The terminating colon */
            done = 1;
            break;
        }

        /* Shift it into the buffer and see if any bytes are ready */
        leftchar = (leftchar << 6) | (this_ch);
        leftbits += 6;
        if ( leftbits >= 8 ) {
            leftbits -= 8;
            *(bin_data++) = (leftchar >> leftbits) & 0xff;
            leftchar &= ((1 << leftbits) - 1);
        }
    }

    if ( leftbits && !done )
        throw new Error(0); //(Incomplete) String has incomplete number of bytes 

    outdata->unit.resize(bin_data-bin_start);
    return new tuple2<bytes *, __ss_int>(2,outdata,done);
}

static unsigned char table_b2a_hqx[] =
"!\"#$%&'()*+,-012345689@ABCDEFGHIJKLMNPQRSTUVXYZ[`abcdefhijklmpqr";

bytes *b2a_hqx(bytes *binary) {
    __ss_int bin_len = binary->__len__();
    if (bin_len > PY_SSIZE_T_MAX / 2 - 2) {
        throw new Error(0); // out of memory
    }

    char * bin_data = &binary->unit[0];

    bytes *ascii = new bytes("",(bin_len<<1)+2);
    char * ascii_data = &ascii->unit[0];
    char * ascii_start = ascii_data;

    int leftbits = 0, leftchar = 0;
    char this_ch;

    for( ; bin_len > 0 ; bin_len--, bin_data++ ) {
        /* Shift into our buffer, and output any 6bits ready */
        leftchar = (leftchar << 8) | *bin_data;
        leftbits += 8;
        while ( leftbits >= 6 ) {
            this_ch = (leftchar >> (leftbits-6)) & 0x3f;
            leftbits -= 6;
            *ascii_data++ = table_b2a_hqx[this_ch];
        }
    }
    /* Output a possible runt byte */
    if ( leftbits ) {
        leftchar <<= (6-leftbits);
        *ascii_data++ = table_b2a_hqx[leftchar & 0x3f];
    }

    ascii->unit.resize(ascii_data-ascii_start);
    return ascii;
}

#define RUNCHAR 0x90

bytes *rledecode_hqx(bytes *in_data_str) {
    __ss_int in_len = in_data_str->__len__();
    if ( in_len == 0 ) {
        return new bytes("",0);
    } else if (in_len > PY_SSIZE_T_MAX / 2) {
        throw new Error(0); // no memory
    }

    char * in_data = &in_data_str->unit[0];
    char in_byte, in_repeat;

    /* Allocate a buffer of reasonable size. Resized when needed */
    __ss_int out_len = in_len<<1;
    bytes *out_data_str = new bytes("",out_len);
    __ss_int out_len_left = out_len;
    char * out_data = &out_data_str->unit[0];
    char * out_data_start = out_data;

    /*
    ** We need two macros here to get/put bytes and handle
    ** end-of-buffer for input and output strings.
    */
#define INBYTE(b) \
    do { \
             if ( --in_len < 0 ) { \
                    throw new Error(0);  /* Incomplete */ \
             } \
             b = *in_data++; \
    } while(0)

#define OUTBYTE(b) \
    do { \
             if ( --out_len_left < 0 ) { \
                      if ( out_len > PY_SSIZE_T_MAX / 2) throw new Error(0); /*No Memory*/ \
                      out_data_str->unit.resize(out_len<<1);\
                      out_data = out_data_start + out_len; \
                      out_len_left = out_len-1; \
                      out_len *= 2; \
             } \
             *out_data++ = b; \
    } while(0)

    /*
    ** Handle first byte separately (since we have to get angry
    ** in case of an orphaned RLE code).
    */
    INBYTE(in_byte);

    if (in_byte == RUNCHAR) {
        INBYTE(in_repeat);
        if (in_repeat != 0) {
            /* Note Error, not Incomplete (which is at the end
            ** of the string only). This is a programmer error.
            */
            throw new Error(0); // "Orphaned RLE code at start"
        }
        OUTBYTE(RUNCHAR);
    } else {
        OUTBYTE(in_byte);
    }

    while( in_len > 0 ) {
        INBYTE(in_byte);

        if (in_byte == RUNCHAR) {
            INBYTE(in_repeat);
            if ( in_repeat == 0 ) {
                /* Just an escaped RUNCHAR value */
                OUTBYTE(RUNCHAR);
            } else {
                /* Pick up value and output a sequence of it */
                in_byte = out_data[-1];
                while ( --in_repeat > 0 )
                    OUTBYTE(in_byte);
            }
        } else {
            /* Normal byte */
            OUTBYTE(in_byte);
        }
    }
    out_data_str->unit.resize(out_data-out_data_start);
    return out_data_str;
}

bytes *rlecode_hqx(bytes *data) {
    __ss_int len = data->__len__();
    if (len >  PY_SSIZE_T_MAX / 2 - 2) {
        throw new Error(0); // no memory
    }

    char * in_data = &data->unit[0];
    bytes *rv = new bytes("", len*2+2);
    char * out_data = &rv->unit[0];
    char * out_start = out_data;
    char ch;
    __ss_int in, inend; 

    for( in=0; in<len; in++) {
        ch = in_data[in];
        if ( ch == RUNCHAR ) {
            /* RUNCHAR. Escape it. */
            *out_data++ = RUNCHAR;
            *out_data++ = 0;
        } else {
            /* Check how many following are the same */
            for(inend=in+1;
                inend<len && in_data[inend] == ch &&
                    inend < in+255;
                inend++) ;
            if ( inend - in > 3 ) {
                /* More than 3 in a row. Output RLE. */
                *out_data++ = ch;
                *out_data++ = RUNCHAR;
                *out_data++ = inend-in;
                in = inend-1;
            } else {
                /* Less than 3. Output the byte itself */
                *out_data++ = ch;
            }
        }
    }
    rv->unit.resize(out_data-out_start);
    return rv;
}

static unsigned short crctab_hqx[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

__ss_int crc_hqx(bytes *data, __ss_int crc) {
    __ss_int len = data->__len__();
    char * bin_data = &data->unit[0];
    while (len-- > 0) {
        crc=((crc<<8)&0xff00)^crctab_hqx[((crc>>8)&0xff)^*bin_data++];
    }
    return crc;
}

/*  Crc - 32 BIT ANSI X3.66 CRC checksum files
    Also known as: ISO 3307
**********************************************************************|
*                                                                    *|
* Demonstration program to compute the 32-bit CRC used as the frame  *|
* check sequence in ADCCP (ANSI X3.66, also known as FIPS PUB 71     *|
* and FED-STD-1003, the U.S. versions of CCITT's X.25 link-level     *|
* protocol).  The 32-bit FCS was added via the Federal Register,     *|
* 1 June 1982, p.23798.  I presume but don't know for certain that   *|
* this polynomial is or will be included in CCITT V.41, which        *|
* defines the 16-bit CRC (often called CRC-CCITT) polynomial.  FIPS  *|
* PUB 78 says that the 32-bit FCS reduces otherwise undetected       *|
* errors by a factor of 10^-5 over 16-bit FCS.                       *|
*                                                                    *|
**********************************************************************|

 Copyright (C) 1986 Gary S. Brown.  You may use this program, or
 code or tables extracted from it, as desired without restriction.

 First, the polynomial itself and its table of feedback terms.  The
 polynomial is
 X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0
 Note that we take it "backwards" and put the highest-order term in
 the lowest-order bit.  The X^32 term is "implied"; the LSB is the
 X^31 term, etc.  The X^0 term (usually shown as "+1") results in
 the MSB being 1.

 Note that the usual hardware shift register implementation, which
 is what we're using (we're merely optimizing it by doing eight-bit
 chunks at a time) shifts bits into the lowest-order term.  In our
 implementation, that means shifting towards the right.  Why do we
 do it this way?  Because the calculated CRC must be transmitted in
 order from highest-order term to lowest-order term.  UARTs transmit
 characters in order from LSB to MSB.  By storing the CRC this way,
 we hand it to the UART in the order low-byte to high-byte; the UART
 sends each low-bit to hight-bit; and the result is transmission bit
 by bit from highest- to lowest-order term without requiring any bit
 shuffling on our part.  Reception works similarly.

 The feedback terms table consists of 256, 32-bit entries.  Notes:

  1. The table can be generated at runtime if desired; code to do so
     is shown later.  It might not be obvious, but the feedback
     terms simply represent the results of eight shift/xor opera-
     tions for all combinations of data and CRC register values.

  2. The CRC accumulation logic is the same for all CRC polynomials,
     be they sixteen or thirty-two bits wide.  You simply choose the
     appropriate table.  Alternatively, because the table can be
     generated at runtime, you can start by generating the table for
     the polynomial in question and use exactly the same "updcrc",
     if your application needn't simultaneously handle two CRC
     polynomials.  (Note, however, that XMODEM is strange.)

  3. For 16-bit CRCs, the table entries need be only 16 bits wide;
     of course, 32-bit entries work OK if the high 16 bits are zero.

  4. The values must be right-shifted by eight bits by the "updcrc"
     logic; the shift must be unsigned (bring in zeroes).  On some
     hardware you could probably optimize the shift in assembler by
     using byte-swap instructions.
********************************************************************/

static unsigned int crc_32_tab[256] = {
0x00000000U, 0x77073096U, 0xee0e612cU, 0x990951baU, 0x076dc419U,
0x706af48fU, 0xe963a535U, 0x9e6495a3U, 0x0edb8832U, 0x79dcb8a4U,
0xe0d5e91eU, 0x97d2d988U, 0x09b64c2bU, 0x7eb17cbdU, 0xe7b82d07U,
0x90bf1d91U, 0x1db71064U, 0x6ab020f2U, 0xf3b97148U, 0x84be41deU,
0x1adad47dU, 0x6ddde4ebU, 0xf4d4b551U, 0x83d385c7U, 0x136c9856U,
0x646ba8c0U, 0xfd62f97aU, 0x8a65c9ecU, 0x14015c4fU, 0x63066cd9U,
0xfa0f3d63U, 0x8d080df5U, 0x3b6e20c8U, 0x4c69105eU, 0xd56041e4U,
0xa2677172U, 0x3c03e4d1U, 0x4b04d447U, 0xd20d85fdU, 0xa50ab56bU,
0x35b5a8faU, 0x42b2986cU, 0xdbbbc9d6U, 0xacbcf940U, 0x32d86ce3U,
0x45df5c75U, 0xdcd60dcfU, 0xabd13d59U, 0x26d930acU, 0x51de003aU,
0xc8d75180U, 0xbfd06116U, 0x21b4f4b5U, 0x56b3c423U, 0xcfba9599U,
0xb8bda50fU, 0x2802b89eU, 0x5f058808U, 0xc60cd9b2U, 0xb10be924U,
0x2f6f7c87U, 0x58684c11U, 0xc1611dabU, 0xb6662d3dU, 0x76dc4190U,
0x01db7106U, 0x98d220bcU, 0xefd5102aU, 0x71b18589U, 0x06b6b51fU,
0x9fbfe4a5U, 0xe8b8d433U, 0x7807c9a2U, 0x0f00f934U, 0x9609a88eU,
0xe10e9818U, 0x7f6a0dbbU, 0x086d3d2dU, 0x91646c97U, 0xe6635c01U,
0x6b6b51f4U, 0x1c6c6162U, 0x856530d8U, 0xf262004eU, 0x6c0695edU,
0x1b01a57bU, 0x8208f4c1U, 0xf50fc457U, 0x65b0d9c6U, 0x12b7e950U,
0x8bbeb8eaU, 0xfcb9887cU, 0x62dd1ddfU, 0x15da2d49U, 0x8cd37cf3U,
0xfbd44c65U, 0x4db26158U, 0x3ab551ceU, 0xa3bc0074U, 0xd4bb30e2U,
0x4adfa541U, 0x3dd895d7U, 0xa4d1c46dU, 0xd3d6f4fbU, 0x4369e96aU,
0x346ed9fcU, 0xad678846U, 0xda60b8d0U, 0x44042d73U, 0x33031de5U,
0xaa0a4c5fU, 0xdd0d7cc9U, 0x5005713cU, 0x270241aaU, 0xbe0b1010U,
0xc90c2086U, 0x5768b525U, 0x206f85b3U, 0xb966d409U, 0xce61e49fU,
0x5edef90eU, 0x29d9c998U, 0xb0d09822U, 0xc7d7a8b4U, 0x59b33d17U,
0x2eb40d81U, 0xb7bd5c3bU, 0xc0ba6cadU, 0xedb88320U, 0x9abfb3b6U,
0x03b6e20cU, 0x74b1d29aU, 0xead54739U, 0x9dd277afU, 0x04db2615U,
0x73dc1683U, 0xe3630b12U, 0x94643b84U, 0x0d6d6a3eU, 0x7a6a5aa8U,
0xe40ecf0bU, 0x9309ff9dU, 0x0a00ae27U, 0x7d079eb1U, 0xf00f9344U,
0x8708a3d2U, 0x1e01f268U, 0x6906c2feU, 0xf762575dU, 0x806567cbU,
0x196c3671U, 0x6e6b06e7U, 0xfed41b76U, 0x89d32be0U, 0x10da7a5aU,
0x67dd4accU, 0xf9b9df6fU, 0x8ebeeff9U, 0x17b7be43U, 0x60b08ed5U,
0xd6d6a3e8U, 0xa1d1937eU, 0x38d8c2c4U, 0x4fdff252U, 0xd1bb67f1U,
0xa6bc5767U, 0x3fb506ddU, 0x48b2364bU, 0xd80d2bdaU, 0xaf0a1b4cU,
0x36034af6U, 0x41047a60U, 0xdf60efc3U, 0xa867df55U, 0x316e8eefU,
0x4669be79U, 0xcb61b38cU, 0xbc66831aU, 0x256fd2a0U, 0x5268e236U,
0xcc0c7795U, 0xbb0b4703U, 0x220216b9U, 0x5505262fU, 0xc5ba3bbeU,
0xb2bd0b28U, 0x2bb45a92U, 0x5cb36a04U, 0xc2d7ffa7U, 0xb5d0cf31U,
0x2cd99e8bU, 0x5bdeae1dU, 0x9b64c2b0U, 0xec63f226U, 0x756aa39cU,
0x026d930aU, 0x9c0906a9U, 0xeb0e363fU, 0x72076785U, 0x05005713U,
0x95bf4a82U, 0xe2b87a14U, 0x7bb12baeU, 0x0cb61b38U, 0x92d28e9bU,
0xe5d5be0dU, 0x7cdcefb7U, 0x0bdbdf21U, 0x86d3d2d4U, 0xf1d4e242U,
0x68ddb3f8U, 0x1fda836eU, 0x81be16cdU, 0xf6b9265bU, 0x6fb077e1U,
0x18b74777U, 0x88085ae6U, 0xff0f6a70U, 0x66063bcaU, 0x11010b5cU,
0x8f659effU, 0xf862ae69U, 0x616bffd3U, 0x166ccf45U, 0xa00ae278U,
0xd70dd2eeU, 0x4e048354U, 0x3903b3c2U, 0xa7672661U, 0xd06016f7U,
0x4969474dU, 0x3e6e77dbU, 0xaed16a4aU, 0xd9d65adcU, 0x40df0b66U,
0x37d83bf0U, 0xa9bcae53U, 0xdebb9ec5U, 0x47b2cf7fU, 0x30b5ffe9U,
0xbdbdf21cU, 0xcabac28aU, 0x53b39330U, 0x24b4a3a6U, 0xbad03605U,
0xcdd70693U, 0x54de5729U, 0x23d967bfU, 0xb3667a2eU, 0xc4614ab8U,
0x5d681b02U, 0x2a6f2b94U, 0xb40bbe37U, 0xc30c8ea1U, 0x5a05df1bU,
0x2d02ef8dU
};

__ss_int crc32(bytes *data, __ss_int signed_crc) {
    __ss_int len = data->__len__();
    unsigned int crc = signed_crc;
    crc = ~crc;
    char * bin_data = &data->unit[0];
    while (len-- > 0) {
        crc = crc_32_tab[(crc ^ *bin_data++) & 0xffU] ^ (crc >> 8);
    }
    return (__ss_int)(crc ^ 0xFFFFFFFFU);
}

bytes *b2a_hex(bytes *data) {
    
    return hexlify(data);
}

bytes *a2b_hex(bytes *data) {
    
    return unhexlify(data);
}

void __init() {
    __name__ = new str("binascii");

    cl_Error = new class_("Error");
    cl_Incomplete = new class_("Incomplete");
    default_0 = False;
    default_1 = False;
    default_2 = False;
    default_3 = False;

}

} // module namespace
