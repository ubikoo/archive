
/* ISO 8859/1 Latin-1 alphabetic and upper and lower case bit vector tables. */

/* LINTLIBRARY */

unsigned char isoalpha[32] = {
    0,0,0,0,0,0,0,0,127,255,255,224,127,255,255,224,0,0,0,0,0,0,0,0,255,255,
    254,255,255,255,254,255
};

unsigned char isoupper[32] = {
    0,0,0,0,0,0,0,0,127,255,255,224,0,0,0,0,0,0,0,0,0,0,0,0,255,255,254,254,
    0,0,0,0
};

unsigned char isolower[32] = {
    0,0,0,0,0,0,0,0,0,0,0,0,127,255,255,224,0,0,0,0,0,0,0,0,0,0,0,1,255,255,
    254,255
};

#ifndef ISASCII
/* true if character is ascii. */
_Bool isascii(int c)
{
	return c >= 0 && c < 128;
}
#endif
