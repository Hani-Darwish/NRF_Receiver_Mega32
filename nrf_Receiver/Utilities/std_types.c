#include <stdint.h>
#include <stdbool.h>
#include "std_types.h"


void EF_ArrayCopy ( U8_t * Dest_Address, U8_t* Src_Address, U16_t CopiedBytesLength )
{
    U16_t index = 0;
    for (index = 0; index < CopiedBytesLength ; index++ )
    {
        Dest_Address[index] = Src_Address[index];
    }

}




/*********************************************************************
* Function    :  itoa_Convert (U16_t Number , U8_t * NumberASCII_ptr)
*
* DESCRIPTION : This function used to convert integer number to ASCII
*
* PARAMETERS  : U16_t Number : integer Number
*               U8_t * NumberASCII_ptr : pointer to the ASCII number after Conversion
*               NumberOFDigits_ptr: pointer to size NumberASCII_ptr array
*
* Return Value: None.
**********************************************************************/
 void itoa_Convert (U32_t Number , U8_t * NumberASCII_ptr ,U8_t* NumberOFDigits_ptr)
{
	U8_t  ArrayIndex = 0;
	U8_t NumberOfDigits = 0;
	U32_t Number_temp = Number;
	if ( Number == 0 )
	{
		NumberASCII_ptr[ArrayIndex] = '0';
		return;
	}
	/* get Number of digits of the wanted  number to be converted */
	while ( Number_temp > 0 )
	{
		Number_temp = Number_temp/10;
		NumberOfDigits++;
	}
	*NumberOFDigits_ptr = NumberOfDigits;

	for ( ArrayIndex =0 ; ArrayIndex < NumberOfDigits ; ArrayIndex++)
	{
		NumberASCII_ptr[ArrayIndex] = Number %10 +'0';
		Number = Number/10;
	}
	/* rotate , using Number as a temp */
	for ( ArrayIndex =0 ; ArrayIndex < NumberOfDigits/2 ; ArrayIndex++)
	{
		Number     = NumberASCII_ptr[ArrayIndex];
		NumberASCII_ptr[ArrayIndex]   = NumberASCII_ptr[NumberOfDigits-ArrayIndex-1];
		NumberASCII_ptr[NumberOfDigits-ArrayIndex-1] = Number;
	}
	NumberASCII_ptr [NumberOfDigits] = 0;
}
