/*
 * ActionPi.cpp
 * Version 1.02
 * IT-Hardware Projekt GWS-Bühl
 * Copyright 2016  <admin@kellerkinder.xyz>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *zum kompilieren ---> g++ -Wall -c "%f" -l raspicam -l bcm2835 -std=c++0x
 */
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <bcm2835.h>
#include <raspicam/raspicam.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

using namespace std;

string gibDateiName();
string getFolderName();
void videoAufnehmen();
int fotoAufnehmen();
void liveAusgabe();
void videoAbspeichern();
void initialisierung();
void blinken();
void herunterfahren();
void programmBeenden();
void convert();
void creatNewFolder();
void creatDCIM();
bool ordnerExistiert(string pfad);
void pruefeNewFolder(string input);
void rueckschau(string input);
void setStandardWerte();
void einstellungen();
void einstellungErhoehen();
void einstellungVerringern();
void zuruecksetzten();
void iso(int richtung);
void helligkeit(int richtung);
void kontrast(int richtung);
void saettigung(int richtung);
void verschlusszeit(int richtung);

void setISO ( int iso );
void setBrightness ( unsigned int brightness );
void setContrast ( int contrast );
void setSaturation ( int saturation );
void setExposureCompensation ( int val ); 
void setShutterSpeed ( unsigned int ss );

int getMODI();
int getREC();
int getEx1();
int getEx2();
int getEx3();

#define LED RPI_V2_GPIO_P1_13 
#define MODI RPI_V2_GPIO_P1_22
#define REC RPI_V2_GPIO_P1_18
#define Ex1 RPI_V2_GPIO_P1_16
#define Ex2 RPI_V2_GPIO_P1_15
#define Ex3 RPI_V2_GPIO_P1_11

int valMODI;
int valREC;
int valEx1;
int valEx2;
int valEx3;

int valISO;
int valHelligkeit;
int valKontrast;
int valSaettigung;
int valVerschlusszeit;
int zaehlerISO = 1;
int zaehlerHelligkeit = 1;
int zaehlerKontrast = 1;
int zaehlerSaettigung = 2;
int zaehlerVerschlusszeit = 4;

int einstellung = 0; // 0=Verschlusszeit;1=ISO;2=Kontrast;3=Helligkeit;4=Sättigung;
int wieLangeGedrueckt = 0;

bool aktiv = true;
bool videoAufnahme = false;
bool liveBildWirdAngezeigt = false;

int main(int argc, char **argv)
{
	creatDCIM();	//prüft ob DCIM Ordner existiert, sollt er nicht existieren wird er erstellt
	creatNewFolder();    //prüft ob Datums-Ordner existiert, sollt er nicht existieren wird er erstellt
	if (!bcm2835_init())
	return 1;
	 initialisierung();
	 setStandardWerte();
	  while (aktiv)
    {
		if(videoAufnahme && getMODI() )
		{
			videoAbspeichern();
		}
		if(!liveBildWirdAngezeigt)
		{
	     liveAusgabe(); // Live-Ausgabe wird gestartet
        }
	if(getREC())
	{
    //REC Taster gedrückt
    if(getMODI())
    {
		//Fotomodus
		fotoAufnehmen();
	}
	else{
		//Videomodus
		if(!videoAufnahme)
		{
			videoAufnehmen();
			delay(1000); // mindestens 1sec Video
		}
		else{
			videoAbspeichern();
		}
	}
}
		if(getMODI()){ // Eindtellungen nur im Fotomodus
		einstellungen();
		einstellungErhoehen();
		einstellungVerringern();
		zuruecksetzten();
		}
    delay(100);
    programmBeenden(); // prüft ob das Programm beendet werden soll
	herunterfahren(); // prüft ob der Pi heruntergefahren werden soll
} //while Ende
    bcm2835_close();
	return 0;
}

