/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Muu.cpp
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

#include "Muu.h"

using namespace kauppa;

// Konstruktori parametrilla
Muu::Muu( const tuoteTyyppi tyyppi, const double x ) :
Tuote( tyyppi )
{
	mHinta = x;
}

// Kopiomuodostin
Muu::Muu(const Muu& vanha) :
Tuote( vanha )
{
	mHinta = vanha.mHinta;
}

// Sijoitusoperaattori
Muu& Muu::operator=( const Muu& vanha )
{
	// Itseen sijoituksen tarkistus
	if( this == &vanha )
		return *this;

	Tuote::operator=( vanha );

	mHinta = vanha.mHinta;

	return *this;
}