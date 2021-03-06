/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Muu.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Muiden tuotteiden isäluokka
//
//  Kommentit:	Luokasta periytytetään Maito, Jugurtti ja Riisi.
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __MUU_H__
#define __MUU_H__

#include "Tuote.h"

namespace kauppa
{
	class Muu : public Tuote
	{
	public:

		// Parametrillinen konstruktori
		Muu( const tuoteTyyppi tyyppi, const double x);

		// Kopiomuodostaja
		Muu( const Muu& vanha);

		// Destructor
		virtual ~Muu() { }

		// Palauttaa tuotteen tyypin
		virtual tuoteTyyppi getTyyppi() const = 0;

		// Asettaa tuotteelle hinnan
		virtual void setHinta( const double hinta ) = 0;
		
		// Palauttaa tuotteen hinnan
		virtual double getHinta() const = 0;
		
	protected:

		// Sijoitusoperaattori
		Muu& operator=(const Muu& vanha);

		double mHinta;

	private:

		
	};
}

#endif // __MUU_H__
