/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Kassa.h
//   Projekti:	Kaupassak�yntisimulaatio
//     Versio:	1.0
//     Tekij�:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Kassa-luokka, korin sis�ll�n laskeminen
//
//  Kommentit:	K�ytet��n korin sis�lt�mien tuotteiden hinnan laskemiseen,
//				sek� punnitsemattomien pussien tarkistukseen ja punnituksen
//				kutsumiseen
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __KASSA_H__
#define __KASSA_H__

#include "Kori.h"

#include <iostream>

namespace kauppa
{
	class Kori;

	class Kassa
	{
	public:

		// Konstruktori
		Kassa();

		// Destruktori
		~Kassa();

		// Korin sis�ll�n laskeva funktio
		void laskeJaTulosta( Kori& klaskeKori );

		// Tarkistaa onko korissa punnitsemattomia pusseja
		bool tarkastaPussit( Kori& tarkKori );

	private:

		// Kopiomuodostaja
		Kassa( const Kori& vanha );

		// Sijoitusoperaattori
		Kassa& operator=( const Kori& vanha);
	};
}

#endif