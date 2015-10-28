/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Kauppaliittyma.cpp
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


#include "Kauppaliittyma.h"

#include "Kassa.h"
#include "Kori.h"
#include "Tuote.h"

#include "Jugurtti.h"
#include "Maito.h"
#include "Pussi.h"
#include "Riisi.h"
#include "Banaani.h"
#include "Mandariini.h"
#include "Paaryna.h"

#include <iostream>

using namespace kauppa;
using namespace std;

// Konstruktori
Kauppaliittyma::Kauppaliittyma()
{
	for( int i = 0; i < 60; i++ )
	{
		hylly[i] = NULL;
	}
}


// Destruktori
Kauppaliittyma::~Kauppaliittyma()
{
	for( int i = 0; i < 60; i++ )
	{
		delete hylly[i];
		hylly[i] = NULL;
	}
}


// Ostoskorin luova funktio
Kori* Kauppaliittyma::otaKori()
{
	Kori* kauppaKori;
	kauppaKori = new Kori();
	return kauppaKori;
}

// Uuden pussin luonti, pussi siirret��n suoraan koriin 
bool Kauppaliittyma::otaPussi( Kori& opKori )
{
	Pussi* lPussi = new Pussi( pussi, pTyhja );
	return koriin( lPussi, opKori );
}


// Luo tuotteet hyllyyn
void Kauppaliittyma::aloita()
{
	// Hedelmien painot
	double pBanaani = 0.11, pMandariini = 0.04, pPaaryna = 0.08;

	// Muiden tuotteiden kappalehinnat
	double hJugurtti = 1.09, hMaito = 0.79, hRiisi = 1.22;

	int i = 0;
	//Jokaista tuotetta lis�t��n 10 kappaletta hyllyyn

	// Banaanit
	for( i = 0; i < 10; i++ )
	{
		hylly[i] = new Banaani( banaani, pBanaani );
	}

	// Mandariinit
	for( i = 10; i < 20; i++ )
	{
		hylly[i] = new Mandariini( mandariini, pMandariini );
	}

	// P��ryn�t
	for( i = 20; i < 30; i++ )
	{
		hylly[i] = new Paaryna( paaryna, pPaaryna );
	}

	// Maidot
	for( i = 30; i < 40; i++ )
	{
		hylly[i] = new Maito( maito, hMaito );
	}

	// Jugurtit
	for( i = 40; i < 50; i++ )
	{
		hylly[i] = new Jugurtti( jugurtti, hJugurtti );
	}

	// Riisit
	for( i = 50; i < 60; i++ )
	{
		hylly[i] = new Riisi( riisi, hRiisi );
	}
}

// Siirt�� tuotteen (kopion) koriin, palauttaa true, jos korissa on tilaa
bool Kauppaliittyma::koriin( Tuote* lisattava, Kori& kKori )
{
	// Haetaan korista lis�tt�v�lle tuotteelle paikka, tuotteita()-funktio
	// palauttaa korissa olevien tuotteiden m��r�n, tallennustilan indeksin
	// alkaessa nollasta, palautettu arvo on tuotteen lis�yspaikka
	const int paikka = kKori.tuotteita();
	
	// Jos korissa on tilaa, lis�t��n tuote sinne k�ytt�m�ll� korin tKoriin()
	// funktiota, joka palauttaa totuusarvon lis�yksen onnistumisesta
	if( paikka < 30 )
	{
		return kKori.tKoriin( lisattava, paikka );	
	}

	// Jos tilaa ei ole
	return false;
}


