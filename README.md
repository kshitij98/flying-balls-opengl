# Pacman Killer

## Usage
```
cd 20161234_Assignment1
mkdir build
cd build
cmake ..
make
./graphics_asgn1
```

## Controls

`a`: Move left

`Drag left`: Move left

`d`: Move right

`Drag Right`: Move right

`space`: Jump

`Left`: Pan left

`Scroll Left`: Pan left

`Right`: Pan right

`Scroll Right`: Pan right

`Scroll Up`: Zoom In

`Scroll Down`: Zoom Out

`Drag Up`: Jump

`q`: Quit Game


## Bonus Features

1. Infinite world: A cyclic world is implemented without any borders.
2. Infinite Levels: The ratio of different types of balls, player's health, objects in the game are changed according to the level number.
3. Scorecard: Health, points and time left is displayed on the bottom of the screen.
4. Physics: Various things are taken into consideration for the movement of the various objects on screen (Friction, Max Velocity due to magnetic attraction, Bouncing effect, ...)
5. Animations: Moving water, Fixed magnet, Fixed scorecard, ...