/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Riisi.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Riisi
//
//  Kommentit:	-
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __RIISI_H__
#define __RIISI_H__

#include "Muu.h"

namespace kauppa
{
	class Riisi : public Muu
	{
	public:
		
		// Konstruktori parametrein
		Riisi( const tuoteTyyppi tyyppi, const double x );

		// Kopiomuodostin
		Riisi( const Riisi& vanha);

		// DestruktoRR
		virtual ~Riisi() { };

		// Palauttaa tyypin
		virtual tuoteTyyppi getTyyppi() const;

		// Asettaa hinnan
		virtual void setHinta( const double hinta );

		// Palauttaa hinnan
		virtual double getHinta() const;

		// Sijoitusoperaattori
		Riisi& operator=( const Riisi& vanha);

	private:
	};
}

#endif // __RIISI_H__