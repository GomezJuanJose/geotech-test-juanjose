# Minesweeper tool for geotech test
![showcase](showcase.gif)

## Setup

1. Copy the plugin into the project's "Plugins" folder.

2. Open the project.

_Note : If for any reason the tool does not seem to appear, then try to generate project files and reopen the project._
![alt text](image.png)
![alt text](image-1.png)

## Details
- A View-Model architecture has been applied to separate the visual aspect from the functionality and data. The View contains everything related to what the user will see and its interactions, and the Model will be in charge of processing data that will be requested by the View.

- Mines are generated when clicked to prevent loss on the first attempt

- Added a two-way scroll box

- A series of tests can be run on the Session Frontend

- References to the buttons have been stored to avoid constant casting.

- A structure called Tile2D has been created instead of using FIntegerVec2 for readability consistency since Vec2 is expressed in X and Y, and this can lead to confusion.

- A new FTileStatus structure has been created instead of using an enumerator for two reasons: more readability in the code (before the change there were lines that read `board[row][col] >= Revealed`) and for scalability (if a new tile type is introduced, all the enums would have to be reassigned).

- If you have any doubts about why I have done or designed certain things this way,  ask me : gjuanjo1999@gmail.com