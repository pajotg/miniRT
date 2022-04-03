# miniRT, A project where you code a raytracer from scratch in C<br>


## How to use:
First: Run make (use bonus target so it can do the fancy stuff)<br>
Second: ./miniRT [file] {args}<br>

It ***should*** work on both mac and linux

The file is the scene it needs to render, a bunch of example scenes are given in the scenes directory<br>
The arguments could be: **--save**, **--no-res-cap** and **--save-on-exit**
* **--save** makes the window not open, and render out a max quality (defined in scene file) and saves it to screenshot.bmp
* **--no-res-cap** removes the resolution cap, It wont clamp to your screen size
* **--save-on-exit** saves the last frame to screenshot.bmp while still opening the window

The controlls themselves are hopefully pretty simple:<br>
* **WASD** to move your camera around
* **Q** to move your camera up
* **E** to move your camera down
* **Left** click somewhere to look in that direction (Makes that pixel the new center)
* **R/F/P** to increase/decrease/auto-calculate the white point
* **T** to switch to the next camera
* **Esc** to quit

## Frame types (in logs)
There are 4 different frame types:
1. **FI** - First: A 1 sample per pixel render
2. **AA** - Anti-Aliasing: Run after FI, a [N] sample per pixel where there are edges determined by a sobel filter, render on top of the old frame, N is defined in the scene file
3. **NR** - Noise-Reduction: Run after all AA frames, detects areas with high noise (noise is how much a pixel has changed since the last frame) and renders [N] samples there, much more efficient than AA for GI enabled scenes, and pretty good for non-GI scenes too, N again is defined in the scene file
4. **DI** - Dirty: A frame where the camera has moved, and will be completely replaced by a FI frame next frame, Restarting from the top does not look good, you can't really see where you are going

## The scene file format:
Every single like is a new instruction<br>
They all start with a keyword, and then some arguments, Capitalized keywords can only
appear once<br>
Lines starting with '#' are comments.<br>
You can have:
* **R** [width] [height] - The resolution
* **A** [hdrcolor] - The ambiant color, ratio is a multiplier, can go above 1 for bright stuff
* **AA** [samples] ... - The Anti-Aliasing sample count, Bonus, can have multiple numbers, so 'AA 2 4 8' will render a AA frame with 2, then with 4, and then with 8
* **NR** [samples] ... - The Noise-Rediction sample count, Bonus
* **GI** - Enables Global-Illumination, Bonus
* **c** [transform] [fov] - Camera
* **l** [location] [hdrcolor] - Point Light
* **dl** [lookdir] [hdrcolor] - Directional Light, Bonus, Light goes in the direction of [lookdir]
* **sp** [location] [size] [material] - Sphere
* **pl** [transform] [material] - Plane
* **cy** [transform] [diameter] [height] [material] - Cylinder
* **sq** [transform] [size] [material] - Square
* **cu** [transform] [size] [material] - Cube, Bonus
* **tr** [location] [location] [location] [material] - Triangle, Bonus, Makes a triangle between the 3 given locations
* **obj** [transform] [obj_path] [material] - Obj file, Bonus, path is relative to scene file

Ive used things like [transform] and [color] and such above, they are written like this:
* **location**: [x],[y],[z]
* **lookdir**: [x],[y],[z]
* **transform**: [location] [lookdir]
* **color**: [r],[g],[b]
* **hdrcolor**: [ratio] [color]
* **material**: its either:
	*	a plain [color], diffuse only
	*	or Defined by a tree like structure:
		*	**D** [color] - Diffuse
		*	**E** [hdrcolor] - Emissive
		*	**M** [ratio] [material] [material] - Mix
		*	**A** [material] [material] - Add
		*	**C** [size] [color] [material] - Checkerboard
		*	**P** [color] - Perfect mirror
		*	**S** [color] [pow] - Specular

	#### examples:
	* 255,255,255 - Plain color
	* A D 127,63,63 S 255,255,255 100 - Diffuse + Specular
	* A C 1 127,127,127 C 0.5 127,127,127 C 0.25 127,127,127 D 127,127,127 S 255,255,255 75 - A bunch of checkerboards of different sizes + specular
	* M 0.5 C 1 127,127,127 D 255,0,0 D 0,255,0 - %50 red checkered, %50 green diffuse

Anything not specified is just a number, so [fov], [pow] and [size] are just numbers

## Extra things added in this project
1. Anti-Alliasing / Noise reduction
2. Threading
3. HDR colors, after rendering is done converted to 0-255 RGB values via reindard mapping
4. Fancy materials
5. Directional lights
6. .obj file parings, you can render .obj's! (see deermageddon scene file)
7. BVH (Bounding volume hierarchy) purely added to speed up rendering the deermageddon scene, might take a while to generate though, but rendering is much, much faster

## Final thoughts
All in all, it was a fun project, untill the norm came along<br>
Now its all ugly, where are my empty lines and comments inside functions?<br>
Why must i declare a variables on the top of the function when i only use it at the bottom?