/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Mandariini.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Mandariini
//
//  Kommentit:	-
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __MANDARIINI_H__
#define __MANDARIINI_H__

#include "Hedelma.h"

namespace kauppa
{
	class Mandariini : public Hedelma
	{
	public:
		
		// Konstruktori parametrein
		Mandariini( const tuoteTyyppi tyyppi, const double x );


		// Kopiomuodostin
		Mandariini( const Mandariini& vanha);

		// DestruktoRR
		virtual ~Mandariini() { };

		// Palauttaa tyypin
		virtual tuoteTyyppi getTyyppi() const;

		// Asettaa painon
		virtual void setPaino( const double paino );

		// Palauttaa painon
		virtual double getPaino() const;

		// Sijoitusoperaattori
		Mandariini& operator=( const Mandariini& vanha);

	private:
	};
}

#endif // __MANDARIINI_H__