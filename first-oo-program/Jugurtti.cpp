/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Jugurtti.cpp
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

#include "Jugurtti.h"
#include "Kauppaliittyma.h"
#include "Kori.h"

using namespace kauppa;

// Konstruktori parametrein
Jugurtti::Jugurtti( const tuoteTyyppi tyyppi, const double x ) :
Muu( tyyppi, x )
{
}

// Kopiomuodostin
Jugurtti::Jugurtti(const Jugurtti& vanha) :
Muu( vanha )
{
}

// Palauttaa tuotteen tyypin
tuoteTyyppi Jugurtti::getTyyppi() const
{
	return jugurtti;
}

// Hinnan asettaminen tuotteelle
void Jugurtti::setHinta( const double hinta )
{
	mHinta = hinta;
}

// Palauttaa tuotteen hinnan
double Jugurtti::getHinta() const
{
	return mHinta;
}

// Sijoitusoperaattori
Jugurtti& Jugurtti::operator=( const Jugurtti& vanha )
{
	if( this == &vanha )
		return *this;

	Muu::operator=( vanha );

	return *this;
}

