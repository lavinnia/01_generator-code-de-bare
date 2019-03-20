#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <windows.h>
using namespace std;

void L(int valoare, int bin[], int poz);
void G(int valoare, int bin[], int poz);
void R(int valoare, int bin[], int poz);


int main()
{
	char response;
	char codDat[10];
	string denumire;	
	int flag, lungime, i;
	do{ 
		do{
			flag =0;
			//codul si denumirea produsului , variabile tip sir de caractere
			cout << "Introduceti denumirea produsului (max. 20 caractere): ";
			cin >> denumire;
			cout << "Introduceti codul produsului (max. 9 cifre, fara spatii): ";
			cin >> codDat;

			lungime = strlen(codDat);

			// Verificare. Numar toate caracterele bune folosind un contor.
			i = 0;
			while(codDat[i] >= '0' && codDat[i] <= '9')
				i++;
			
			if(i != lungime)
			{	cin.get();
			cout << "Cod eronat: " << codDat << endl;				
			cout<<"Try again, please!"<<endl<<endl;
			flag =1;
			}

			else if(lungime>9)
			{	cin.get();
			cout << "Check again, too much numbers. There are "<<i<<" numbers : " << codDat << endl;			
			cout<<"Try again, please!"<<endl<<endl;
			flag =1;
			}
		}while(flag != 0);

		char sirSapte[9] = ""; // sirul initial este vid
		if(lungime < 9)
		{ 	int j;
		for(j=0; j<9-lungime; j++)
			sirSapte[j] = '7';
		// Limitez sirSapte la dreapta adaugandu-i un caracter '\0'
		sirSapte[j] = '\0';		
		}



		char codRO[] = "544";//594
		char ean13[13]; // 13 caractere, 12+1
		strcpy(ean13, codRO);
		strcat(ean13, sirSapte);
		strcat(ean13, codDat);

		//// Verific pana aici
		//cin.get();	
		//cout << "Primele 12 car. sunt : " << ean13 << endl;


		// Convertim caracterele in valori numerice intregi (int)
		int nean13[13]; // Pentru rezultate
		for(int k=0; k<12; k++)
			nean13[k] = ean13[k]-'0';


		int S = 0, s1 = 0, s2 =0;

		for(int i=0; i<12; i=i+2)
		{
			s1 = s1 + nean13[i];
			s2 = s2 + nean13[i+1];
		}
		S = s1 + 3*s2;
		nean13[12] = 10 - S%10; // Cifra de control

		cin.get();
		cout<<"Cifra de control: "<<nean13[12]<<endl;
		cout<<"Codul final :  ";
		for(int k=0;k<13;k++)
			cout<< nean13[k];
		cout<<endl;
		cout<<endl;

		int b[95];
		// Se codifica inceputul: 101
		b[0]=b[2]=1;
		b[1]=0;
		// Se codifica prima parte a codului (6 caractere, nean13[1] la	nean13[6]):
		L(nean13[1], b, 3); // Primul, nean13[1]. Se apeleaza functia L
		G(nean13[2], b, 10); // Car. 2, se apeleaza functia G
		G(nean13[3], b, 17); // Car. 3, se apeleaza functia G
		L(nean13[4], b, 24); // Car. 4, se apeleaza functia L
		L(nean13[5], b, 31); // Car. 5, se apeleaza functia L
		G(nean13[6], b, 38); // Car. 6, se apeleaza functia G
		// Se codifica zona de separare din mijloc, 01010:
		b[45]=b[47]=b[49]=0;
		b[46]=b[48]=1;
		// Se codifica partea a doua a codului (nean13[7] la nean13[12]).
		// Pentru toate se apeleaz? func?ia R():
		for(i=0; i<6; i++)
			R(nean13[7+i], b, 50+i*7);
		// Se codifica partea finala a codului, 101:
		b[92] = b[94] = 1;
		b[93] = 0;



		fstream cod;
		denumire=denumire+".html";
		cod.open(denumire, ios::out);
		cod << "<!DOCTYPE html>" << endl;
		cod << "<html>" << endl;
		cod << "<body>" << endl;
		cod << "<svg height=\"100\" width=\"200\">" << endl;
		// Doi pixeli pentru fiecare bara
		int pozx = 10; // De la acest x incep trasarea

		int pozitie1 = 3;
		int pozitie2 = 50;
		int index=0;
		for(i=0; i<95; i++)
		{
			if(b[i] == 1) // Se traseaza o linie
			{if(i<3 || (i>=45 && i<=48) || (i>=92 && i<=94) )
			cod << "<line x1=\"" << pozx
			<< "\" y1=\"20\" x2=\"" << pozx
			<< "\" y2=\"60\" style=\"stroke:rgb(0,0,0); stroke-width:2\" />"			
			<< endl;

			else
				cod << "<line x1=\"" << pozx
				<< "\" y1=\"20\" x2=\"" << pozx
				<< "\" y2=\"50\" style=\"stroke:rgb(0,0,0); stroke-width:2\" />"
				<< endl;
			}
			 
			if(i==1){
				cod<<"<text x=\"0\" y=\"62\" fill=\"black\" style='font-weight: bold'>"<<nean13[index]<<"</text>";
				index++;
			}
			if(i == pozitie1 && pozitie1<=38 ){
				cod<<"<text x=\"" << pozx<<"\" y=\"62\" fill=\"black\" style='font-weight: bold'>"<<nean13[index]<<"</text>";
				pozitie1+=7;
				index++;
			}
			if(i == pozitie2 && pozitie2<=85 ){
				cod<<"<text x=\"" <<pozx<<"\" y=\"62\" fill=\"black\" style='font-weight: bold'>"<<nean13[index]<<"</text>";
				pozitie2+=7;
				index++;
			}
			pozx = pozx + 2;// Avans cu 2px, indiferent daca s-a trasat sau nu
		}
		

		cod << "</svg>" << endl;
		cod << "</body>" << endl;
		cod << "</html>" << endl;
		cod.close();

		//char htmlFile[] = "D:\\DSUsers\\bozdogl\\MASTER\\ProiectIntegrator";
		//char htmlFile[] = "D:\\DSUsers\\bozdogl\\MASTER\\ProiectIntegrator\\GeneratorCodeBar\GeneratorCodeBar";
		char htmlFile[] = "";
		const char *denum = denumire.c_str();

		char ceva[512];
		sprintf(ceva,"%s%s",htmlFile,denum);

		system(ceva);

		cout<<"\n\nDo you want to continue (y/n)  ?"<<endl;
		cin>>response;

	}while(tolower(response) == 'y');
	if(tolower(response) == 'n')	
		cout<<"  ***  Bye!  ***"<<endl;

	cin.get();
	return 0;

}



