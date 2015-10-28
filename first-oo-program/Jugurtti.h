/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Jugurtti.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Jugurtti
//
//  Kommentit:	-
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __JUGURTTI_H__
#define __JUGURTTI_H__

#include "Muu.h"

namespace kauppa
{
	class Jugurtti : public Muu
	{
	public:
		
		// Konstruktori parametrein
		Jugurtti( const tuoteTyyppi tyyppi, const double x );

		// Kopiomuodostin
		Jugurtti( const Jugurtti& vanha);

		// DestruktoRR
		virtual ~Jugurtti() { };

		// Palauttaa tyypin
		virtual tuoteTyyppi getTyyppi() const;

		// Asettaa hinnan
		virtual void setHinta( const double hinta );

		// Palauttaa hinnan
		virtual double getHinta() const;

		// Sijoitusoperaattori
		Jugurtti& operator=( const Jugurtti& vanha);

	private:
	};
}

#endif // __JUGURTTI_H__