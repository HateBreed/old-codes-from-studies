/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Kassa.cpp
//   Projekti:	Kaupassakäyntisimulaatio
//     Versio:	1.0
//     Tekijä:	Jussi Laakkonen
//        Pvm:	30.3.2006
//
//  Tarkoitus:	Kassa-luokka, korin sisällön laskeminen
//
//  Kommentit:	Käytetään korin sisältämien tuotteiden hinnan laskemiseen,
//				sekä punnitsemattomien pussien tarkistukseen ja punnituksen
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

// Ostoskorin sisällön hinnan laskeva funktio
// Saa parametrina osoittimen koriin
void Kassa::laskeJaTulosta( Kori& klaskeKori )
{
	double summa = 0;
	int tyhjat = 0, i = 0, kTulostus = 2;

	// Tulostetaan korin sisältö käyttäjälle, tulostusmetodi 2 - kuitti
	klaskeKori.naytaKori( kTulostus );

	// Käydään korin sisältö läpi, tuotteiden hinnat lisätään loppusummaan
	for( i = 0; i < 30; i++ )
	{
		Tuote* kKohta = klaskeKori.kerroTuote(i);
		if( kKohta )
		{
			// Jos kyseisessä kohdassa on pussi, kysytään pussin kokonaishinta
			if( (kKohta->getTyyppi() == pussi) )
			{
				Pussi* pkohta = static_cast<Pussi*>(kKohta);
				// Jos pussi on tyhjä (kaikki pussit punnitaan ennen korin
				// laskemista, joten vain aidosti tyhjät jäävät)
				if( pkohta->getLaji() == pTyhja )
				{
					tyhjat++;
				}

				else
				{
					summa = summa + pkohta->sisaltoHinta();
				}
			}
			
			// Muutoin summaan lisätään tuotteen hinta
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
	
	// Jos tyhjiä pusseja löytyi, niin tästä ilmoitetaan käyttäjälle
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

// Pussit tarkastava funktio, suoritetaan ennen korin sisällön laskemista
// Palauttaa boolean arvolla true, jos punnitsemattomia pusseja ei löydy
bool Kassa::tarkastaPussit( Kori& tarkKori )
{
	bool tarkastus = true;

	// Käydään korin tuotteet läpi
	for( int i = 0; i < 30; i++ )
	{
		Tuote* pK = tarkKori.kerroTuote(i);

		// Jos löydetään korista pussi, niin tarkistetaan onko se punnittu
		// Jos pussia ei ole punnittu, se punnitaan setPaino()-funtkiota
		// käyttämällä
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
	
	// Tarkastusarvolla informoidaan käyttäjää punnitsemattomista pusseista
	return tarkastus;
}