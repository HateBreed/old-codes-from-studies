/*////////////////////////////////////////////////////////////////////////////
//
//
//   Tiedosto:	Kori.cpp
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

#include "Kori.h"

#include "Jugurtti.h"
#include "Maito.h"
#include "Pussi.h"
#include "Riisi.h"

#include <cstddef>
#include <iostream>
#include <iomanip>

using namespace kauppa;
using namespace std;

// Konstruktori
Kori::Kori()
{
	for(int i = 0; i < 30; i++)
		sisalto[i] = NULL;
}

// Destruktori
Kori::~Kori()
{
	for(int i = 0; i < 30; i++)
	{
		delete sisalto[i];
		sisalto[i] = NULL;
	}
}

// Korin sisällön tulostava funktio
void Kori::naytaKori( const int tulostus )
{
	// Apumuuttujat eri tuotteiden esiintymismäärien laskemiseen
	int kmaito = 0, kjugurtti = 0, kriisi = 0;
	double khmaito = 0, khjugurtti = 0, khriisi = 0;

	cout << endl;

	// Jos korissa on tuotteita
	if( tuotteita() > 0 )
	{
		// Tulostustyyppi, 1 = vain korin sisällön näyttäminen
		// muilla arvoilla kuitintapainen tulostus
		if( tulostus == 1 )
		{
			cout << "Ostoskorisi sisalto:\n" << endl;
		}

		else
		{
			cout << "Kuitti ostoksistasi" << endl << endl;
		}

		cout << "Tuote\t\t\tLkm\tPaino (kg)";

		if( tulostus != 1)
		{
			cout << "\tYht. hinta";
		}

		cout << endl;

		// Käydään korin sisältö läpi
		for(int i = 0; i < 30; i++)
		{
			if( ( sisalto[i] ) && ( sisalto[i]->getTyyppi() == maito ) )
			{
				Muu* kMuu = static_cast<Muu*>(sisalto[i]);
				khmaito = kMuu->getHinta();
				kmaito++;
			}
				
			else if( ( sisalto[i] ) && ( sisalto[i]->getTyyppi() == jugurtti ) )
			{
				Muu* kMuu = static_cast<Muu*>(sisalto[i]);
				khjugurtti = kMuu->getHinta();
				kjugurtti++;
			}
	
			else if( ( sisalto[i] ) && ( sisalto[i]->getTyyppi() == riisi ) ) 
			{
				Muu* kMuu = static_cast<Muu*>(sisalto[i]);
				khriisi = kMuu->getHinta();
				kriisi++;
			}

			// Jos korissa on pussi, tulostetaan sen tiedot samantein
			// Käytetään lajin ja painon selvitykseen Pussi-luokan tarjoamia funktioita
			else if( ( sisalto[i] ) && ( sisalto[i]->getTyyppi() == pussi ) )
			{
				Pussi* p = static_cast<Pussi*>(sisalto[i]);

				// Jos pussi on tyhjä
				if( p->getLaji() == pTyhja )
				{
					cout << "Tyhja pussi" << endl;
				}

				// Muutoin tulostetaan pussin tiedot lajin ja hedelmien määrän perusteella
				else
				{
					if( p->getLaji() == pBanaani)
					{
						cout << "Pussi (banaani)\t\t";
						cout << p->pussissa() << "\t";
					}

					else if( p->getLaji() == pMandariini )
					{
						cout << "Pussi (mandariini)\t";
						cout << p->pussissa() << "\t";
					}

					else if( p->getLaji() == pPaaryna )
					{
						cout << "Pussi (paaryna)\t\t";
						cout << p->pussissa() << "\t";
					}

					if(p->getPainop() == 0)
					{
						cout << "punnitsematon" << endl;
					}
					else
					{
						cout << p->getPainop();
						// Jos tulostetaan kuittia, niin tulostetaan myös yhteis hinta
						if( tulostus != 1)
						{
							cout << "\t\t" << setprecision(3) << p->sisaltoHinta() << " euroa";
						}

						cout << endl;
					}
				}
			}
		
		}
	
	}

	else
		cout << "Ostoskorisi on tyhja." << endl;

	// Tulostetaan eri tuotteiden määrät korissa
	if(kmaito != 0)
	{
		cout << "Maitopurkki\t\t" << kmaito;
		double kilo = 1.50;
		cout << "\t" << setprecision(3) << kilo*kmaito;

		// Kuitti
		if( tulostus != 1 )
		{
			cout << "\t\t" << setprecision(3) << khmaito*kmaito << " euroa";
		}
		cout << endl;
	}

	if(kjugurtti != 0)
	{
		cout << "Jugurttipurkki\t\t" << kjugurtti;
		double kilo = 0.90;
		cout << "\t" << setprecision(3) << kilo*kjugurtti;

		// Kuitti
		if( tulostus != 1 )
		{
			cout << "\t\t" << setprecision(3) << khjugurtti*kjugurtti << " euroa";
		}
		cout << endl;
	}

	if(kriisi != 0)
	{
		cout << "Riisipussi\t\t" << kriisi;
		double kilo = 0.525;
		cout << "\t" << setprecision(3) << kilo*kriisi;

		// Kuitti
		if( tulostus != 1 )
		{
			cout << "\t\t" << setprecision(3) << khriisi*kriisi << " euroa";
		}
		cout << endl;
		
	}
	
	cout << endl;

	if( tulostus == 1 )
	{
		cout << "Korissa tilaa viela " << 30-tuotteita() << ":lle tuotteelle" << endl << endl;
	}

}

// Käytetään hakemaan haluttu pussi korista, pussi on joko saman lajin hedelmiä sisältävä ja punnitsematon (paino == 0)
// tai pussi on tyhjä, kun pussi on löytynyt, palautetaan osoitin siihen, muutoin palautetaan NULL-osoitin
Pussi* Kori::haePussi( pussiLaji laji ) const
{
	for(int i = 0; i < 30; i++)
	{		
		if( ( sisalto[i] ) && ( sisalto[i]->getTyyppi() == pussi) )
		{
			Pussi* hp = static_cast<Pussi*>(sisalto[i]);

			if( ( ( hp->getLaji() == laji ) && ( hp->getPainop() == 0 ) ) || ( hp->getLaji() == pTyhja ) )
			{
				return hp;
			}
		}
	}
	return NULL;
}

// Palauttaa korissa olevien tuotteiden määrän
const int Kori::tuotteita() const
{
	int tuotteet = 30;
	for(int i = 0; i < 30; i++)
	{
		if( sisalto[i] == NULL)
		{
			tuotteet--;
		}
	}
	return tuotteet;
}


// Lisää tuotteen koriin
bool Kori::tKoriin( Tuote* lisattavaTuote, const int kPaikka )
{
	sisalto[kPaikka] = lisattavaTuote;
	return true;
}

// Palauttaa osoittimen haluttuun tuotteeseen
Tuote* Kori::kerroTuote( const int x ) const
{
	return sisalto[x];
}
