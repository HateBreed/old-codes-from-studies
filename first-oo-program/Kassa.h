/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Kassa.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Kassa-luokka, korin sisällön laskeminen
//
//  Kommentit:	Käytetään korin sisältämien tuotteiden hinnan laskemiseen,
//				sekä punnitsemattomien pussien tarkistukseen ja punnituksen
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

		// Korin sisällön laskeva funktio
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