//setzt Fotoeinstellungen zurück
void zuruecksetzten()
{
	if(getEx2()){
	if(wieLangeGedrueckt < 20){
		wieLangeGedrueckt = wieLangeGedrueckt +1;
		//cout<<wieLangeGedrueckt<<endl;
	}
	else{
	wieLangeGedrueckt = 0;
	cout<<"Zurückgesetzt"<<endl;
	blinken();
	setStandardWerte();
	}
}
	return;
}

//öffnet Einstellungen und zeigt die verschiedenen Optionen an
void einstellungen()
{
	if(getEx2()){
		delay(100);
		einstellung = einstellung +1;
		if(einstellung == 5){
			einstellung = 0;
		}
		//cout<<einstellung<<endl;
	
	    if(einstellung ==1){
			cout<<"ISO:";
			cout<<valISO<<endl;
		}
		if(einstellung == 3){
			cout<<"Helligkeit:";
			cout<<valHelligkeit<<endl;
		}
		if(einstellung == 2){
			cout<<"Kontrast:";
			cout<<valKontrast<<endl;
		}
		if(einstellung == 4){
			cout<<"Saettigung:";
			cout<<valSaettigung<<endl;
		}
		if(einstellung == 0){
			cout<<"Verschlusszeit:";
			cout<<valVerschlusszeit<<endl;
		}
	}
	return;
}

//setzt die gewählte Einstellung auf den nächts höheren Wert
void einstellungErhoehen()
{
	if(getEx3()){
		delay(100);
		wieLangeGedrueckt = 0;
	if(einstellung == 1){
			//ISO
			iso(1);
		}
		if(einstellung == 3){
			//Helligkeit
			helligkeit(1);
		}
		if(einstellung == 2){
			//Kontrast
			kontrast(1);
		}
		if(einstellung == 4){
			//Sättigung
			saettigung(1);
		}
		if(einstellung == 0){
			//Verschlusszeit
			verschlusszeit(1);
		}
	}
	return;
}

//setzt die gewählte Einstellung auf den nächts niedrigeren Wert
void einstellungVerringern()
{
	if(getEx1()){
		delay(100);
		wieLangeGedrueckt = 0;
	if(einstellung == 1){
			//ISO
			
			iso(-1);
		}
		if(einstellung == 3){
			//Helligkeit
			
			helligkeit(-1);
		}
		if(einstellung == 2){
			//Kontrast
			
			kontrast(-1);
		}
		if(einstellung == 4){
			//Sättigung
		
			saettigung(-1);
		}
		if(einstellung == 0){
			//Verschlusszeit
		
			verschlusszeit(-1);
		}
	}
	return;
}

//Werte für ISO 100,400,600,600,800 min:100; max:800
void iso(int richtung)
{
		zaehlerISO = zaehlerISO + (richtung);
		if(zaehlerISO == -1){
			zaehlerISO = 0;
		}
		if(zaehlerISO == 4){
			zaehlerISO = 3;
		}
		
		if(zaehlerISO == 0){
			valISO = 100;
			cout<<"ISO:";
			cout<<valISO<<endl;
		}
		if(zaehlerISO == 1){
			valISO = 400;
			cout<<"ISO:";
			cout<<valISO<<endl;
		}
		if(zaehlerISO == 2){
			valISO = 600;
			cout<<"ISO:";
			cout<<valISO<<endl;
		}
		if(zaehlerISO == 3){
			valISO = 800;
			cout<<"ISO:";
			cout<<valISO<<endl;
		}
	return;
	
}

//Werte für Helligkeit 25,50,75,100
void helligkeit(int richtung)
{
		zaehlerHelligkeit = zaehlerHelligkeit + (richtung);
		if(zaehlerHelligkeit == -1){
			zaehlerHelligkeit = 0;
		}
		if(zaehlerHelligkeit == 4){
			zaehlerHelligkeit = 3;
		}
		
		if(zaehlerHelligkeit == 0){
			valHelligkeit = 25;
			cout<<"Helligkeit:";
			cout<<valHelligkeit<<endl;
		}
		if(zaehlerHelligkeit == 1){
			valHelligkeit = 50;
			cout<<"Helligkeit:";
			cout<<valHelligkeit<<endl;
		}
		if(zaehlerHelligkeit == 2){
			valHelligkeit = 75;
			cout<<"Helligkeit:";
			cout<<valHelligkeit<<endl;
		}
		if(zaehlerHelligkeit == 3){
			valHelligkeit = 100;
			cout<<"Helligkeit:";
			cout<<valHelligkeit<<endl;
		}
	return;
}

