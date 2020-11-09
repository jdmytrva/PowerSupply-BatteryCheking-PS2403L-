#ifndef _USART_
#define _USART_
#include "General.h"
#define DEBUG_LEVEL 1

#define ENABLE_PREFIX 1

#define ENABLE_LOG_INFO 1
#define ENABLE_LOG_ERROR 1
#define ENABLE_LOG_DEBUG 1


#if ENABLE_LOG_INFO
	#if ENABLE_PREFIX
		#define logInfo(x)  {  printToBufferWithoutEndUART1("INFO: ");  printToBufferUART1(x);}
		#define logInfoD(x,y,z)  {  printToBufferWithoutEndUART1("INFO: ");  printToBufferUART1D(x,y,z);}
	#else
		#define logInfo(x)  {printToBufferUART1(x);}
		#define logInfoD(x,y,z)  {printToBufferUART1D(x,y,z);}
	#endif
#else
#define logInfo(x)  // nothing
#define logInfoD(x,y,z) // nothing
#endif


#if ENABLE_LOG_ERROR
	#if ENABLE_PREFIX
		#define logError(x)  {  printToBufferWithoutEndUART1("ERROR: ");  printToBufferUART1(x);}
		#define logErrorD(x,y,z)  {  printToBufferWithoutEndUART1("ERROR: ");  printToBufferUART1D(x,y,z);}
	#else
		#define logError(x)  {printToBufferUART1(x);}
		#define logErrorD(x,y,z)  {printToBufferUART1D(x,y,z);}
	#endif
#else
#define logError(x)  // nothing
#define logErrorD(x,y,z) // nothing
#endif

#if ENABLE_LOG_DEBUG
	#if ENABLE_PREFIX
		#define logDebug(x)  {  printToBufferWithoutEndUART1("DEBUG: ");  printToBufferUART1(x);}
		#define logDebugD(x,y,z)  {  printToBufferWithoutEndUART1("DEBUG: ");  printToBufferUART1D(x,y,z);}
	#else
		#define logDebug(x)  {printToBufferUART1(x);}
		#define logDebugD(x,y,z)  {printToBufferUART1D(x,y,z);}
	#endif
#else
#define logDebug(x)  // nothing
#define logDebugD(x,y,z) // nothing
#endif


void Print_to_USART1(volatile char *data);
void Print_to_USART1_d(volatile int32_t value,volatile char *string,uint8_t koma);
void putCharInBufferUart1(uint8_t c);
void putCharDirectToUart1(uint8_t c);
void putStringInBufferUart1(volatile char *s);
void putStringDirectToUart1(volatile char *s);
void printToBufferUART1(volatile char* str);
void printToBufferWithoutEndUART1(volatile char* str);
void printToBufferUART1D(volatile char* str, volatile int32_t value, volatile uint8_t koma);

#endif
