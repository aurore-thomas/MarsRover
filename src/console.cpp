#include <iostream>

#include "console.hpp"

using namespace std;

Console::Console() {}

void Console::displayMap(int width, int height, Tile **map, Orientation orientation) const
{
  cout << "Mars map:" << endl << endl;

  string separationRow = "";
  for (int x = 0; x < height; x++)
  {
    separationRow += "####";
  }
  separationRow += "#";

/*
  int a ; int b ; int c ;
  a = 4;
  b = 2;
  c = 3 ;
  cout << "Bonjour voici a = " << a << " b = " << b <<" c = " << c << endl;
  cout << "Voici c - b " << c + b << " b - c " << b - c << endl ;  
  cout << " le reste de c - b par a = " << ((c + b) % a + a) % a << " et " << ((b - c) % a + a) % a  << endl; 
*/

  for (int y = 0; y < width; y++)
  {
    cout << separationRow << endl;
    for (int x = 0; x < height; x++)
    {
      string cellContent = "";

      if (map[y][x].type == ROVER)
      {
        if (orientation == NORTH) { cellContent = " ^ "; }
        else if (orientation == EAST)  { cellContent = " > "; }
        else if (orientation == SOUTH) { cellContent = " v "; }
        else if (orientation == WEST)  { cellContent = " < "; }
      }
      else if (map[y][x].type == UNKNOWN) {
        cellContent = " ? ";
      }
      else if (map[y][x].type == EMPTY)
      {
        cellContent = " . ";
      }
      else if (map[y][x].type == OBSTACLE)
      {
        cellContent = " X ";
      }

        cout << "#" << cellContent;
    }
    cout << "#" << endl;
  }
  cout << endl;
}