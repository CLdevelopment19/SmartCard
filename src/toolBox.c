/*
 * toolBox.c
 *
 *  Created on: 15 janv. 2019
 *      Author: clatour
 */

#include "toolBox.h"
//*****************************************************************************************************************************************//

/* Define */
//#define DISPLAY_SIGNE_PLUS
//*****************************************************************************************************************************************//

void rmv_lastCarStr(char *str, uint8_t size)
{
	str[size-1] = '\0';
}
//*****************************************************************************************************************************************//

void strcopy(void *dest, const void *src, uint8_t ofs_dest, uint8_t ofs_src, uint8_t n_towrite)
{
	char *d = dest;
	const char *s = src;

    for(uint8_t i=0;i<n_towrite;i++)
    {
        d[i+ofs_dest]=s[i+ofs_src];
    }
    //str1[p1 + nb] = '\0';
}
//*****************************************************************************************************************************************//

uint8_t IntToStr(uint8_t *str, int32_t n, e_bool eof)
{
	int32_t tmp 	= n;
	uint8_t size 	= 0;
	uint8_t ofs 	= 0;
    int 	i 		= 0;
    e_bool 	n_neg 	= FALSE;

    if(n < 0)
    {
    	n_neg = TRUE;
        n = -n;
    }

    do
    {
    	tmp/=10;
    	size++;
    }while(tmp!=0);

    if(eof)
        str[size+1] = '\0';

    if(n_neg){
		str[0] = '-';
		ofs = 1;
    }
#ifdef DISPLAY_SIGNE_PLUS
	else{
		str[0] = '+';
		ofs = 1;
	}
#endif


    for (i = size+ofs; i > ofs; i--)
    {
        str[i-1] = n%10 + '0';
        n/=10;
    }
    return (size+ofs);
}
//*****************************************************************************************************************************************//

uint16_t StrToInt(uint8_t* str, uint8_t l)
{
	uint16_t 	tmp 	= 0;
	uint16_t	n 		= 0;
	uint8_t 	inc 	= 1;

	for (int i = l-1; i >= 0; i--)
	{
		tmp = str[i] - '0';
		tmp *= inc;
		n += tmp;
		inc *= 10;
	}
	return n;
}
//*****************************************************************************************************************************************//

uint8_t DoubleToStr (uint8_t *str, double n, uint8_t precision)
{
	uint32_t 	tmp 				= 0;
	e_bool 		n_neg 				= FALSE;
	uint8_t 	size_before_comma 	= 0;
	uint8_t 	size_after_comma 	= 0;
	int32_t 	val_before_comma	= 0;
	uint32_t 	val_after_comma		= 0;
	double 		dmp					= 0.0;
	uint8_t 	str_dmp[5] 			= {0};
	uint16_t	precision_mult		= 10;

	if(n == 0)
	{
		memcpy(str, "0.00", 4);
		return 4;
	}else
	{
		if(n < 0)
		{
			n_neg = TRUE;
			n = -n;
		}

		tmp = (uint32_t)n;

		val_before_comma = (int32_t)n;

		if (n_neg){
			val_before_comma = -val_before_comma;
			size_before_comma = IntToStr(str, val_before_comma, FALSE);
		}else
			size_before_comma = IntToStr(str, val_before_comma, FALSE);

		str[size_before_comma++] = '.';

		for(int i = 0; i < precision; i++)
			precision_mult *= 10;
		dmp = (double)(n*precision_mult) - (tmp*precision_mult);

		if(dmp != 0){
			dmp /= 10.0;
			val_after_comma = (uint32_t)dmp;
			size_after_comma = IntToStr(str_dmp, val_after_comma, FALSE);
			if((val_after_comma<10) && (precision != 1))
			{
				char c = str_dmp[0];
				str_dmp[0] = '0';
				str_dmp[1] = c;
				size_after_comma = 2;
			}
			strcopy(str, str_dmp, size_before_comma, 0, size_after_comma);
		}else
		{
			strcopy(str, "00", size_before_comma, 0, 2);
			size_after_comma = 2;
		}

		return (size_before_comma + size_after_comma);
	}
}
//*****************************************************************************************************************************************//

uint8_t decToBCD(uint8_t valInDec)
{
    // Example : 15 in decimal is egal to 00010101 in BCD
    // 15 --> 1 = 0001 and 5 = 0101
    uint8_t tmp = 0;
    tmp = valInDec % 10;                // get the smalest byte
    tmp |= (valInDec/10)<<4;
    return tmp;
}
//*****************************************************************************************************************************************//

uint8_t BCDToDec(uint8_t valInBCD)
{
	uint8_t out = 0;
	uint8_t MSB = 0;
	uint8_t LSB = 0;
    MSB = (valInBCD & 0b11110000) >> 4;
    LSB = valInBCD & 0b00001111;
    out = (MSB*10) + LSB;
    return out;
}
//*****************************************************************************************************************************************//

uint8_t split_str(char* dest, const char* src, char delim)
{
	uint8_t lgt 	= 0;
	char 	c 		= 0;
	uint8_t n 		= 0;

	if(src!= NULL)
	{
		for(int i = 0; i < strlen(src); i++)
		{
			c = src[i];

			if(c == delim)
			{
				break;
			}
			n++;
		}

		if(n!=0)
		{
			do
			{
				*dest++ = *src++;
				lgt++;
			}while(n--);
		}

	}
	return lgt;
}
//*****************************************************************************************************************************************//

double	 StrToDouble	(uint8_t* str)
{
	double 		res  		= 0.0;
	double 		tmp  		= 0.0;
	uint32_t 	val 		= 0;
	uint8_t 	lenght 		= 0;
	uint8_t 	n			= 0;
	uint16_t 	puissance 	= 1;
	uint8_t 	str_tmp[16] = {0};

	if(str[strlen((const char*)str)-1] == '\t')
		rmv_lastCarStr((char*)str, strlen((const char*)str));

	lenght = split_str((char*)str_tmp, (const char*)str, '.');
	val = StrToInt(str_tmp, lenght-1);

	res = (double)val;

	for(int i = 0; i < 2; i++)		// Keep max two digit after comma
	{
		if(str[i + lenght] != '\0'){
			str_tmp[i] = str[i + lenght];
			n++;
		}else
			break;
	}
	val = StrToInt(str_tmp, n);

	while(n--)
		puissance *= 10;

	tmp = (double)val/puissance;
	res += tmp;
	val = res*100;
	res = (double)val/100;

	return res;
}
//*****************************************************************************************************************************************//

uint16_t ABS(uint16_t a, uint16_t b)
{
	uint16_t res = 0;
	if(a >= b)
		res = a - b;
	else
		res = b - a;
	return res;
}
//*****************************************************************************************************************************************//

double MAX(double a, double b)
{
	if(a>b) return a;
	else return b;
}
//*****************************************************************************************************************************************//

