/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Banaani.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Banaani
//
//  Kommentit:	-
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __BANAANI_H__
#define __BANAANI_H__

#include "Hedelma.h"

namespace kauppa
{
	class Banaani : public Hedelma
	{
	public:
		
		// Konstruktori parametrein
		Banaani( const tuoteTyyppi tyyppi, const double x );

		// Kopiomuodostin
		Banaani( const Banaani& vanha);

		// DestruktoRR
		virtual ~Banaani() { };

		// Palauttaa tyypin
		virtual tuoteTyyppi getTyyppi() const;

		// Asettaa painon
		virtual void setPaino( const double paino );

		// Palauttaa painon
		virtual double getPaino() const;

		// Sijoitusoperaattori
		Banaani& operator=( const Banaani& vanha);

	private:
	};
}

#endif // __BANAANI_H__