void L(int valoare, int bin[], int poz)
{
	switch(valoare)
	{
	case 0: // 0001101
		bin[poz]=bin[poz+1]=bin[poz+2]=bin[poz+5]=0;
		bin[poz+3]=bin[poz+4]=bin[poz+6]=1;
		break;
	case 1: // 0011001
		bin[poz]=bin[poz+1]=bin[poz+4]=bin[poz+5]=0;
		bin[poz+2]=bin[poz+3]=bin[poz+6]=1;
		break;
	case 2: // 0010011
		bin[poz]=bin[poz+1]=bin[poz+3]=bin[poz+4]=0;
		bin[poz+2]=bin[poz+5]=bin[poz+6]=1;
		break;
	case 3: // 0111101
		bin[poz]=bin[poz+5]=0;
		bin[poz+1]=bin[poz+2]=bin[poz+3]=bin[poz+4]=bin[poz+6]=1;
		break;
	case 4: // 0100011
		bin[poz]=bin[poz+2]=bin[poz+3]=bin[poz+4]=0;
		bin[poz+1]=bin[poz+5]=bin[poz+6]=1;
		break;
	case 5: // 0110001
		bin[poz]=bin[poz+3]=bin[poz+4]=bin[poz+5]=0;
		bin[poz+1]=bin[poz+2]=bin[poz+6]=1;
		break;
	case 6: // 0101111
		bin[poz]=bin[poz+2]=0;
		bin[poz+1]=bin[poz+3]=bin[poz+4]=bin[poz+5]=bin[poz+6]=1;
		break;
	case 7: // 0111011
		bin[poz]=bin[poz+4]=0;
		bin[poz+1]=bin[poz+2]=bin[poz+3]=bin[poz+5]=bin[poz+6]=1;
		break;
	case 8: // 0110111
		bin[poz]=bin[poz+3]=0;
		bin[poz+1]=bin[poz+2]=bin[poz+4]=bin[poz+5]=bin[poz+6]=1;
		break;
	case 9: // 0001011
		bin[poz]=bin[poz+1]=bin[poz+2]=bin[poz+4]=0;
		bin[poz+3]=bin[poz+5]=bin[poz+6]=1;
		break;
	}
}