//Werte für Kontrast -50,0,50,100
void kontrast(int richtung)
{
		zaehlerKontrast = zaehlerKontrast + (richtung);
		if(zaehlerKontrast == -1){
			zaehlerKontrast = 0;
		}
		if(zaehlerKontrast == 4){
			zaehlerKontrast = 3;
		}
		
		if(zaehlerKontrast == 0){
			valKontrast = -50;
			cout<<"Kontrast:";
			cout<<valKontrast<<endl;
		}
		if(zaehlerKontrast == 1){
			valKontrast = 0;
			cout<<"Kontrast:";
			cout<<valKontrast<<endl;
		}
		if(zaehlerKontrast == 2){
			valKontrast = 50;
			cout<<"Kontrast:";
			cout<<valKontrast<<endl;
		}
		if(zaehlerKontrast == 3){
			valKontrast = 100;
			cout<<"Kontrast:";
			cout<<valKontrast<<endl;
		}
	return;
}

//Werte für Sättigung -100,-75,0,50,100
void saettigung(int richtung)
{
		zaehlerSaettigung = zaehlerSaettigung + (richtung);
		if(zaehlerSaettigung == -1){
			zaehlerSaettigung = 0;
		}
		if(zaehlerSaettigung == 5){
			zaehlerSaettigung = 4;
		}
		
		if(zaehlerSaettigung == 0){
			valSaettigung = -100;
			cout<<"Saettigung:";
			cout<<valSaettigung<<endl;
		}
		if(zaehlerSaettigung == 1){
			valSaettigung = -75;
			cout<<"Saettigung:";
			cout<<valSaettigung<<endl;
		}
		if(zaehlerSaettigung == 2){
			valSaettigung = 0;
			cout<<"Saettigung:";
			cout<<valSaettigung<<endl;
		}
		if(zaehlerSaettigung == 3){
			valSaettigung = 50;
			cout<<"Saettigung:";
			cout<<valSaettigung<<endl;
		}
		if(zaehlerSaettigung == 4){
			valSaettigung = 100;
			cout<<"Saettigung:";
			cout<<valSaettigung<<endl;
		}
	return;
}

//Werte für Verschluss-/Belichtungszeit 0,5000,20000,40000,100000 kleiner = kürzere Verschlusszeit
void verschlusszeit(int richtung)
{
		zaehlerVerschlusszeit = zaehlerVerschlusszeit + (richtung);
		if(zaehlerVerschlusszeit == -1){
			zaehlerVerschlusszeit = 0;
		}
		if(zaehlerVerschlusszeit == 5){
			zaehlerVerschlusszeit = 4;
		}
		
		if(zaehlerVerschlusszeit == 0){
			valVerschlusszeit = 5000;
			cout<<"Verschlusszeit:";
			cout<<valVerschlusszeit<<endl;
		}
		if(zaehlerVerschlusszeit == 1){
			valVerschlusszeit = 20000;
			cout<<"Verschlusszeit:";
			cout<<valVerschlusszeit<<endl;
		}
		if(zaehlerVerschlusszeit == 2){
			valVerschlusszeit = 40000;
			cout<<"Verschlusszeit:";
			cout<<valVerschlusszeit<<endl;
		}
		if(zaehlerVerschlusszeit == 3){
			valVerschlusszeit = 100000;
			cout<<"Verschlusszeit:";
			cout<<valVerschlusszeit<<endl;
		}
		if(zaehlerVerschlusszeit == 4){
			valVerschlusszeit = 0;
			cout<<"Verschlusszeit:";
			cout<<valVerschlusszeit<<endl;
		}
	return;
}

