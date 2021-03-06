/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Tuote.cpp
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Tuote-luokka
//
//  Kommentit:	Tästä luokasta periytytetään kaikki tuotteet
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#include "Tuote.h"
#include <iostream>

using namespace std;
using namespace kauppa;

// Konstruktori, joka saa parametrina luotavan tuotteen tyypin
Tuote::Tuote( const tuoteTyyppi tyyppi) :
tTyyppi( tyyppi )
{
}

// Kopiomuodostin
Tuote::Tuote( const Tuote& vanha ) :
tTyyppi( vanha.tTyyppi )
{
}

// Sijoitusoperaattori
Tuote& Tuote::operator=( const Tuote& vanha )
{
	// Itseen sijoituksen tarkistus
	if( this == &vanha )
		return *this;

	tTyyppi = vanha.tTyyppi;

	return *this;
}