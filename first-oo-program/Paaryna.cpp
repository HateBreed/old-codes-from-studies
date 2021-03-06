/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Paaryna.cpp
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

#include "Paaryna.h"
#include "Kauppaliittyma.h"
#include "Kori.h"
#include "Pussi.h"

using namespace kauppa;

// Konstruktori parametrein
Paaryna::Paaryna( const tuoteTyyppi tyyppi, const double x ) :
Hedelma( tyyppi, x )
{
}

// Kopiomuodostin
Paaryna::Paaryna(const Paaryna& vanha) :
Hedelma( vanha )
{
}

// Palauttaa tuotteen tyypin
tuoteTyyppi Paaryna::getTyyppi() const
{
	return paaryna;
}

// Hinnan asettaminen tuotteelle
void Paaryna::setPaino( const double paino )
{
	hPaino = paino;
}

// Palauttaa tuotteen painon
double Paaryna::getPaino() const
{
	return hPaino;
}

// Sijoitusoperaattori
Paaryna& Paaryna::operator=( const Paaryna& vanha )
{
	if( this == &vanha )
		return *this;

	Hedelma::operator=( vanha );

	return *this;
}

