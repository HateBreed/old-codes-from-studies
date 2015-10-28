/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Riisi.cpp
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

#include "Riisi.h"
#include "Kauppaliittyma.h"
#include "Kori.h"

using namespace kauppa;

// Konstruktori parametrein
Riisi::Riisi( const tuoteTyyppi tyyppi, const double x ) :
Muu( tyyppi, x )
{
}

// Kopiomuodostin
Riisi::Riisi(const Riisi& vanha) :
Muu( vanha )
{
}

// Palauttaa tuotteen tyypin
tuoteTyyppi Riisi::getTyyppi() const
{
	return riisi;
}

// Hinnan asettaminen tuotteelle
void Riisi::setHinta( const double hinta )
{
	mHinta = hinta;
}

// Palauttaa tuotteen hinnan
double Riisi::getHinta() const
{
	return mHinta;
}

// Sijoitusoperaattori
Riisi& Riisi::operator=( const Riisi& vanha )
{
	if( this == &vanha )
		return *this;

	Muu::operator=( vanha );

	return *this;
}
