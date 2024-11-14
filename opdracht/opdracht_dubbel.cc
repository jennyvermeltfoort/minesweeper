
//                                                                      
// Programmeermethoden 2024
//
// http://www.liacs.leidenuniv.nl/~kosterswa/pm/dubbel.cc
// C++-programma om te oefenen in het gebruik van pointers, deel 2.
// Er moet een dubbelverbonden pointerlijst gefabriceerd worden,
// waarbij de "elementen" die met elkaar verbonden worden bestaan uit
//   1. Een int(eger)
//   2. Een pointer naar het vorige element (of nullptr)
//   3. Een pointer naar het volgende element (of nullptr)
// Schrijf de functies afdrukken (2x) en bouwlijst. Gebruik een eigen
// hulpfunctie voegvoor.
//                        
// Compiler: GNU g++
// Datum: 9 oktober 2024
// Auteur: Walter Kosters, Informatica Universiteit Leiden
//         email w.a.kosters@liacs.leidenuniv.nl
//

#include <iostream>
using namespace std;

class element {
  public:    
    element* vorige;
    int info;    
    element* volgende;    
};//element

class lijst {
  private:
    element* start;
    element* end ;
  public:
    lijst ( );
    void add_front (int value );
    void afdrukkenVA ( );
    void afdrukkenAV ( );
    void bouwlijst (unsigned int n);
};//lijst

//   +-----+      +-----+----+-----+        +-----+----+-----+      +-----+
//   |   --+----> | NULL| 10 |   --+------> |  |  | 20 | NULL| <----+--   |
//   +-----+      +-----+--+-+-----+        +-----+--+-+-----+      +-----+
//  L.ingang                    ^---------------                   L.uitgang
//       
// L.ingang (L van klasse lijst) wijst dus het eerste element uit een serie 
// elementen aan, waarbij ieder element informatie bevat (het info-veld), 
// en twee pointers: naar het vorige en volgende element;
// voor het voorbeeld hierboven zijn in totaal twee new's nodig. 

//constructor
lijst::lijst ( ) {
  start = nullptr; // of NULL
  end = nullptr; // of NULL
}//lijst::lijst

// druk lijst af, van voor naar achter
void lijst::afdrukkenVA ( ) {
  element*current = start;
  while (current != nullptr) {
    cout << current->info << endl;
    current = current->volgende;
  }

    cout << "end of list in order!" << endl;
  // TODO
}//lijst::afdrukkenVA

// druk lijst af, van achter naar voor
void lijst::afdrukkenAV ( ) {
  element*current = end;
  while (current != nullptr) {
    cout << current->info << endl;
    current = current->vorige;
  }
    cout << "end of list in reverse!" << endl;

  // TODO
}//lijst::afdrukkenAV

void lijst::add_front(int value){
  element *new_element = new element;
  new_element->info = value;

  if (start == nullptr){
    start = new_element;
    end = new_element;
  } else {
    start->vorige = new_element;
    new_element->volgende = start;
    start = new_element;
  }
}

// maak lijst met n elementen, bevattende 10,20,...,10*n
void lijst::bouwlijst (unsigned int n) {

while (n--){
  add_front(n*10);
}


  // TODO
}//lijst::bouwlijst

int main () {
  lijst L;
  L.bouwlijst (3);
  L.afdrukkenVA ( );
  L.afdrukkenAV ( );
  return 0;
}//main