void G(int valoare, int bin[], int poz)
{
	switch(valoare)
	{
	case 0: // 0100111
		bin[poz]=bin[poz+2]=bin[poz+3]=0;
		bin[poz+1]=bin[poz+4]=bin[poz+5]=bin[poz+6]=1;
		break;
	case 1: // 0110011
		bin[poz]=bin[poz+3]=bin[poz+4]=0;
		bin[poz+1]=bin[poz+2]=bin[poz+5]=bin[poz+6]=1;
		break;
	case 2: // 0011011
		bin[poz]=bin[poz+1]=bin[poz+4]=0;
		bin[poz+2]=bin[poz+3]=bin[poz+5]=bin[poz+6]=1;
		break;
	case 3: // 0100001
		bin[poz]=bin[poz+2]=bin[poz+3]=bin[poz+4]=bin[poz+5]=0;
		bin[poz+1]=bin[poz+6]=1;
		break;
	case 4: // 0011101
		bin[poz]=bin[poz+1]=bin[poz+5]=0;
		bin[poz+2]=bin[poz+3]=bin[poz+4]=bin[poz+6]=1;
		break;
	case 5: // 0111001
		bin[poz]=bin[poz+4]=bin[poz+5]=0;
		bin[poz+1]=bin[poz+2]=bin[poz+3]=bin[poz+6]=1;
		break;
	case 6: // 0000101
		bin[poz]=bin[poz+1]=bin[poz+2]=bin[poz+3]=bin[poz+5]=0;
		bin[poz+4]=bin[poz+6]=1;
		break;
	case 7: // 0010001
		bin[poz]=bin[poz+1]=bin[poz+3]=bin[poz+4]=bin[poz+5]=0;
		bin[poz+2]=bin[poz+6]=1;
		break;
	case 8: // 0001001
		bin[poz]=bin[poz+1]=bin[poz+2]=bin[poz+4]=bin[poz+5]=0;
		bin[poz+3]=bin[poz+6]=1;
		break;
	case 9: // 0010111
		bin[poz]=bin[poz+1]=bin[poz+3]=0;
		bin[poz+2]=bin[poz+4]=bin[poz+5]=bin[poz+6]=1;
		break;
	}
}

void R(int valoare, int bin[], int poz)
{
	switch(valoare)
	{
	case 0: // 1110010
		bin[poz+3]=bin[poz+4]=bin[poz+6]=0;
		bin[poz]=bin[poz+1]=bin[poz+2]=bin[poz+5]=1;
		break;
	case 1: // 1100110
		bin[poz+2]=bin[poz+3]=bin[poz+6]=0;
		bin[poz]=bin[poz+1]=bin[poz+4]=bin[poz+5]=1;
		break;
	case 2: // 1101100
		bin[poz+2]=bin[poz+5]=bin[poz+6]=0;
		bin[poz]=bin[poz+1]=bin[poz+3]=bin[poz+4]=1;
		break;
	case 3: // 1000010
		bin[poz+1]=bin[poz+2]=bin[poz+3]=bin[poz+4]=bin[poz+6]=0;
		bin[poz]=bin[poz+5]=1;
		break;
	case 4: // 1011100
		bin[poz+1]=bin[poz+5]=bin[poz+6]=0;
		bin[poz]=bin[poz+2]=bin[poz+3]=bin[poz+4]=1;
		break;
	case 5: // 1001110
		bin[poz+1]=bin[poz+2]=bin[poz+6]=0;
		bin[poz]=bin[poz+3]=bin[poz+4]=bin[poz+5]=1;
		break;
	case 6: // 1010000
		bin[poz+1]=bin[poz+3]=bin[poz+4]=bin[poz+5]=bin[poz+6]=0;
		bin[poz]=bin[poz+2]=1;
		break;
	case 7: // 1000100
		bin[poz+1]=bin[poz+2]=bin[poz+3]=bin[poz+5]=bin[poz+6]=0;
		bin[poz]=bin[poz+4]=1;
		break;
	case 8: // 1001000
		bin[poz+1]=bin[poz+2]=bin[poz+4]=bin[poz+5]=bin[poz+6]=0;
		bin[poz]=bin[poz+3]=1;
		break;
	case 9: // 1110100
		bin[poz+3]=bin[poz+5]=bin[poz+6]=0;
		bin[poz]=bin[poz+1]=bin[poz+2]=bin[poz+4]=1;
		break;
	}
}