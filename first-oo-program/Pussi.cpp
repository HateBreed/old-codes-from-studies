/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Pussi.cpp
//   Projekti:	Kaupassak‰yntisimulaatio
//     Versio:	1.0
//     Tekij‰:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Pussi hedelmien s‰ilytykseen
//
//  Kommentit:	Pussiin voidaan lis‰t‰ valitut hedelm‰t, pussissa voi olla 
//				vain yhdenlaisia hedelmi‰. Tarjoaa metodit painon, hinnan ja
//				lajin palautukseen ja asettamiseen.
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#include "Pussi.h"
#include "Kori.h"

#include "Banaani.h"
#include "Mandariini.h"
#include "Paaryna.h"

#include <cstddef>

using namespace kauppa;

// Konstruktori, ei parametreja
Pussi::Pussi( const tuoteTyyppi tyyppi, const pussiLaji uusi ) :
// Kutsutaan is‰luokan rakentajaa ja asetetaan pussin lajiksi tyhj‰
Tuote( tyyppi )
{
	pLaji = uusi;
	pPaino = 0;
	pHinta = 0;
	for( int i = 0; i < 10; i++ )
	{
		sisalto[i] = NULL;
	}
}


// Destruktori
Pussi::~Pussi()
{
	for( int i = 0; i < 10; i++ )
	{
		delete sisalto[i];
		sisalto[i] = NULL;
	}
}


// Kopiorakentaja
Pussi::Pussi( const Pussi& vanha ) :
Tuote( vanha ), pLaji( vanha.pLaji)
{
	pPaino = vanha.pPaino;
	pHinta = vanha.pPaino;

	for( int i = 0; i < 10; i++ )
	{
		sisalto[i] = vanha.sisalto[i];
	}
}


// Sijoitusoperaattori
Pussi& Pussi::operator=( const Pussi& vanha )
{
	if( this == &vanha )
		return *this;

	Tuote::operator=( vanha );

	pLaji = vanha.pLaji;
	pPaino = vanha.pPaino;
	pHinta = vanha.pPaino;

	for( int i = 0; i < 10; i++ )
	{
		sisalto[i] = vanha.sisalto[i];
	}

	return *this;
}


// Palauttaa tuotteen tyypin
tuoteTyyppi Pussi::getTyyppi() const
{
	return pussi;
}


// Asettaa pussille lajin, laji = pussissa olevien hedelmien laji
void Pussi::setLaji( const pussiLaji laji )
{
	pLaji = laji;
}


// PUNNITSEMISFUNKTIO - Asettaa pussille painon punnitsemalla sis‰llˆn
void Pussi::setPainop()
{
	double paino = 0, temp = 0;
	// pussin sis‰ltˆ l‰pi, jokaisen hedelm‰n paino haetaan getPaino()-funktiolla
	// ja lasketaan yhteen muiden kanssa
	for( int i = 0; i < 10; i++)
	{
		if( sisalto[i] )
		{
			paino += sisalto[i]->getPaino();

		}
	}
	pPaino = paino;
}


// Palauttaa pussin sis‰lt‰mien hedelmien lajin
pussiLaji Pussi::getLaji() const
{
	return pLaji;
}


// Palauttaa pussin painon
double Pussi::getPainop() const
{
	return pPaino;
}


// Palauttaa pussin sis‰llˆn hinnan
double Pussi::sisaltoHinta()
{
	double kerroin;

	// Kerroin (hinta per paino) m‰‰r‰ytyy lajin mukaan
	if(getLaji() == pMandariini)
		kerroin = 1.3;

	else if(getLaji() == pBanaani)
		kerroin = 1.69;
		
	else if(getLaji() == pPaaryna)
		kerroin = 2.1;

	// Jos pussi on tyhj‰, niin kertoimeksi asetetaan yksi
	// Hintana tulostuu kuitenkin 0, koska paino alustetaan nollaksi.
	else
		kerroin = 1;

	pHinta = kerroin * getPainop();

	return pHinta;
}


// Palauttaa pussissa olevien hedelmien m‰‰r‰n
const int Pussi::pussissa() const
{
	int hedelmat = 10;

	for(int i = 0; i < 10; i++ )
	{
		// Jos kyseinen paikka on tyhj‰ -> hedelmi‰ on 1 v‰hemm‰n
		if( sisalto[i] == NULL )
			hedelmat--;
	}

	return hedelmat;
}

// Lis‰‰ tuotteen pussiin
void Pussi::tPussiin(Tuote* pHedelma, const int pPaik )
{
	Hedelma* pH = static_cast<Hedelma*>(pHedelma);
	sisalto[pPaik] = pH;
}



