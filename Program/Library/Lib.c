#include "Lib.h"
char * itoa(int32_t n)
{
	int32_t sign;
	int8_t i,k;
	char s[16];
	static uint8_t s1[16];
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
	static uint8_t s1[17];
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