//wird von void zuruecksetzten() aufgerufen und setzt die Fotoeinstellungen zurück auf den Standartwert
void setStandardWerte()
{
	valISO = 400;
	valHelligkeit = 50;
	valKontrast = 0;
	valSaettigung = 0;
	valVerschlusszeit = 0;
	return;
}

//startet die Videoaufnahme
void videoAufnehmen()
{
	bcm2835_gpio_set(LED); //schaltet LED an
	videoAufnahme = true; // Merker für aktuelle Videoaufnahme
	system("killall raspivid");	//beendet den Prozess raspivid (Liveausgabe)
	system("raspivid -o vid.h264 -t 1140000 -p '0,-10,720,480' &");		//nimmt video mit 19 Minuten auf -o temporäre output Datei, -p '0,-10,1280,720' für Position und Größe des Vorschau Bildes
	liveBildWirdAngezeigt = false;	//setzt Merker für Livebildausgabe
	return;
}

//nimmt ein temporäres Foto im ppm Format auf
int fotoAufnehmen()
{
	bcm2835_gpio_set(LED);	//schaltet LED an
	system("killall raspivid");
    raspicam::RaspiCam Camera; //definiert Camera Objekt
	//setzt die gewählten Fotoeinstellungen
    Camera.setISO(valISO);
    Camera.setBrightness(valHelligkeit);
    Camera.setContrast(valKontrast);
    Camera.setSaturation(valSaettigung);
    Camera.setShutterSpeed(valVerschlusszeit);

    //Öffnet Camera 
     if ( !Camera.open()) {cerr<<"Fehler beim öffnen der Kamera!"<<endl;return -1;}	//gibt Fehlermeldung aus sofern Kamera öffnen nicht möglich
    sleep(1); //schlafe 1 Sekunde bis Kamera stabilisiert (ohne Pause wird das Bild rot)
    Camera.grab();	//hohle Kamera
    //reserviere Speicher
    unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];	//setzt Bild Format auf RGB
    Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB );//aufnehmen des Bildes
    std::ofstream outFile ("bild.ppm" ,std::ios::binary ); //speichern des temporären Bildes
    outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
    outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );	//speichern der zusätzlichen Parameter
    delete data;	//gibt speicher wieder frei
    liveBildWirdAngezeigt = false;	//setzt Merker für Livebildausgabe
    convert();	//ruft Funktion zum konvertieren des temporären Bildes auf
    bcm2835_gpio_clr(LED);	//schaltet LED aus
    return 1;
}

void liveAusgabe ()
{
	liveBildWirdAngezeigt = true;	//setzt Merker für Livebildausgabe 
	system("raspivid -t -0 -p '0,-10,720,480'&"); //-p '0,0,1280,720' für Position und Größe des Vorschau Bildes
	return;
}

//konvertiert Video und speichert es auf USB Stick
void videoAbspeichern()
{
	bcm2835_gpio_clr(LED); //schaltet LED aus
	std::string datname = gibDateiName();	//holen des Dateinamen
	std::string ordnerName = getFolderName();	//holen des Ordnernamen
	pruefeNewFolder(ordnerName);	//prüfen ob Ordner bereits existiert
	std::string cmd_line = std::string() + " sudo MP4Box -fps 30 -add vid.h264 /media/usb0/DCIM/"+ordnerName+"/""\"" +datname+"\".mp4 ";	//Befehl zu packen von video in .mp4 container
	system( cmd_line.c_str() );	//ruft pack befehl auf
	system("killall raspivid");//beendet video aufnahme
	system("rm vid.h264");	//löscht temporäre vid.h264 video datei
	videoAufnahme = false;	//Merker für Videoaufnahme
	liveBildWirdAngezeigt = false;//setzt Merker für Livebildausgabe
	blinken();	//lässt LED blinken
	return;
}

