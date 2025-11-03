#include <iostream>

#include "console.hpp"

using namespace std;

Console::Console() {}

void Console::displayMap(int width, int height, Tile **map, Orientation orientation) const
{
  cout << "Mars map:" << endl << endl;

  string separationRow = "";
  for (int x = 0; x < width; x++)
  {
    separationRow += "####";
  }
  separationRow += "#";

  for (int x = 0; x < width; x++)
  {
    cout << separationRow << endl;
    for (int y = 0; y < height; y++)
    {
      string cellContent = "";

      if (map[x][y].type == ROVER)
      {
        if (orientation == NORTH) { cellContent = " ^ "; }
        else if (orientation == EAST)  { cellContent = " > "; }
        else if (orientation == SOUTH) { cellContent = " v "; }
        else if (orientation == WEST)  { cellContent = " < "; }
      }
      else if (map[x][y].discovered == false) {
        cellContent = " ? ";
      }
      else if (map[x][y].type == EMPTY)
      {
        cellContent = " . ";
      }
      else if (map[x][y].type == OBSTACLE)
      {
        cellContent = " X ";
      }

        cout << "#" << cellContent;
    }
    cout << "#" << endl;
  }
  cout << endl;
}