/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Kori.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Ostoskori
//
//  Kommentit:	Kori-luokka tarjoaa taulukon valittujen tuotteiden silöntään.
//				Luokka sisältää myös korin sisällön tulostavan metodin, sekä
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

		// Korin sisällön tulostava funktio
		void naytaKori(const int tulostus);

		// Sopivan pussin hakeva funktio, palauttaa osoittimen ko. pussiin
		Pussi* haePussi( pussiLaji laji ) const;

		// Palauttaa korissa olevien tuottedien määrän
		const int tuotteita() const;

		// Lisää tuotteen koriin
		bool tKoriin( Tuote* lisattavaTuote, const int kPaikka );

		// Palauttaa korissa paikassa x olevan tuotteen osoittimen
		Tuote* kerroTuote( const int x ) const;

	private:

		// Kopiomuodostin, käyttö estetty
		Kori( const Kori& vanha );

		// Sijoitusoperaattori
		Kori& operator=( const Kori& vanha );

		// Korin tallennustila
		Tuote* sisalto[30];
	};
}

#endif // __KORI_H__