//konvertiert Foto und speichert es auf USB Stick
void convert(){
	cout << "Speichern von Bild ..." << endl;
	std::string datname =  gibDateiName();	//holen des Dateinamen
	std::string ordnerName = getFolderName();	//holen des Ordnernamen
	pruefeNewFolder(ordnerName);
	std::string comand = "convert bild.ppm /media/usb0/DCIM/"+ordnerName+ "/" + datname + ".jpg";	//Befehl zum konverieren, Ordnername und Dateiname werden hinzugefügt
	const char *cComand = comand.c_str();	//befehl zum ausführen in cons char 
	system(cComand);	//ausführen des Befehls
	std::string pfad = "fbi --noverbose -T 1 /media/usb0/DCIM/"+ordnerName+ "/" + datname + ".jpg &/";	//Befehl zum anzeigen das fertigen Bildes
	system("rm bild.ppm");	//löschen des temporären Bildes
	rueckschau(pfad);
	sleep(4);	//für 4 Sekunden
	system("killall fbi");	//beendet Anzeigen des Bildes
	cout << "Abspeichern fertig" << endl;
	return;
}

//anzeigen des fertigen Bildes
void rueckschau(string input)
{
	const char *cComand = input.c_str();
	system(cComand);
	return;
	
}

//erstellt den Dateinamen aus der auktuellen Uhrzeit
string gibDateiName()
{
	time_t t = time(0);	
	struct tm *aT = localtime(&t);	//holen der aktuellen Zeit
	int istd = aT->tm_hour;	//holen der Stunden
	int imin = aT->tm_min;	//holen der Minuten
	int isec = aT->tm_sec;	//holen der Sekunden
	std::string sstd = to_string(istd);	//in String konvertieren
	std::string smin = to_string(imin);
	std::string ssec = to_string(isec);
	std::string datName = sstd + "-" + smin + "-" + ssec + "ActionPi";	//zu einem String zusammenfügen
	return datName;
}

//erstellen des neuen Ordners
void creatNewFolder()
{
	std::string folderName = getFolderName();	//holen des Ordnernamen
	std::string pfad = "/media/usb0/DCIM/" + folderName;	//Pafd des Ordners
	bool exists = ordnerExistiert(pfad);	//prüfen ob Ordner existiert
	if(exists == false)	//wenn nein dann neuen Ordner erstellen
	{
		std::string comand = "mkdir " + pfad + "";	//Befehl zum erstellen des Ordners
		const char *cComand = comand.c_str();
		system(cComand);
		cout << "Ordner erstellt." << endl;
	}
	else //falls Ordner bereits vorhanden
	{
		cout << "Ordner existiert bereits." << endl;
	}
	return;
}

//erstellen des DCIM Ordners 
void creatDCIM()
{
	std::string dcmiPfad = "/media/usb0/DCIM";
	bool dcmiExists = ordnerExistiert(dcmiPfad);
	if(dcmiExists == false){
		std::string dcmiComand = "mkdir /media/usb0/DCIM";
		const char *dcmicComand = dcmiComand.c_str();
		system(dcmicComand);
		cout << "DCIM Ordner erstellt." << endl;
	}
	else
	{
		cout << "DCIM existiert bereits." << endl;
	}
}

//prüft ob Ordner/DCIM bereits existiert
bool ordnerExistiert(string pfad)
{
	const char *cPfad = pfad.c_str();
	DIR *pDir;	//Verzeichniss wird auf aktuellen Pfad gesetzt
	bool oExists = false;
	pDir = opendir(cPfad);	//öffnen des Pfades
	if(pDir != NULL)	//falls Ordner unter dem aktuellen Pfad vorhanden pDIR ungleich NULL
	{
	oExists = true;
	closedir(pDir);		//schließen des Ordners
	}
	return oExists; 
}

//prüft ob Ordner bereits existiert (wird gebraucht sollt die Kamera länger als 0.00 Uhr in Betrieb sein "0 Uhr Bug")
void pruefeNewFolder(string input)
{
	std::string pfad = "/media/usb0/DCIM/" + input;
	bool newFolderExists = ordnerExistiert(pfad);
	if(newFolderExists == false)
	{
		creatNewFolder();
	}
	else
	{
	}
	return;
}

