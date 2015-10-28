/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Banaani.cpp
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

#include "Banaani.h"
#include "Kauppaliittyma.h"
#include "Kori.h"
#include "Pussi.h"

using namespace kauppa;

// Konstruktori parametrein
Banaani::Banaani( const tuoteTyyppi tyyppi, const double x ) :
Hedelma( tyyppi, x )		
{
}

// Kopiomuodostin
Banaani::Banaani(const Banaani& vanha) :
Hedelma( vanha )
{
}

// Palauttaa tuotteen tyypin
tuoteTyyppi Banaani::getTyyppi() const
{
	return banaani;
}

// Hinnan asettaminen tuotteelle
void Banaani::setPaino( const double paino )
{
	hPaino = paino;
}

// Palauttaa tuotteen painon
double Banaani::getPaino() const
{
	return hPaino;
}

// Sijoitusoperaattori
Banaani& Banaani::operator=( const Banaani& vanha )
{
	if( this == &vanha )
		return *this;

	Hedelma::operator=( vanha );

	return *this;
}

