#ifndef DATA_H_
#define DATA_H_

#define len 15

#include <stdint.h>
#include <stdbool.h>


enum // data türünün belirlenecegi enum yapısı
{

	_shortint,  	//short int
	_ushortint,  	//unsigned short int
	_uint,			//unsigned int
	_int,			//int
	_lint,			//long int
	_ulint,			//unsigned long int
	_llint,			//long long int
	_ullint,		//unsigned long int
	_char,			//char
	_uchar,			//unsigned char
	_float,			//float
	_double,		//double
	_ldouble,		//long double

}DATATYPES;

typedef union
{
	struct
	{
		uint8_t 		start;			// komut baslangıc adresi
		uint16_t 	commandNo;		// komut numarası
		uint8_t 	slaveNo;		// komutun gonderildigi birimin numarası
		uint32_t	commandAddr;	// komut adresi
		uint8_t		commandType;	// komut türü  -- 0 : read -- 1: write
		uint8_t 	dataType;		// datanın türü
		uint32_t    data;			// komutun isleyecegi data
		uint8_t 	end;            // komıt sonu
	}PARS;
	uint8_t COMMAND[sizeof(PARS)];
}SLAVE;

extern SLAVE slave;


#endif /* DATA_H_ */