//erstellt den Ordnernamen aus der auktuellen Uhrzeit (siehe gibDateiName)
string getFolderName()
{
char *wtagName[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    char *monatName[] = { "Dec", "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov" };     
	
	time_t t = time(0);
	struct tm *aT = localtime(&t);
	int tag = aT->tm_mday;
	int wtag = aT->tm_wday;
	int monat = aT->tm_mon;
	
	std::string stag = std::to_string(tag);
	std::string tagumonat = std::string() + wtagName[wtag] + "-" + monatName[monat];
	std::string folName = tagumonat + "-" + stag;
	return folName;
}


void initialisierung() 
{		
bcm2835_gpio_fsel(LED, BCM2835_GPIO_FSEL_OUTP); //Setze LED als Ausgang
bcm2835_gpio_fsel(MODI, BCM2835_GPIO_FSEL_INPT); //Setze MODI als Eingang
bcm2835_gpio_fsel(REC, BCM2835_GPIO_FSEL_INPT); //Setze REC als Eingang
bcm2835_gpio_fsel(Ex1, BCM2835_GPIO_FSEL_INPT); //Setze Ex1 als Eingang
bcm2835_gpio_fsel(Ex2, BCM2835_GPIO_FSEL_INPT); //Setze Ex2 als Eingang
bcm2835_gpio_fsel(Ex3, BCM2835_GPIO_FSEL_INPT); //Setze Ex3 als Eingang

bcm2835_gpio_set_pud(MODI, BCM2835_GPIO_PUD_UP);  //Schalter als Öffner
bcm2835_gpio_set_pud(REC, BCM2835_GPIO_PUD_UP);  //Schalter als Öffner
bcm2835_gpio_set_pud(Ex1, BCM2835_GPIO_PUD_UP);  //Schalter als Öffner
bcm2835_gpio_set_pud(Ex2, BCM2835_GPIO_PUD_UP);  //Schalter als Öffner
bcm2835_gpio_set_pud(Ex3, BCM2835_GPIO_PUD_UP);  //Schalter als Öffner
 return;
}

//liest die Schalterstellung von MODI ein
int getMODI()
{
	valMODI = !bcm2835_gpio_lev(MODI);
	
	return valMODI;
}

//liest die Schalterstellung von REC ein
int getREC() 
{
	valREC = !bcm2835_gpio_lev(REC);
	return valREC;
}

//liest die Schalterstellung von Ex1 ein
int getEx1() 
{
	valEx1 = !bcm2835_gpio_lev(Ex1);
	return valEx1;
}

//liest die Schalterstellung von Ex2 ein
int getEx2() 
{
	valEx2 = !bcm2835_gpio_lev(Ex2);
	return valEx2;
}

//liest die Schalterstellung von Ex3 ein
int getEx3() 
{
	valEx3 = !bcm2835_gpio_lev(Ex3);
	return valEx3;
}

//lässt die Status-LED 10 mal blinken
void blinken()
{
	
	for(int i=0; i<10; i++)
	{
		bcm2835_gpio_set(LED);
		delay(100);
		bcm2835_gpio_clr(LED);
		delay(50);
	}
	return;
}

//Herunterfahren der Kamera bei gleizeitgem drücken des REC Knopfes und der mittleren Funktionstaste
void herunterfahren()
{
	if(getREC() && getEx2())
	{
		cout << "Herunterfahren" << endl;
		system("killall raspivid");
		blinken();
		system("sudo shutdown -h now ");
	}
	return;
}

//beenden des Kameraprogramms bei gleichzeitigen drücken der äußeren Funktionstasten
void programmBeenden()
{
	if(getEx3() && getEx1())
	{
		aktiv = false; // Merker für while-Schleife
		system("killall raspivid");
	blinken();
	}
	return;
}
