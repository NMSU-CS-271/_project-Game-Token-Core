# Token Game Core

In this project, you’ll be building out some new classes to add functionality to a simple game. The basic loop of the game is that there is a token that needs to move from the top of a board to the bottom of a board. The board consists of a grid of spaces, which may be randomly generated or come from a file. Each space contains a type of terrain, which may block progress or damage (i.e., reduce the hit points of) the token. The game is won if the token successfully moves from the top of the board to the bottom; the game is lost if the token runs out of hit points (or takes too long).

The project involves a bit of following directions and a bit of creativity. You'll need to develop some components according to a specification and develop others based on something you want to add to the game. 

## What's Provided

### Game Board: `Board` class

This models a grid game board consisting of `Space`s (see below). `Board` has a few constructors, but the main one takes in file that specifies the board. 

#### Files:
````
Board.h
Board.cpp
````
#### Constructor

`Board(string filename)`

Constructs a `Board` instance using the specified `filename` (referenced in the file system from the place at which the game was launched – generally this file should just sit in the same folder as you run the game from). This file contains two integers, followed by a sequence of characters. The first integer specifies the width of the board. The second integer indicates the starting position of the token on the board on the top row. The sequence of characters specifies the types of spaces on the board: `e` = empty, `l` = lava, `w` = water, `b` = barrier. If the sequence of characters specifies an incomplete row, the row is filled in with empty spaces. 

#### Important Functions:

`pair<int int> get_start_position()`

Specifies the starting position of the `Token` on the `Board`: X and Y (zero-indexed). 

`const Space& retrieve_space(int x, int y) const`

Returns a reference to the `Space` object at the specified X and Y coordinates on the board. Throws an exception if the position is out of the map bounds.

`const int width() const;`

Returns the width of the `Board` object (note that positions are zero-indexed, so the largest X value on the board is at `width()-1`). 

`const int height const;`

Returns the height of the `Board` object (note that positions are zero-indexed, so the largest Y value on the board is at `height()-1`). 

### Game Board Space: `Space` class

This models a single grid location in a game `Board`. The `Space` has a type of terrain, specified with the enumeration `terrain_type`. This type is to be used by the `Token` to determine how it reacts to being in that space (or whether or not it can enter the space). 

`Space` objects are constructed automatically when a `Board` is constructed; you should not need to construct them.

#### Files: 
````
Board_space.h
Board_space.cpp
````

#### Important Function:

`const terrain_type& type() const`

Returns a `terrain_type` enum for the current `Space` object. Terrain types are `empty`, `barrier`, `water`, or `lava`.

### Game Token: `Token_base` base class

