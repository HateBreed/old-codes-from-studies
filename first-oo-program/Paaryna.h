/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Paaryna.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Päärynä
//
//  Kommentit:	-
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __PAARYNA_H__
#define __PAARYNA_H__

#include "Hedelma.h"

namespace kauppa
{
	class Paaryna : public Hedelma
	{
	public:
		
		// Konstruktori parametrein
		Paaryna( const tuoteTyyppi tyyppi, const double x );

		// Kopiomuodostin
		Paaryna( const Paaryna& vanha);

		// DestruktoRR
		virtual ~Paaryna() { };

		// Palauttaa tyypin
		virtual tuoteTyyppi getTyyppi() const;

		// Asettaa painon
		virtual void setPaino( const double paino );

		// Palauttaa painon
		virtual double getPaino() const;

		// Sijoitusoperaattori
		Paaryna& operator=( const Paaryna& vanha );

	private:
	};
}

#endif // __PAARYNA_H__