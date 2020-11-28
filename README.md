# SteamAnimation
Simple steam engine animation code for Irrlicht Engine

This example uses Irrlicht3D library, which is not included. The library can be downloaded from Source Forge. The Irrlicht3D engine (in fact, a rendering engine with just triangle-selector to character-capsule collision) is actually rarely used and the community is barely active.
No rigged model, instead separated in fixed and mobile parts moved independenty.
No collisions, a ball out of stone is pushed and rolled through bounding-box intersection and speed calculation
Inertia calculated without physics but only with string series (multiplying with the difference between treshold and actual value)
Variable frame rate (a feature of Irrlicht) command using keyboard (Irrlicht keymap)

The code is just a sample containing parts ready to adapt and use in any simple animation of a stem engine and collision effect with any library.