// Siirt�� tuotteen (kopion) korissa olevaan sopivaan pussiin, jos sopiva pussi
// on l�ytynyt ja siin� on tilaa, niin tuote lis�t��n pussiin ja palautetaan true
bool Kauppaliittyma::pussiin( Tuote* tHedelma, const Kori& kKori )
{
	Pussi* lisaysPussi = NULL;

	// Haetaan sopiva pussi hedelm�n lajin perusteella haePussi()-funktiolla
	if( tHedelma->getTyyppi() == banaani )
	{
		lisaysPussi = kKori.haePussi( pBanaani );
	}

	else if( tHedelma->getTyyppi() == mandariini )
	{
		lisaysPussi = kKori.haePussi( pMandariini );
	}

	else
	{
		lisaysPussi = kKori.haePussi( pPaaryna );
	}

	// Jos l�ydet��n sopiva pussi, tarkistetaan onko siin� tilaa
	if( lisaysPussi != NULL)
	{
		// Sama tehd� tilan tarkistus t�ss�, koska kuitenkin joudutaan hakemaan
		// oikea paikka pussista siihen lis�tt�v�lle hedelm�lle
		int paikka = lisaysPussi->pussissa();

		// Jos pussissa on tilaa, niin lis�t��n hedelm� sinne ja palautetaan true
		if( paikka < 10 )
		{
			lisaysPussi->tPussiin( tHedelma, paikka );

			// Jos lis�t��n tyhj��n pussiin, niin sille asetetaan lis�tt�v�n
			// hedelm�n laji
			if( paikka == 0 )
			{
				if( tHedelma->getTyyppi() == banaani )
				{
					lisaysPussi->setLaji( pBanaani );
				}

				else if( tHedelma->getTyyppi() == mandariini )
				{
					lisaysPussi->setLaji( pMandariini );
				}

				else
				{
					lisaysPussi->setLaji( pPaaryna );
				}
			}

			return true;
		}
	}

	// Jos tilaa ei ole tai sopivaa pussia ei ole korissa
	return false;
}

// Kaupan hyllyss� oleavat tuotteet n�ytt�v� funktio
void Kauppaliittyma::naytaTuotteet()
{
	int ibanaani = 0, ijugurtti = 0, imaito = 0, imandariini = 0, ipaaryna = 0, iriisi = 0;
	int i = 0;
	
	// Ensiksi lasketaan hyllyss� olevat tuotteet
	for( i = 0; i < 60; i++ )
	{
		if( hylly[i] )
		{
			if( hylly[i]->getTyyppi() == banaani )
			{
				ibanaani++;
			}

			else if( hylly[i]->getTyyppi() == mandariini )
			{
				imandariini++;
			}

			else if( hylly[i]->getTyyppi() == paaryna )
			{
				ipaaryna++;
			}

			else if( hylly[i]->getTyyppi() == maito )
			{
				imaito++;
			}

			else if( hylly[i]->getTyyppi() == jugurtti )
			{
				ijugurtti++;				
			}

			else if( hylly[i]->getTyyppi() == riisi )
			{
				iriisi++;
			}
			else
			{

			}
		}
	}

	// Tulostetaan tuotteet ja niiden m��r�t, sek� hinnat k�ytt�j�lle
	// Jos jotain tuotetta ei ole en�� j�ljell� hyllyss�, niin sit� ei n�ytet�
	// Numero tuotteen edess� vastaa ko. tuotteen valintanumeroa
	if( ( ibanaani != 0) || ( imandariini != 0 ) || ( ipaaryna != 0 ) )
	{
		cout << "HEDELMIA: " << endl;

		// Banaanit
		if( ibanaani != 0 )
		{
			cout << "(1) Banaaneja:\t\t" << ibanaani << " kpl";

			for( i = 0; i < 10; i++ )
			{
				if( hylly[i] )
				{
					Hedelma* h = static_cast<Hedelma*>(hylly[i]);
					cout << "\ta " << h->getPaino() << " kg " << endl;
					break;
				}
			}
		}

		// Mandariinit
		if( imandariini != 0 )
		{
			cout << "(2) Mandariineja:\t" << imandariini << " kpl";

			for( i = 10; i < 20; i++ )
			{
				if( hylly[i] )
				{
					Hedelma* h = static_cast<Hedelma*>(hylly[i]);
					cout << "\ta " << h->getPaino() << " kg " << endl;
					break;
				}
			}
		}

		// Paarynat
		if( ipaaryna != 0 )
		{
			cout << "(3) Paarynoita:\t\t" << ipaaryna << " kpl";

			for( i = 20; i < 30; i++ )
			{
				if( hylly[i] )
				{
					Hedelma* h = static_cast<Hedelma*>(hylly[i]);
					cout << "\ta " << h->getPaino() << " kg " << endl;
					break;
				}
			}

		}
	}

		cout << endl;

	if( imaito != 0 || ijugurtti != 0 || iriisi != 0 )
	{
		cout << "MUITA TUOTTEITA:" << endl;

		// Maidot
		if( imaito != 0 )
		{
			cout << "(4) Maitopurkkeja:\t" << imaito << " kpl";

			for( i = 30; i < 40; i++ )
			{
				if( hylly[i] )
				{
					Muu* m = static_cast<Muu*>(hylly[i]);
					cout << "\ta " << m->getHinta() << " euroa " << endl;
					break;
				}
			}
		}

		// Jugurtit
		if( ijugurtti != 0 )
		{
			cout << "(5) Jugurttipurkkeja:\t" << ijugurtti << " kpl";

			for( i = 40; i < 50; i++ )
			{
				if( hylly[i] )
				{
					Muu* m = static_cast<Muu*>(hylly[i]);
					cout << "\ta " << m->getHinta() << " euroa " << endl;
					break;
				}
			}
		}

		// Riisit
		if( iriisi != 0 )
		{
			cout << "(6) Riisipusseja:\t" << iriisi << " kpl";

			for( i = 50; i < 60; i++ )
			{
				if( hylly[i] )
				{
					Muu* m = static_cast<Muu*>(hylly[i]);
					cout << "\ta " << m->getHinta() << " euroa " << endl;
					break;
				}
			}
		}
	}
		cout << endl;

		cout << "(7) Pussi" << endl;
		cout << "(8) Poistu" << endl;
	
}


