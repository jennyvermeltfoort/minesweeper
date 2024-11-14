
//                                                                      
// Programmeermethoden 2024
//
// http://www.liacs.leidenuniv.nl/~kosterswa/pm/oefpoint.cc
// C++-programma om te oefenen in het gebruik van pointers.
// Er moet eerst een enkelverbonden pointerlijst gefabriceerd worden,
// waarbij de "vakjes" die met elkaar verbonden worden bestaan uit
//   1. Een int(eger)
//   2. Een pointer naar een int(eger) (of nullptr)
//   3. Een pointer naar het volgende vakje (of nullptr)
// Schrijf de functies Afdrukken, Toevoegen en Verwijderen   
// --- in deze volgorde.  
// Er is ook een uitwerking van dit programma beschikbaar: 
//   http://www.liacs.leidenuniv.nl/~kosterswa/pm/pointer.cc
//
// Doe hierna
//   http://www.liacs.leidenuniv.nl/~kosterswa/pm/dubbel.cc
//                        
// Compiler: GNU g++
// Datum: 9 oktober 2024
// Auteur: Walter Kosters, Informatica Universiteit Leiden
//         e-mail w.a.kosters@liacs.leidenuniv.nl
//

#include <iostream>
using namespace std;

// het soort vakje waar het om draait:
class vakje {              // een struct mag ook
  public:                  
    int info;    
    int* andere;
    vakje* volgende;       
};//vakje

//
//   +-----+        +----+----+-----+        +----+----+-----+
//   |   --+------> | 34 |  | |   --+------> | 56 |  | | null|ptr
//   +-----+        +----+--+-+-----+        +----+--+-+-----+
//  ingang                  |                        |
//                          V                        V
//                       +----+                   +----+
//                       | 23 |                   | 18 |
//                       +----+                   +----+
//       
// ingang wijst dus een vakje uit een lijst vakjes aan
// waarbij ieder vakje informatie bevat (het info-veld), 
// een pointer naar een int(eger) (het andere-veld),
// en een pointer naar het volgende vakje; om zulke 
// structuurtjes te maken is doorgaans in totaal steeds 
// twee maal new nodig, voor het voorbeeld hierboven in totaal vier
// Een voorbeeld: het getal 34 (in het eerste vakje)
// wordt afgedrukt via:  
//   cout << ingang->info << endl;
// En het getal 18 (toegankelijk door het tweede vakje) via:
//   cout << *(ingang->volgende->andere) << endl;
// En een pointer hulp (vakje* hulp) gaat naar het vakje
// met onder andere 56 erin wijzen via:
//   hulp = ingang->volgende;
//

// ************************************************************************

void Afdrukken (vakje* current) {
// druk lijst met ingang als ingang af 
  cout << "Lijst afdrukken ..." << endl;
    while (current!=nullptr)
    {
        cout << +current->info << endl;
        cout << +*current->andere << endl;
        current = current->volgende;
    }

  //
  //   XXXXXX
  //     XX
  //     XX
  //   XXXXXX
  //
  
}//Afdrukken
 
void Toevoegen (vakje* & ingang, int een, int twee) {
// voeg vakje met getallen een en twee erin vooraan lijst ingang toe
// preciezer: nieuw vakje met getal een en een POINTER naar een     
// nieuwe int met getal twee erin                                   
// mooier: als bijvoorbeeld twee = -1, dan p->andere nullptr maken
  cout << "Voeg een nieuw vakje toe ..." << endl;
  vakje*current = new vakje;
  if (ingang == nullptr) {
    ingang = current;
  } else {
    current->volgende = ingang;
    ingang = current;
  }

  ingang->info = een;
  if (twee < 0){
    ingang->andere = nullptr;
  } else {
    ingang->andere = new int;
    *ingang->andere = twee;
  }
  

  //
  //   XXXXXX XXXXXX
  //     XX     XX
  //     XX     XX
  //   XXXXXX XXXXXX
  //
  
}//Toevoegen

void Verwijderen (vakje*& ingang) {
// gooi eerste vakje van lijst ingang weg als ingang niet nullptr is
  cout << "Verwijder een vakje ..." << endl;

  if (ingang == nullptr) {
    return;
  }

  vakje *current = ingang->volgende;
  delete ingang;
  ingang =current;
    
  //
  //   XXXXXX XXXXXX XXXXXX
  //     XX     XX     XX
  //     XX     XX     XX
  //   XXXXXX XXXXXX XXXXXX
  //
  
}//Verwijderen

void LeesInGetallen (int & een, int & twee) {
// lees integers een en twee in
  cout << "Eerste getal svp .. ";
  cin >> een;
  cout << "Tweede getal svp .. ";
  cin >> twee;
}//LeesInGetallen

// *********************************************************************

int main ( ) {

  vakje* ingang = nullptr; // ingang van de op te bouwen lijst (of NULL)
  char keuze;              // wat wil de gebruiker?
  int een, twee;           // toe te voegen getallen

  do {
    cout << "Kies uit: [s]toppen, [t]oevoegen,   " << endl
         << "          [a]fdrukken, [v]erwijderen" << endl
         << "Uw keuze: ";
    cin >> keuze;
    switch (keuze) {
      case 's': case 'S':
        cout << "Dat was het dan ..." << endl; 
        break;
      case 't': case 'T':
        LeesInGetallen (een,twee);
        Toevoegen (ingang,een,twee);
        break;
      case 'a': case 'A':
        Afdrukken (ingang);
        break;
      case 'v': case 'V':
        Verwijderen (ingang);
        break; 
      default:
        cout << "Niet toegestane menukeuze ..." << endl;
    }//switch
  }//do
  while ( ! ( keuze == 's' ) && ! ( keuze == 'S' ) );

  // while ( ingang != nullptr )  // netjes opruimen
  //   Verwijderen (ingang);

  return 0;

}//main
