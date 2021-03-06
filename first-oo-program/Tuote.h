/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Tuote.h
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

#ifndef __TUOTE_H__
#define __TUOTE_H__

namespace kauppa
{
	class Kori;

	enum tuoteTyyppi
	{
		pussi = 0,
		banaani,
		jugurtti,
		maito,
		mandariini,
		paaryna,
		riisi
	};

	class Tuote
	{
	public:
		// Parametrillinen konstruktori
		Tuote( const tuoteTyyppi tyyppi);

		// Kopiomuodostaja
		Tuote( const Tuote& vanha);

		// Destructor
		virtual ~Tuote() { }

		// Palauttaa tuotteen tyypin
		virtual tuoteTyyppi getTyyppi() const = 0;

	protected:

		// Sijoitusoperaattori
		Tuote& operator=(const Tuote& vanha);

	private:

		tuoteTyyppi tTyyppi;
	};
}

#endif // __TUOTE_H__