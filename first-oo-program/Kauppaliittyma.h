/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Kauppaliittyma.h
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Käyttöliittymä kaupan palveluihin
//
//  Kommentit:	Sisältää toiminnallisuuden tuotteiden luomiseksi hyllyyn ja 
//				niiden omistajuuden siirtoon koriin, sekä pussiin. Korin ja
//				pussin luontimetodit, sekä hyllyssä olevien tuotteiden näyttö
//				on myös sisällytetty tähän luokkaan.
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __KAUPPALIITTYMA_H__
#define __KAUPPALIITTYMA_H__

#include <string>
#include <iostream>

namespace kauppa
{
	class Kassa;
	class Kori;
	class Tuote;
	class Muu;

	class Kauppaliittyma
	{
	public:

		// Konstruktori
		Kauppaliittyma();

		// Dekonstruktor
		virtual ~Kauppaliittyma();

		// Korin ottaminen (luontikutsu)
		Kori* otaKori();

		// Pussin ottaminen
		bool otaPussi( Kori& opKori );

		// Luo tuotteet hyllyyn
		void aloita();

		// Tuotteen siirtäminen hyllystä koriin
		bool koriin( Tuote* lisattava, Kori& kKori );

		// Tuotteen (hedelmän) siirtäminen hyllystä pussiin
		bool pussiin( Tuote* hedelma, const Kori& pKori );

		// Tulostaa kaupan tuotteet ruudulle
		void naytaTuotteet();

		// Punnitsemattomien pussien punnitseminen
		void valitsePunnittavat( Kori& vpKori );

		// Päävalikko
		void kaynnistaJaSuorita();


		
	private:

		// Kopiomuodostin
		Kauppaliittyma( const Kauppaliittyma& vanha );

		// Sijoitusoperaattori
		Kauppaliittyma& operator=( const Kauppaliittyma& vanha );

		// Tuotteiden lisäysvalikko
		void tuoteKoriin( Kori& tKori );

		// Tallennustila
		Tuote* hylly[60];

	};
}

#endif // __KAUPPALIITTYMA_H__