// Valitun tuotteen lis��minen koriin (tai pussiin)
// Ensiksi tulostetaan k�ytt�j�lle hyllyss� olevat tuotteet
void Kauppaliittyma::tuoteKoriin( Kori& tKori )
{
	char tValinta = 0;
	bool tSilmukka = true, tTarkistus = true;
	int a = 0, tnValinta = 0;
		
	// Tuotteen valintasilmukka
	while( tSilmukka )
	{
		cout << "\nValitse tuote numeron perusteella.\n" << endl;
		naytaTuotteet();
		cout << "\n: " << flush;
		cin >> ws;
		cin >> tValinta;

		if( tValinta == '1') tnValinta = 1;
		else if( tValinta == '2') tnValinta = 2;
		else if( tValinta == '3') tnValinta = 3;
		else if( tValinta == '4') tnValinta = 4;
		else if( tValinta == '5') tnValinta = 5;
		else if( tValinta == '6') tnValinta = 6;

		// Hedelm�t
		if( ( tValinta == '1' ) || ( tValinta == '2' ) || ( tValinta == '3' ) )
		{
			// Haetaan hyllyst� oikea tuote valinnan perusteella
			for( a = tnValinta*10-10; a < tnValinta*10; a++)
			{
				// Jos hyllyst� l�ytyy tuote, niin poistutaan silmukasta
				if( hylly[a] != NULL )
				{
					break;
				}

				// Jos hyllyst� ei l�ydy tuotetta, niin tarkistusarvoksi false
				else if( ( hylly[a] == NULL ) && ( a == tnValinta*10-1 ) )
				{
					tTarkistus = false;
				}
			}
	
			// Jos hyllyst� l�ytyi tuote
			if( tTarkistus )
			{
				// Lis�t��n tuote pussiin
				if( pussiin( hylly[a], tKori ) )
				{
					cout << "Hedelma lisattiin pussiin." << endl;

					// Siirto pussiin onnistunut, voidaan tuhota hyllyss� oleva
					hylly[a] = NULL;
				}

				// Jos lis�ysfunktion paluuarvo on false, niin hedelm�� ei voitu lis�t� pussiin
				else
				{
					cout << "Hedelmaa ei voitu lisata,";

					if( tKori.tuotteita() < 30 )
					{
						cout << " korissa ei ollut sopivaa pussia, ota uusi." << endl;
					}
					
					else
					{
						cout << " korissa ei ollut sopivaa pussia, korissa ei ole tilaa uudelle pussille." << endl;
					}
				}
			}

			// Jos tuotetta ei l�ytynyt
			else
			{
				cout << "Hyllyssa ei ollut haluttua hedelmaa jaljella." << endl;
			}

		}

		// Muut tuotteet
		else if( ( tValinta == '4' ) || ( tValinta == '5' ) || ( tValinta == '6' ) )
		{
			// Haetaan hyllyst� oikea tuote valinnan perusteella
			for( a = tnValinta*10-10; a < tnValinta*10; a++)
			{
				// Jos hyllyst� l�ytyy tuote, niin poistutaan silmukasta
				if( hylly[a] != NULL )
				{
					break;
				}

				// Jos hyllyst� ei l�ydy tuotetta, niin tarkistusarvoksi false
				else if( ( hylly[a] == NULL ) && ( a == tValinta*10-1 ) )
				{
					tTarkistus = false;
				}
			}
			
			// Jos hyllyst� l�ytyi tuote
			if( tTarkistus )
			{
				// Lis�t��n tuote koriin
				if( koriin( hylly[a], tKori ) )
				{
					cout << "Tuote lisattiin koriin." << endl;
	
					// Siirto koriin onnistunut, voidaan tuhota hyllyss� oleva
					hylly[a] = NULL;
				}
	
				// Jos korissa ei ollut tilaa
				else
				{
					cout << "Tuotetta ei voitu lisata koriin, korissa ei ole tilaa." << endl;
				}
			}

			// Jos tuotetta ei l�ytynyt
			else
			{
				cout << "Hyllyssa ei ollut haluttua tuotetta jaljella." << endl;
			}
		}

		// Pussin lis�ys
		else if( tValinta == '7' )
		{
			if( otaPussi( tKori ) )
			{
				cout << "Pussi lisattiin koriin" << endl;
			}
			else
			{
				cout << "Korissa ei ollut tilaa pussille" << endl;
			}
		}

		// Tuotteiden lis�yksest� poistuminen
		else if( tValinta == '8' )
		{
			tSilmukka = false;
		}

		else
		{
			cout << "Virheellinen valinta! Tuotevalikosta poistutaan" << endl;
			break;

		}
	}
	cout << endl;

}