This models a Token -- a game piece that moves around the board (also known as a counter, game piece, meeple, mover, etc. -- [see appropriate Wikipedia article](https://en.wikipedia.org/wiki/Glossary_of_board_games#piece)). The `Token_base` class acts as the base for the classes designed and implemented in this assignment: `Token` and `Token_special`. `Token_base` has a few important attributes: 
- its location on the `Board` in X, Y coordinates (which should *always* be valid coordinates for the current `Board`);
- how close it is to losing the game (measured by hit points, which cannot be negative and cannot be more than its maximum); and 
- how much ability it has to use special moves (measured in power points, which cannot be negative and cannot be more than its maximum). 

A `Token_base` can be moved around the `Board` and is affected by the `terrain_type` of the  `Space` that it occupies. Subclasses of `Token_base` define how it can move -- a primary exercise here. By default, a `Token_base` should move one space at a time in any cardinal (up, right, down, left) direction and respond to terrain accordingly. Tokens regain a power point every time they move normally and lose one every time they use a special move. If they no power points when attempting a special move, they move normally instead. A `Token_base` is controlled by a Token Driver (see below). 

As the base class for all types of game tokens,  `Token_base` manages power points and the semantics of calling the `move` and `special_move` functions that you develop by using its `step` and `step_special` functions. Note that `step` and `step_special` are part of the external interface to `Token_base` and are the ones to be invoked by a Token Driver. `move` and `special_move` are never called directly!

`Token_base` defines an enumeration to handle cardinal movement directions: `direction`. A `direction` may be one of `up`, `right`, `down`, or `left`. Note that the game's X and Y coordinates follow the way that FLTK handles coordinates. Thus, a move `up` should decrease the Y value and `down` should increase it. Likewise, a move `right` should increase the X value and a move `left` should decrease it.

#### File:
````
Game_token_base.h
````

#### Constructor:

`Token_base(int start_hp, int start_pp, const Board &the_board)`

Constructs a `Token_base` object and sets the current and maximum hp and pp to the values specified. The position of the `Token_base` is set by getting the starting position from the passed `Board` object reference. 

#### Interface Functions: 

`void step_normal(direction dir)`

Takes a single `direction`. Increments the number of power points by one, then invokes `move(dir)`, then invokes `apply_terrain_effect()` for the current location on the `Board`. Further definition of what `move()` does is up to subclasses.

`void step_special(vector<direction> dirs)`

Takes a `vector` of `direction`s. If the number of power points is `0`, then `step_special()` works the same as `step_normal()`, but with only the first supplied direction. If there are more than one power points, then `step_special()` decrements the number of power points and invokes `move_special(dirs)`. It then invokes `apply_terrain_effect()` for the current location on the `Board`. Further definition of what `move_special()` does is up to subclasses. 

#### Pure Virtual Functions:

The following pure virtual functions must be overriden by a subclass in order to create a functional token for use in the game.

(protected) `virtual void move(direction) = 0;`

This function must be overriden to enable the token object to be moved around the board. An invocation of `move()` should cause the token's `x` and `y` values to change in accordance with one step in the specified `direction`. `move()` must not allow the token to move out of the bounds of the board (such a move should leave the token in its original location). Likewise, if the `terrain_type` of the `Space` into which it is attempting to move is a `barrier`, the move should also fail.

(protected) `virtual void move_special(vector<direction>) = 0;`

The definition of `move_special()` is designer dependent. `move_special()` takes in a `vector` of `direction`s -- how these are to be interpreted depends on the design of the subclass implementation. `move_special()` may allow the token to violate one or more game rules, but the invariant that the token's location is within the bounds of the `Board` remains in effect. 

(protected) `virtual void apply_terrain_effect(const Space::terrain_type) = 0;`

This function should update the state of the `Token_base` object as follows:
- `terrain_type::water`: reduce hit points and power points by one; 
- `terrain_type::lava`: set hit points and power points to zero; 
- `terrain_type::empty` OR any other terrain type: nothing happens. 

Note that it should never be possible for a token to enter a location with `terrain_type::barrier` unless it is a special subclass with its own rules for handling this terrain type. 

#### Virtual Function:

(protected) `virtual Shape* make_shape()`

This function supplies a `Shape*` to specify how the token should be drawn on the screen. This function can be overriden to provide an alternative look for the token. The resulting `Shape*` should be centered at a `Point` object representing X = 0 and Y = 0, regardless of the token's current location on the board (this `Shape*` will be automatically translated). Note that the drawing functionality only works for `Shape*`s that are implemented using FLTK's complex shape functionality -- if you want a special `Shape*`, you may have to develop your own `Shape` subclass. See the [FLTK documentation](https://www.fltk.org/doc-1.3/drawing.html#ssect_Complex) for more information. 

#### Accessor Functions (with obvious functionality): 

`const int hit_points() const`

`const int max_hit_points() const`

`const int power_points() const`

`const int max_power_points() const`

`const int x_pos() const`

`const int y_pos() const`

`Shape* shape()`

### Token Driver: `Driver_base` base class

This models a controller for a single token on the board. It has a `Token_base` data members and when its `next_move()` function is invoked, it should issue a single call to either the token's `step_normal()` or `step_special()` function. Subclasses of `Driver_base` provide the functionality for `next_move()`.

`Driver_base` has two `protected` members: 

`const Board &board` - a reference to the `Board` object for the current game, which can be used for making decisions about where to move the token.

`Token_base &token` - a reference to the token for the game, on which the driver operates. 

#### File:
````
Driver_base.h
````

#### Constructor:

`Driver_base(const Board &in_board, Token_base &in_token)`

Constructs a new instance of `Driver_base` from a reference to a game board and a token. The two references are stored as members of the constructed instance.

#### Pure Virtual Function:

`virtual void next_move()=0;`

Subclasses define how `next_move()` works, but it should invoke one call of either `step_normal()` or `step_special()` on the `token` member of the driver. The driver may want to make use of information from the `board` member to gather data about the spaces near to the token. 


### Scripted Token Driver: `Driver_scripted` class

`Driver_scripted` is a subclass of `Driver_base` and is provided to support testing game designs. It is intended for debugging and testing purposes only. `Driver_base` needs a text file formatted as a sequence of commands to supply to its `token`. These will be taken regardless of game state. Commands are all single character, as follows:
- `u`: normal step up.
- `r`: normal step right.
- `d`: normal step down.
- `l`: normal step left.
- `U`: special step up.
- `R`: special step right.
- `D`: special step down.
- `L`: special step left.

#### Files:
````
Driver_scripted.h
Driver_scripted.cpp
````

#### Constructor: 

`Driver_scripted(const Board &in_board, Token_base &in_token, string in_filename)`

Works as `Driver_base`'s constructor, but also takes a `string` to indicate the file name that contains the script commands. 

### Game State: `Game_state` class

Game state is used to track the overall state of the game, it holds references to the token, the board, and the driver and keeps track of the maximum number of turns and the number of elapsed turns. A game is played by repeatedly calling `Game_state`'s `run_game_loop_step()` function and checking either `active()` or `check_termination_condition()`. This is done automatically in `game_loop` and `Window_game`, so there should be no need to engage directly with `Game_state` beyond constructing it in the `main()` function.

`Game_state` includes the enumeration `game_mode`, which should be self-explanitory. 

#### Files:
````
Game_state.h
Game_state.cpp
````

#### Constructor:

`Game_state(Board &in_board, Token_base &in_token, Driver_base &in_driver, int in_max_turns)`

Constructs a new `Game_state` object for use in modeling a game. The constructor takes references to the game board, the token, the driver, and a value for the maximum number of turns. Note that `Game_state` will function normally with subclasses of `Token_base` and `Driver_base` -- it does not need modification, it should just be used. 

#### Accessor Functions (with obvious functionality): 

`const int max_turns() const`

`const int turn() const`

### Game Display Window: `Window_game` class

The game display window makes a window to show the game on the screen. The window includes a status band at the top and a grid of the spaces filling the remainder of the window. Note that exceptionally small windows may not display correctly. 

If using the existing code, only the constructor should be needed -- the provided `main()` function handles initialization and passing off the `Window_game` instance to the code that redraws the window. 

#### Files:
````
Window_game.h
Window_game.cpp
````

#### Constructor:

`Window_game(const Game_state& in_state, const Board& in_board, Token_base& in_token, int w, int h, string title)`

Constructs a new game window. Takes references to the game state, the board, and the token, as well as the width and height of the window (in pixels) and a string to display in the titlebar of the window. 

### Main Game Application

#### File:
````
game_loop.cpp
````

The main game application handles reading files passed in as arguments and constructing all relevant objects. You'll need to modify it to use your subclasses of the various classes (it will not compile by default). You'll need to identify where in `main()` you need to change the code. You should not (and should not need to) alter `State_runner`. Running the application expects up to three command-line parameters, in this order:

1. A string for the parameters filename (there is one supplied as `default.set`). The parameters file should contain three integers, the first is the starting / maximum hit points for the token; the second is the starting / maximum power points for the token; the third is the number of turns in the game. 
2. A string for the map filename. The file should be formatted as in the constructor documentation for `Board`. Two sample boards are provided: `simple.map` and `danger.map`. 
3. A string for the command filename, which can be used for a `Driver_scripted` if needed. The file should be formatted as specified in the `Driver_scripted` documentation. Two sample commands are provided that can go with the `danger.map` -- `dodge_danger.cmd` (which will drive the token around the dangerous parts of the map) and `crash_danger.cmd` (which will run the token through the dangerous parts of the map). 

## What To Do

In this assignment, you'll need to implement one class and design and implement two others. You'll implement, according to the specification below, a subclass of `Token_base` -- `Token` -- that is very simple. Then you'll design and implement a subclass of your `Token` -- `Token_special` that has some kind of special movement rules. Finally, you'll design and implement a subclass of `Driver_base` (or `Driver_scripted` -- your choice) that is used to move your `Token_special` based on the map. There is no grading on the performance of your tokens! Just make sure you design and implement the required things. 

Start by designing, sans code, your classes, then develop implementations. Details on each of the three classes are below.

Once you have started working on them, you can test them through the `main()` in `game_loop.cpp` (or build your own test harnesses). You'll have to make some edits to it to make sure that it's using your classes and you can change the parameters, maps, and/or scripts to suit your needs. Note that once you get started, you'll also need to update the included `Makefile` to account for compiling your source code. 

### A Plain Token: `Token` class

This is a basic token, with no special abilities. It can only move one space at a time. This class should subclass `Token_base` and be constructable with a number of hit points and a number of power points. It should take in a reference to a `Board` object, which will supply the starting X and Y position of the `Token`. `Token` needs to override and implement the pure virtual functions from `Token_base`: `move(direction)`, `move_special(vector<direction>)`, and `apply_terrain_effect(terrain_type)`. 

The specifications for `move()` and `apply_terrain_effect()` are provided in `Token_base`. For `move_special()`, which takes a `vector` of `direction`'s, a `Token` should just call its `move()` function with the first element of the `vector`.

#### Files:
````
Game_token.h
Game_token.cpp
````

### A Special Token: `Token_special` class

The `Token_special` should extend `Token` and it should be an implementation of YOUR design for a special token -- a token that has a special ability. It should be the same as the `Token` in most ways, but it should have a special move! How you design the move is up to you. When you build your `Token_special`, develop an implementation of `move_special(vector<direction>)`; if you want your special token to look different from a default token, you can also develop your own implementation of `make_shape()`.  

Note that using the special move costs power points, so your token won't be able to use it every turn. What you do with the special move is up to you. Note that it takes in a `vector<direction>` rather than a single direction. Why it does this is also up to you (the main reason it was provided was in case you wanted to offer the ability to move diagonally, but you could use it for other reasons as well). Note that `move_special()` must still adhere to the invariant that the token is somewhere in the bounds of the board and NOT entering a barrier space! Document what your special token does and why. 

#### Files:
````
Game_token_special.h
Game_token_special.cpp
````

### A Special Token Driver: `Driver_special` class

The `Driver_special` is a driver to issue commands to your `Token_special` to get it through the game board; it should be designed with this in mind. The special driver will be called each game turn automatically by invoking `next_move()`. The driver gets created with a reference to the board object and the token object -- thus `next_move()` can scan spaces on the board near to the token to make decisions about how to move. 

#### Files:
````
Driver_special.h
Driver_special.cpp
````


