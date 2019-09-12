#include "Lib.h"
char * itoa(int32_t n)
{
	int32_t sign;
	int8_t i,k;
	char s[16];
	static char s1[16];
     if ((sign = n) < 0)
         n = -n;
     i = 0;
     do {
         s[i++] = n % 10 + '0';
     } while ((n /= 10) > 0);
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';

     k=0;
     while (i)
     {
    	s1[k]=s[i-1];
    	i--;
    	k++;
     }
     s1[k] = '\0';
     return s1;
}

char * itoaP(int32_t n,char *s1)
{
	int32_t sign;
	int8_t i,k;
	char s[16];

     if ((sign = n) < 0)
         n = -n;
     i = 0;
     do {
         s[i++] = n % 10 + '0';
     } while ((n /= 10) > 0);
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';

     k=0;
     while (i)
     {
    	s1[k]=s[i-1];
    	i--;
    	k++;
     }
     s1[k] = '\0';
     return s1;
}

char * itoa_koma(int32_t n,uint8_t koma)
{
	int32_t sign;
	int8_t i,k,j;
	char s[17];
	char sKoma[17];
	static char s1[17];
    if ((sign = n) < 0)
    	n = -n;
     i = 0;
     do
     {
         s[i++] = n % 10 + '0';
     } while ((n /= 10) > 0);
     s[i] = '\0';
     if (i<=1)
     {
       	 s[i] = '0';
       	 i++;
     }
     if (i<=2&&koma>1)
     {
    	 s[i] = '0';
    	 i++;
     }
     k=0;
     j=0;
     while (j<i)
     {
    	 if (k==koma)
         {
         	 sKoma[k]='.';
         	 k++;
         }
    	 sKoma[k]=s[j];
         j++;
         k++;
      }
     if (sign < 0) sKoma[k++] = '-';
     i=0;
     while (k)
     {
    	s1[i]=sKoma[k-1];
    	k--;
    	i++;
     }
     s1[i] = '\0';
     return s1;
}

char * itoa_komaP(int32_t n,char *s1, uint8_t koma)
{
	int32_t sign;
	int8_t i,k,j;
	char s[17];
	char sKoma[17];
    if ((sign = n) < 0)
    	n = -n;
     i = 0;
     do
     {
         s[i++] = n % 10 + '0';
     } while ((n /= 10) > 0);
     s[i] = '\0';
     if (i<=1)
     {
       	 s[i] = '0';
       	 i++;
     }
     if (i<=2&&koma>1)
     {
    	 s[i] = '0';
    	 i++;
     }
     k=0;
     j=0;
     while (j<i)
     {
    	 if (k==koma)
         {
         	 sKoma[k]='.';
         	 k++;
         }
    	 sKoma[k]=s[j];
         j++;
         k++;
      }
     if (sign < 0) sKoma[k++] = '-';
     i=0;
     while (k)
     {
    	s1[i]=sKoma[k-1];
    	k--;
    	i++;
     }
     s1[i] = '\0';
     return s1;
}
int16_t Module16 (int16_t value)
{
	if (value == 0) return 0;
	if (value > 0) return value;
	if (value < 0) return (-1)*value;
}

int8_t Module8 (int8_t value)
{
	if (value == 0) return 0;
	if (value > 0) return value;
	if (value < 0) return (-1)*value;
}
int32_t Module32 (int32_t value)
{
	if (value == 0) return 0;
	if (value > 0) return value;
	if (value < 0) return (-1)*value;
}

uint8_t strlen1(char string[])
{
	uint8_t i=0;
	while(string[i]!='\0')
		i++;
	return i;
}

char * Merge2Strings(char * str1in, char * str2in, char * strout)
{
	int8_t i, j;
	for (i=0; str1in[i]!='\0'; i++)
	{
		strout[i] = str1in[i];
	}
	for (j=0; str2in[j]!='\0'; j++,i++)
	{
		strout[i] = str2in[j];
	}
	strout[i] = '\0';
	return strout;
}
char * Merge3Strings(char * str1in, char * str2in,char * str3in, char * strout)
{
	int8_t i, j,k;
	for (i=0; str1in[i]!='\0'; i++)
	{
		strout[i] = str1in[i];
	}
	for (j=0; str2in[j]!='\0'; j++,i++)
	{
		strout[i] = str2in[j];
	}
	for (k=0; str3in[k]!='\0'; k++,i++)
	{
		strout[i] = str3in[k];
	}
	strout[i] = '\0';
	return strout;
}

char *ClockStringWithSec(uint32_t time, char *OutStr)
{
	int8_t i, j,k;
	char s[17];
	itoaP(time/3600,s);
	i=0;
	if (strlen1(s)<2)
	{
		OutStr[i] = '0';
		i++;
	}

	for (k=0; s[k]!='\0';k++, i++)
	{
		OutStr[i] = s[k];
	}

	OutStr[i] = ':';
	i++;
	itoaP((time/60)%60,s);
	if (strlen1(s)<2)
	{
		OutStr[i] = '0';
		i++;
	}
	for (j=0; s[j]!='\0'; i++,j++)
	{
		OutStr[i] = s[j];
	}

	OutStr[i] = ':';
	i++;

	itoaP(time%60,s);
	if (strlen1(s)<2)
	{
		OutStr[i] = '0';
		i++;
	}
	for (j=0; s[j]!='\0'; i++,j++)
	{
		OutStr[i] = s[j];
	}

	OutStr[i] = '\0';
	return OutStr;
}

char *ClockStringNoSec(uint32_t time, char *OutStr)
{

	int8_t i, j,k;
	char s[17];
	itoaP(time/3600,s);
	i=0;
	if (strlen1(s)<2)
	{
		OutStr[i] = '0';
		i++;
	}

	for (k=0; s[k]!='\0';k++, i++)
	{
		OutStr[i] = s[k];
	}

	OutStr[i] = ':';
	i++;
	itoaP((time/60)%60,s);
	if (strlen1(s)<2)
	{
		OutStr[i] = '0';
		i++;
	}
	for (j=0; s[j]!='\0'; i++,j++)
	{
		OutStr[i] = s[j];
	}


	OutStr[i] = '\0';
	return OutStr;
}