// Haetaan korista punnitsemattomat pussit punnittaviksi
void Kauppaliittyma::valitsePunnittavat( Kori &vpKori )
{
	// tarkistus - oliko punnitsemattomia pusseja
	bool punPussit = false, pTark = true;

	// K�yd��n korin tuotteet l�pi
	for( int i = 0; i < 30; i++ )
	{
		Tuote* kt = vpKori.kerroTuote( i );
		pTark = true;

		// Jos kyseisest� paikasta l�ydet��n pussi
		if( ( kt ) && ( kt->getTyyppi() == pussi) )
		{
			Pussi* pkt = static_cast<Pussi*>(kt);

			// Tarkistetaan onko pussi jo punnittu tai onko siell� jotain
			if( ( pkt->getPainop() == 0 ) && ( pkt->getLaji() != pTyhja) )
			{
				// Ilmoitetaan k�ytt�j�lle l�ytyneest� punnitsemattomasta pussista
				char punnitse;
				punPussit = true;
				cout << "\nPunnitsematon pussi ";
				
				if( pkt->getLaji() == pBanaani ) cout << "banaaneja, ";
				else if( pkt->getLaji() == pMandariini ) cout << "mandariineja, ";
				else if( pkt->getLaji() == pPaaryna ) cout << "paarynoita, ";
				
				while( pTark )
				{
					// Punnitaanko vaiko ei
					cout << "punnitse pussi? (k/e): ";
					cin >> ws;
					cin >> punnitse;
						
					if( punnitse == 'k' )
					{
						pkt->setPainop();
						cout << "\nPussi punnittu, paino: " << pkt->getPainop() << " kg" << endl; 
						pTark = false;
					}

					else if( punnitse == 'e' )
					{
						cout << "\nPussia ei punnittu" << endl;
						pTark = false;
					}
					else 
					{
						cout << "\n Virheellinen valinta!" << endl;
						pTark = true;
					}
				}
			}
		}
	}

	// Jos korissa ei ollut punnitsemattomia pusseja
	if( !punPussit)
	{
		cout << "\nKorissa ei ole punnitsemattomia pusseja." << endl;
	}
	cout << endl;
}


