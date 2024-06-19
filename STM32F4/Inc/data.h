#ifndef DATA_H_
#define DATA_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define typename(x) _Generic((x), \
			short int 			: 0, \
			unsigned short int 	: 1, \
			unsigned int		: 2, \
			int					: 3, \
			long int			: 4, \
			unsigned long int	: 5, \
			long long int		: 6, \
			char				: 8, \
			unsigned char		: 9, \
			float				: 10, \
			double				: 11, \
			long double			: 12)  // data turlerini tanımlamak ıcın kullanılan generıc yapısı

typedef enum { // Dondurulecek hata kodlarını tutan enum yapısı
    NO_ERROR,
    INVALID_START_CHAR,
    INVALID_END_CHAR,
    INVALID_COMMAND_NUM,
    INVALID_SLAVE_NO,
    INVALID_COMMAND_ADDR,
    INVALID_COMMAND_TYPE,
    INVALID_DATA_TYPE,
    INVALID_DATA_BLOCK
}ErrorCode;

typedef union
{
	struct
	{
		uint8_t     start;			// komut baslangıc adresi
		uint16_t 	commandNo;		// komut numarası
		uint8_t 	slaveNo;		// komutun gonderildigi birimin numarası
		uint32_t	commandAddr;	// komut adresi
		uint8_t		commandType;	// komut türü  -- 0 : read -- 1: write
		uint8_t 	dataType;		// datanın türü
		uint32_t    data;			// komutun isleyecegi data
		uint8_t 	end;            // komıt sonu

	}PARS;

	uint8_t COMMAND[24];

}SLAVE;

extern SLAVE slave;


ErrorCode parser();
int data_handle();

#endif

