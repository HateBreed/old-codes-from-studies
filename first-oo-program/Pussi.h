/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Pussi.h
//   Projekti:	Kaupassak�yntisimulaatio
//     Versio:	1.0
//     Tekij�:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Pussi hedelmien s�ilytykseen
//
//  Kommentit:	Pussiin voidaan lis�t� valitut hedelm�t, pussissa voi olla 
//				vain yhdenlaisia hedelmi�. Tarjoaa metodit painon, hinnan ja
//				lajin palautukseen ja asettamiseen.
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __PUSSI_H__
#define __PUSSI_H__

#include "Tuote.h"

namespace kauppa
{
	class Hedelma;
	class Kauppaliittyma;

	enum pussiLaji
	{
		pTyhja = 0,
		pBanaani,
		pMandariini,
		pPaaryna
	};

	class Pussi : public Tuote
	{
		
	public:

		// Ei parametrillista rakentajaa
		Pussi( const tuoteTyyppi tyyppi, const pussiLaji uusi );

		// Destructor
		virtual ~Pussi();

		// Kopiorakentaja
		Pussi(const Pussi& vanha);

		// Sijoitusoperaattori
		Pussi& operator=( const Pussi& vanha);

		// Tyypin palauttava
		tuoteTyyppi getTyyppi() const;

		// Pussin sis�ll�n lajin asetus
		void setLaji( const pussiLaji laji );

		// Painon asetus pussille, eli punnitseminen
		void setPainop();

		// Pussin sis�ll�n lajin palautus
		pussiLaji getLaji() const;

		// Painon palautus
		double getPainop() const;

		// Sis�ll�n hinta
		double sisaltoHinta();

		// Palauttaa pussissa olevien hedelmien m��r�n
		const int pussissa() const;

		// Hedelm� haluttuun paikkaan
		void tPussiin( Tuote* pHedelma, const int pPaik );

	private:

		pussiLaji pLaji;
		double pPaino;
		double pHinta;

		Hedelma* sisalto[10];
	};
}

#endif // __PUSSI_H__