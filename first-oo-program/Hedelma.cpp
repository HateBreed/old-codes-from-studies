/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Hedelma.cpp
//   Projekti:	Kaupassak‰yntisimulaatio
//     Versio:	1.0
//     Tekij‰:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Hedelma-luokka
//
//  Kommentit:	Luokasta periytytet‰‰n Banaani, Mandariini ja P‰‰ryn‰
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#include "Hedelma.h"

using namespace kauppa;

// Konstruktori parametrilla
Hedelma::Hedelma( const tuoteTyyppi tyyppi, const double x ) :
Tuote( tyyppi )
{
	hPaino = x;
}

// Kopiomuodostin
Hedelma::Hedelma(const Hedelma& vanha) :
Tuote( vanha )
{
	hPaino = vanha.hPaino;
}

// Sijoitusoperaattori
Hedelma& Hedelma::operator=( const Hedelma& vanha )
{
	// Itseen sijoituksen tarkistus
	if( this == &vanha )
		return *this;

	Tuote::operator=( vanha );

	hPaino = vanha.hPaino;

	return *this;
}