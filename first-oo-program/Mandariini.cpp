/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Mandariini.cpp
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

#include "Mandariini.h"
#include "Kauppaliittyma.h"
#include "Kori.h"
#include "Pussi.h"

using namespace kauppa;

// Konstruktori parametrein
Mandariini::Mandariini( const tuoteTyyppi tyyppi, const double x ) :
Hedelma( tyyppi, x )
{
}

// Kopiomuodostin
Mandariini::Mandariini(const Mandariini& vanha) :
Hedelma( vanha )
{
}

// Palauttaa tuotteen tyypin
tuoteTyyppi Mandariini::getTyyppi() const
{
	return mandariini;
}

// Hinnan asettaminen tuotteelle
void Mandariini::setPaino( const double paino )
{
	hPaino = paino;
}

// Palauttaa tuotteen painon
double Mandariini::getPaino() const
{
	return hPaino;
}

// Sijoitusoperaattori
Mandariini& Mandariini::operator=( const Mandariini& vanha )
{
	if( this == &vanha )
		return *this;

	Hedelma::operator=( vanha );

	return *this;
}

