/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Maito.cpp
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

#include "Maito.h"
#include "Kauppaliittyma.h"
#include "Kori.h"

using namespace kauppa;

// Konstruktori parametrein
Maito::Maito( const tuoteTyyppi tyyppi, const double x ) :
Muu( tyyppi, x )
{
}

// Kopiomuodostin
Maito::Maito(const Maito& vanha) :
Muu( vanha )
{
}

// Palauttaa tuotteen tyypin
tuoteTyyppi Maito::getTyyppi() const
{
	return maito;
}

// Hinnan asettaminen tuotteelle
void Maito::setHinta( const double hinta )
{
	mHinta = hinta;
}

// Palauttaa tuotteen hinnan
double Maito::getHinta() const
{
	return mHinta;
}

// Sijoitusoperaattori
Maito& Maito::operator=( const Maito& vanha )
{
	if( this == &vanha )
		return *this;

	Muu::operator=( vanha );

	return *this;
}
