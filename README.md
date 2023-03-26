# Snake-game-IS1200
The classic game snake created in C on a ChipKIT Uno32 made in the course Computer Hardware Engineering IS1200 during spring 2022.


The project was done on a ChipKIT Uno32 development board togheter with a ChipKIT Basic I/O shield. I made a version of the game snake where you controlled whether the snake went left or right with two buttons and ate apples displayed as a white rectangle on the screen. The more apples you ate the more points you got and the faster the snake moved. The game could be played in singleplayer or with 2 players playing on the same chipkit. It also had a scoreboard where you could store your 10 highest scores and give names to each of these runs. There were also difficulty levels that added obstacles on the map to amke the game harder. 

The game can be run by simply downloading the zip folder and extracting the files. Then download the MCB32 Toolchain. Connect the ChipKIT to your computer, open MSYS Shell and run the command ". /opt/mcb32tools/environment". Then change directory to the game folder and run the command "make" to compile the files. Then finally enter the command "make install TTYDEV=/dev/ttyS4" to install it onto the chipkit and now the game is ready. 

More information about the project can be found here: https://canvas.kth.se/courses/31596/pages/mini-project

