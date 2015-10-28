/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Maito.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Maito
//
//  Kommentit:	-
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __MAITO_H__
#define __MAITO_H__

#include "Muu.h"

namespace kauppa
{
	class Maito : public Muu
	{
	public:
		
		// Konstruktori parametrein
		Maito( const tuoteTyyppi tyyppi, const double x );

		// Kopiomuodostin
		Maito( const Maito& vanha);

		// DestruktoRR
		virtual ~Maito() { };

		// Palauttaa tyypin
		virtual tuoteTyyppi getTyyppi() const;

		// Asettaa hinnan
		virtual void setHinta( const double hinta );

		// Palauttaa hinnan
		virtual double getHinta() const;

		// Sijoitusoperaattori
		Maito& operator=( const Maito& vanha);

	private:
	};
}

#endif // __MAITO_H__