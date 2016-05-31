/*
 * ActionPi.cpp
 * 
 * Copyright 2016  <admin@kellerkinder>
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
 * 
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

string removeSpaces(string input);
string removeEdnl(string input);
string removeDp(string input);
string editFolder(string input);
string editDatName(string input);
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
		if(getMODI()){
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
}// Ende der Einstellungen

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

void setStandardWerte()
{
	valISO = 400;
	valHelligkeit = 50;
	valKontrast = 0;
	valSaettigung = 0;
	valVerschlusszeit = 0;
	return;
}

void videoAufnehmen()
{
	bcm2835_gpio_set(LED);
	videoAufnahme = true;
	system("killall raspivid");
	system("raspivid -o vid.h264 -t 1140000 -p '0,-10,720,480' &");		//nimmt video mit 19 Minuten auf -n = nopreview -f fullscreen "-p '0,0,1280,720' für Position und Größe des Vorschau Bildes"
	liveBildWirdAngezeigt = false;
	return;
}

int fotoAufnehmen()
{
	bcm2835_gpio_set(LED);
	system("killall raspivid");
    raspicam::RaspiCam Camera; //definiert Camera Objekt
    //Öffnet Camera 
    Camera.setISO(valISO);
    Camera.setBrightness(valHelligkeit);
    Camera.setContrast(valKontrast);
    Camera.setSaturation(valSaettigung);
    Camera.setShutterSpeed(valVerschlusszeit);
    
    //cout<<"Öffne Kamera..."<<endl;
     if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}
    sleep(1); //schlafe 1 Sekunde bis Kamera stabilisiert
    Camera.grab();	//hohle Kamera
    //reserviere Speicher
    unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];	//extrahiert Bild in rgb Format
    Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB );//aufnehmen des Bildes
    std::ofstream outFile ("bild.ppm" ,std::ios::binary ); //speichern
    outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
    outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
    delete data;	//gibt speicher wieder frei
    liveBildWirdAngezeigt = false;
    convert();
    bcm2835_gpio_clr(LED);
    return 1;
}

void liveAusgabe ()
{
	liveBildWirdAngezeigt = true;
	system("raspivid -t -0 -p '0,-10,720,480'&"); //"-p '0,0,1280,720' für Position und Größe des Vorschau Bildes"
	return;
}

void videoAbspeichern()
{
	bcm2835_gpio_clr(LED);
	std::string datname = gibDateiName();
	std::string ordnerName = getFolderName();
	pruefeNewFolder(ordnerName);
	std::string cmd_line = std::string() + " sudo MP4Box -fps 30 -add vid.h264 /media/usb0/DCIM/"+ordnerName+"/""\"" +datname+"\".mp4 ";	//Befehl zu packen von video in .mp4 container
	system( cmd_line.c_str() );	//ruft pack befehl auf
	system("killall raspivid");//beendet video aufnahme
	system("rm vid.h264");	//löscht temporäre vid.h264 video datei
	videoAufnahme = false;
	liveBildWirdAngezeigt = false;
	blinken();
	return;
}

void convert(){
	cout << "Speichern von Bild ..." << endl;
	std::string datname =  gibDateiName();
	std::string ordnerName = getFolderName();
	pruefeNewFolder(ordnerName);
	std::string comand = "convert bild.ppm /media/usb0/DCIM/"+ordnerName+ "/" + datname + ".jpg";
	const char *cComand = comand.c_str();
	system(cComand);
	std::string pfad = "fbi --noverbose -T 1 /media/usb0/DCIM/"+ordnerName+ "/" + datname + ".jpg &/";
	system("rm bild.ppm");
	rueckschau(pfad);
	sleep(4);
	system("killall fbi");
	cout << "Abspeichern fertig" << endl;
	return;
}

void rueckschau(string input)
{
	const char *cComand = input.c_str();
	system(cComand);
	return;
	
}

string gibDateiName()
{
	std::time_t t = std::time(0);
	char *ti = ctime(&t);
	std::string datname = std::string() + ti + "ctionPi";
	datname = removeSpaces(datname);
	datname = removeDp(datname);
	datname = removeEdnl(datname);
	datname = editDatName(datname);
	return datname;
}

string editDatName(string input)
{
	input.erase(0,8);
	input.erase(6,4);
	input.insert(2,"-");
	input.insert(5,"-");
	return input;
}

void creatNewFolder()
{
	std::string folderName = getFolderName();
	std::string pfad = "/media/usb0/DCIM/" + folderName;
	bool exists = ordnerExistiert(pfad);
	if(exists == false)
	{
		std::string comand = "mkdir " + pfad + "";
		const char *cComand = comand.c_str();
		system(cComand);
		cout << "Ordner erstellt." << endl;
	}
	else
	{
		cout << "Ordner existiert bereits." << endl;
	}
	return;
}

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

bool ordnerExistiert(string pfad)
{
	const char *cPfad = pfad.c_str();
	DIR *pDir;
	bool oExists = false;
	pDir = opendir(cPfad);
	if(pDir != NULL)
	{
	oExists = true;
	closedir(pDir);
	}
	return oExists; 
}

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

string getFolderName()
{
	std::time_t t = std::time(0);
	char *ti = ctime(&t);
	std::string folName = std::string() + ti;
	folName = removeSpaces(folName);
	folName = removeDp(folName);
	folName = removeEdnl(folName);
	folName = editFolder(folName);
	return folName;
}

string editFolder(string input)
{
	input.erase(8,6);
	input.erase(12,1);
	input.insert(8,"-");
	return input;
}

string removeSpaces(string input) //entfernt " "
{ 
  input.erase(std::remove(input.begin(),input.end(),' '),input.end());
  return input;
}

string removeDp(string input) //entfernt ":"
{ 
  input.erase(std::remove(input.begin(),input.end(),':'),input.end());
  return input;
}

string removeEdnl(string input) //entfernt endl
{ 
	replace(input.begin(), input.end(), '\n', 'A');
	return input;
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

int getMODI()
{
	valMODI = !bcm2835_gpio_lev(MODI);
	
	return valMODI;
}

int getREC() 
{
	valREC = !bcm2835_gpio_lev(REC);
	return valREC;
}

int getEx1() 
{
	valEx1 = !bcm2835_gpio_lev(Ex1);
	return valEx1;
}

int getEx2() 
{
	valEx2 = !bcm2835_gpio_lev(Ex2);
	return valEx2;
}

int getEx3() 
{
	valEx3 = !bcm2835_gpio_lev(Ex3);
	return valEx3;
}

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

void programmBeenden()
{
	if(getEx3() && getEx1())
	{
		aktiv = false;
		system("killall raspivid");
	blinken();
	}
	return;
}
