/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Kauppaliittyma.h
//   Projekti:	Kaupassak�yntisimulaatio
//     Versio:	1.0
//     Tekij�:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	K�ytt�liittym� kaupan palveluihin
//
//  Kommentit:	Sis�lt�� toiminnallisuuden tuotteiden luomiseksi hyllyyn ja 
//				niiden omistajuuden siirtoon koriin, sek� pussiin. Korin ja
//				pussin luontimetodit, sek� hyllyss� olevien tuotteiden n�ytt�
//				on my�s sis�llytetty t�h�n luokkaan.
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

		// Tuotteen siirt�minen hyllyst� koriin
		bool koriin( Tuote* lisattava, Kori& kKori );

		// Tuotteen (hedelm�n) siirt�minen hyllyst� pussiin
		bool pussiin( Tuote* hedelma, const Kori& pKori );

		// Tulostaa kaupan tuotteet ruudulle
		void naytaTuotteet();

		// Punnitsemattomien pussien punnitseminen
		void valitsePunnittavat( Kori& vpKori );

		// P��valikko
		void kaynnistaJaSuorita();


		
	private:

		// Kopiomuodostin
		Kauppaliittyma( const Kauppaliittyma& vanha );

		// Sijoitusoperaattori
		Kauppaliittyma& operator=( const Kauppaliittyma& vanha );

		// Tuotteiden lis�ysvalikko
		void tuoteKoriin( Kori& tKori );

		// Tallennustila
		Tuote* hylly[60];

	};
}

#endif // __KAUPPALIITTYMA_H__