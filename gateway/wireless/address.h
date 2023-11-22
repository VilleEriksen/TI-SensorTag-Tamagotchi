//DESCRIPTION/NOTES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Name:			address.h
//		Description:	Set student device address
// 		Author(s):		Teemu Leppanen (UBIComp) 
//		Last modified:	2017.09.17
//		Note: 			The commenting style is optimized for automatic documentation generation using DOXYGEN: www.doxygen.org/
//		License:		Refer to Licence.txt file
//						partially based on CC2650 code of Contiki
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef JTKJ_EXAMPLE_WIRELESS_ADDRESS_H_
#define JTKJ_EXAMPLE_WIRELESS_ADDRESS_H_

#define IEEE80154_PANID        	0x1337
#define IEEE80154_CHANNEL      	0x0C
#define IEEE80154_SERVER_ADDR  	0x1234
#define IEEE80154_MY_ADDR      	0x1234 // ADD YOUR OWN ADDRESS HERE
// Reception address interval, end points included:
#define IEEE80154_RECEIVE_START	0x0000
#define IEEE80154_RECEIVE_END  	0xffff

#endif
