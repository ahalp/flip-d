# Flip'd

Flip'd is a 2D puzzle platformer I worked on in my spare time in the summer of 2012. It's built using C++ and the SDL library. The premise is to collect all the coins in a level by creating walls and platforms while upside down, and then flipping the map to collect the coins. Coins are randomly placed on the map, and each level gets progressivly larger. 

Download avaliable here: http://www.deepkanwal.com/files/flipd.zip

## Resources

The following websites were of great help to me:

- [SDL Documentation](http://www.libsdl.org/intro.en/toc.html)   
- [Aaron's Game Programming Tutorials](http://www.aaroncox.net/tutorials/2dtutorials/index.html)   
- [Lazy Foo's Productions Tutorials](http://lazyfoo.net/SDL_tutorials/index.php)   

## Screenshots

![Flip'd vs not](http://i.imgur.com/iT0eqG4.png)

Left: A player in the normal mode. They can collect coins but cannot build blocks.   
Right: A player in flip'd mode. They can build the map but cannot collect coins.

NOTE: There's a bug in the game where the character cannot jump the appropriate distance on slower computers. This is due to a poor implementation of jumping which relies on a timed delay. 
