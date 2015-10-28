/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Pussi.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Pussi hedelmien säilytykseen
//
//  Kommentit:	Pussiin voidaan lisätä valitut hedelmät, pussissa voi olla 
//				vain yhdenlaisia hedelmiä. Tarjoaa metodit painon, hinnan ja
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

		// Pussin sisällön lajin asetus
		void setLaji( const pussiLaji laji );

		// Painon asetus pussille, eli punnitseminen
		void setPainop();

		// Pussin sisällön lajin palautus
		pussiLaji getLaji() const;

		// Painon palautus
		double getPainop() const;

		// Sisällön hinta
		double sisaltoHinta();

		// Palauttaa pussissa olevien hedelmien määrän
		const int pussissa() const;

		// Hedelmä haluttuun paikkaan
		void tPussiin( Tuote* pHedelma, const int pPaik );

	private:

		pussiLaji pLaji;
		double pPaino;
		double pHinta;

		Hedelma* sisalto[10];
	};
}

#endif // __PUSSI_H__