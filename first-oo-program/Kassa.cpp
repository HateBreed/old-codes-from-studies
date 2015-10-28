/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Kassa.cpp
//   Projekti:	Kaupassak�yntisimulaatio
//     Versio:	1.0
//     Tekij�:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Kassa-luokka, korin sis�ll�n laskeminen
//
//  Kommentit:	K�ytet��n korin sis�lt�mien tuotteiden hinnan laskemiseen,
//				sek� punnitsemattomien pussien tarkistukseen ja punnituksen
//				kutsumiseen
//
//////////////////////////////////////////////////////////////////////////////
//
//   Historia:
//    
////////////////////////////////////////////////////////////////////////////*/

#include "Kassa.h"

#include "Kori.h"

#include "Pussi.h"
#include "Banaani.h"
#include "Mandariini.h"
#include "Paaryna.h"
#include "Jugurtti.h"
#include "Maito.h"
#include "Riisi.h"

#include <iomanip>

using namespace kauppa;
using namespace std;

// Muodostaja
Kassa::Kassa()
{
}

// DestruktoRR
Kassa::~Kassa()
{
}

// Ostoskorin sis�ll�n hinnan laskeva funktio
// Saa parametrina osoittimen koriin
void Kassa::laskeJaTulosta( Kori& klaskeKori )
{
	double summa = 0;
	int tyhjat = 0, i = 0, kTulostus = 2;

	// Tulostetaan korin sis�lt� k�ytt�j�lle, tulostusmetodi 2 - kuitti
	klaskeKori.naytaKori( kTulostus );

	// K�yd��n korin sis�lt� l�pi, tuotteiden hinnat lis�t��n loppusummaan
	for( i = 0; i < 30; i++ )
	{
		Tuote* kKohta = klaskeKori.kerroTuote(i);
		if( kKohta )
		{
			// Jos kyseisess� kohdassa on pussi, kysyt��n pussin kokonaishinta
			if( (kKohta->getTyyppi() == pussi) )
			{
				Pussi* pkohta = static_cast<Pussi*>(kKohta);
				// Jos pussi on tyhj� (kaikki pussit punnitaan ennen korin
				// laskemista, joten vain aidosti tyhj�t j��v�t)
				if( pkohta->getLaji() == pTyhja )
				{
					tyhjat++;
				}

				else
				{
					summa = summa + pkohta->sisaltoHinta();
				}
			}
			
			// Muutoin summaan lis�t��n tuotteen hinta
			else
			{
				Muu* mkohta = static_cast<Muu*>(kKohta);
				summa = summa + mkohta->getHinta();
			}
		}
	}

	if( summa != 0 )
	{
		cout << "Tuotteiden kokonaishinta: " << setprecision(3) << summa << " euroa." << endl;
	}
	
	// Jos tyhji� pusseja l�ytyi, niin t�st� ilmoitetaan k�ytt�j�lle
	if( tyhjat != 0)
	{
		cout << "Tyhjia pusseja " << tyhjat;
		if( tyhjat == 1 )
		{
			cout << " kappale." << endl;
		}
		else
		{
			cout << " kappaletta." << endl;
		}
	}

}

// Pussit tarkastava funktio, suoritetaan ennen korin sis�ll�n laskemista
// Palauttaa boolean arvolla true, jos punnitsemattomia pusseja ei l�ydy
bool Kassa::tarkastaPussit( Kori& tarkKori )
{
	bool tarkastus = true;

	// K�yd��n korin tuotteet l�pi
	for( int i = 0; i < 30; i++ )
	{
		Tuote* pK = tarkKori.kerroTuote(i);

		// Jos l�ydet��n korista pussi, niin tarkistetaan onko se punnittu
		// Jos pussia ei ole punnittu, se punnitaan setPaino()-funtkiota
		// k�ytt�m�ll�
		if( ( pK ) && ( pK->getTyyppi() == pussi) )
		{
			Pussi* ppK = static_cast<Pussi*>(pK);

			if( ppK->getPainop() == 0)
			{
				ppK->setPainop();
				tarkastus = false;
			}
		}
	}
	
	// Tarkastusarvolla informoidaan k�ytt�j�� punnitsemattomista pusseista
	return tarkastus;
}