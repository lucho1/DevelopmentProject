## DayBreak

DayBreak is a platformer game done by two students for Development Subject in Videogame Design and Development carreer of CITM at Universitat Politècnica
de Catalunya.

The game is about an alien that must escape from his planet because humans have arrived to conquer and started to destroy everything. But
it's not that easy, in his path he will find enemies that will try to kill him and he will have to find his way out from places that
have active/unactive platforms that obstaculize the path to success.

DayBreak is done with Tiled program, has 2 levels and save/load support. Also, the game supports pathfinding for enemies.

The game is licensed under an MIT License.

![](https://github.com/lucho1/DevelopmentProject/blob/master/docs/Game%20Photo.png)

### Download the game [here](https://github.com/lucho1/DevelopmentProject/releases)

### TRUE DOWNLOAD [here](https://github.com/lucho1/DevelopmentProject/blob/master/docs/DayBreak_v1.zip)
#### Github Repo: [DayBreak](https://github.com/lucho1/DevelopmentProject)

## Developers

![](insert photo)

Lucho Suaya

* IA, pathfinding & FPS/time control
* [Github](https://github.com/lucho1) @lucho1

Roger Leon

* Art, Music, Levels, Game Design, Art 
* [Github](rleonborras) @rleonborras

## Game Main Core

The structure was made with modules that manages different elements of the game. Each one has, appart from their own, common methods for
awake, preupdate, update, postupdte and cleanup. App is a module that manages all the others and it calls all that methods making
each module work when it must.

All regarding to game metadata (paths of assets, character and enemies characteristics...) are loaded with XML files to avoid hardcoding,
avoid confusing code and build an easier work flow.

![](https://github.com/lucho1/DevelopmentProject/blob/master/docs/UML.png)

### Innovations
* Platforms that must be activated or deactivated to pass the level
* Textures Flipping
* Player characteristics loaded from XML file.
* Intro Screen
* Player and enemies shooting
* Music made by us from other base music

## Gameplay & Instructions

You must pass through each level and get to the door that will take you out!

Move left and right with A and D keys and jump with W.

To pass each level, there are some platforms that are unactive. To activate them and be able to pass over them, you must press a button!
Those platforms can be also obstacles that must be deactivate by passing through the ray in order to pass the level.

From main menu, start with Enter. Quit the game with ESC key.

* _Settings for Debug purposes_

F1 - Start from level 1

F2 - Iterate between screens/levels

F5 - Save the Current state

F6 - Load previous state

F9 - View Colliders/Logic and Enemies Pathfinding

F10- God Mode

## License

MIT License

Copyright (c) 2017 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
