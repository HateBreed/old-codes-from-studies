/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Hedelma.h
//   Projekti:	Kaupassak‰yntisimulaatio
//     Versio:	1.0
//     Tekij‰:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Hedelma-luokka
//
//  Kommentit:	Luokasta periytytet‰‰n Banaani, Mandariini ja P‰‰ryn‰
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#ifndef __HEDELMA_H__
#define __HEDELMA_H__

#include "Tuote.h"

namespace kauppa
{

	class Hedelma : public Tuote
	{
	public:

		// Parametrillinen konstruktori
		Hedelma( const tuoteTyyppi tyyppi, const double x );

		// Kopiomuodostaja
		Hedelma( const Hedelma& vanha);

		// Destructor
		virtual ~Hedelma() { }

		// Palauttaa tuotteen tyypin
		virtual tuoteTyyppi getTyyppi() const = 0;

		// Asettaa tuotteelle hinnan
		virtual void setPaino( const double paino ) = 0;
		
		// Palauttaa tuotteen hinnan
		virtual double getPaino() const = 0;
		
	protected:

		// Sijoitusoperaattori
		Hedelma& operator=(const Hedelma& vanha);

		double hPaino;

	private:

		
	};
}

#endif // __HEDELMA_H__