// K�yttt�liittym� kaupan eri palveluihin
void Kauppaliittyma::kaynnistaJaSuorita()
{
	char kValinta = 0;
	bool kSilmukka = true;

	// Aloitusfunktio, luo tuotteet
	aloita();

	// Korin luonti
	Kori* ostosKori = otaKori();

	// Tervehdystekstit
	cout << "Tervetuloa kauppaan, sinulle on luotu ostoskori.\n";
	cout << "Valitse haluttu toiminto:\n" << endl;

	// Toiminnan valitsemissilmukka
	while( kSilmukka )
	{
		cout << "(1) Nayta kaupan tuotteet ja lisaa haluamasi tuote koriin." << endl;
		cout << "(2) Nayta ostoskorisi sisalto." << endl;
		cout << "(3) Punnitse ostoskorin pusseja." << endl;
		cout << "(4) Siirry kassalle ja poistu kaupasta." << endl;
		cout << "(5) Poistu kaupasta.\n" << endl;
		cout << "(V) ";

		cin >> ws;
		cin >> kValinta;

		// Tuotteiden lis�ys koriin
		if( kValinta == '1' )
		{
			tuoteKoriin( *ostosKori );
		}

		// Korin sis�ll�n tarkistaminen, jos korissa on jotain, niin sis�lt� n�ytet��n
		else if( kValinta == '2' )
		{
				int tTulostus = 1;
				ostosKori->naytaKori( tTulostus );
		}
		
		// Pussin punnitseminen
		else if( kValinta == '3' )
		{
			valitsePunnittavat( *ostosKori );				
		}

		// Kassalle siirtyminen
		else if( kValinta == '4' )
		{
			// Luodaan kassa
			Kassa kauppaKassa;

			// Jos punnitsemattomia pusseja oli korissa, niin ilmoitetaan k�ytt�j�lle
			if( !kauppaKassa.tarkastaPussit( *ostosKori ) )
			{
				cout << "Ostoskorissa oli punnitsemattomia pusseja." << endl;		
			}

			// Lasketaan korin sis�lt�, funktio tulostaa tiedot ruudulle
			kauppaKassa.laskeJaTulosta( *ostosKori );

			// Kassalle menon j�lkeen poistutaan kaupasta
			cout << "\nKiitos kaynnista, tervetuloa uudelleen." << endl;
			kSilmukka = false;
		}

		// Kaupasta poistuminen
		else if( kValinta == '5' )
		{
			cout << "\nKiitos kaynnista, tervetuloa uudelleen." << endl;
			kSilmukka = false;
		}

		// Muutoin valinta oli virheellinen
		else
		{
			cout << "Virheellinen valinta!\n" << endl;
		}
	}

}
