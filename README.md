Flip'd is a 2D puzzle platformer built using C++ and the SDL library. The premise is to collect all the coins in a level by creating walls and platforms while upside down, and then flipping the map to collect the coins. Coins are randomly placed on the map, and each level gets progressivly larger. 
Note that I am not maintaining this game, it was just a fun project to get myself familiarized with SDL.

The following websites were of great help to me:

SDL Documentation: http://www.libsdl.org/intro.en/toc.html

Aaron's Game Programming Tutorials: http://www.aaroncox.net/tutorials/2dtutorials/index.html

Lazy Foo's Productions Tutorials: http://lazyfoo.net/SDL_tutorials/index.php

NOTE: There's a bug in the game where the character cannot jump the appropriate distance on slower computers. This is due to a poor implementation of jumping which relies on a timed delay. 
