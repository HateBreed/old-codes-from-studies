/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Kori.h
//   Projekti:	Kaupassak�yntisimulaatio
//     Versio:	1.0
//     Tekij�:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Ostoskori
//
//  Kommentit:	Kori-luokka tarjoaa taulukon valittujen tuotteiden sil�nt��n.
//				Luokka sis�lt�� my�s korin sis�ll�n tulostavan metodin, sek�
//				oikean tyyppisen pussin hakemisen korista.
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __KORI_H__
#define __KORI_H__

#include "Pussi.h"

namespace kauppa
{
	class Tuote;
	class Kauppaliittyma;

	class Kori
	{
	public:

		// Konstruktori
		Kori();

		// Destruktori
		virtual ~Kori();

		// Korin sis�ll�n tulostava funktio
		void naytaKori(const int tulostus);

		// Sopivan pussin hakeva funktio, palauttaa osoittimen ko. pussiin
		Pussi* haePussi( pussiLaji laji ) const;

		// Palauttaa korissa olevien tuottedien m��r�n
		const int tuotteita() const;

		// Lis�� tuotteen koriin
		bool tKoriin( Tuote* lisattavaTuote, const int kPaikka );

		// Palauttaa korissa paikassa x olevan tuotteen osoittimen
		Tuote* kerroTuote( const int x ) const;

	private:

		// Kopiomuodostin, k�ytt� estetty
		Kori( const Kori& vanha );

		// Sijoitusoperaattori
		Kori& operator=( const Kori& vanha );

		// Korin tallennustila
		Tuote* sisalto[30];
	};
}

#endif // __KORI_H__