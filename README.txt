Basic instructions for writing a script is given below:

Each instruction should begin on a new line. Each [] block should be seperated by a space. Examples are given below

Mouse input instructions:
Mouse: [M/K index(1 character)] [Click/No Click/Hold/Release (1/0/2/3)(1 character)] [Left/Right/Middle (1 character)] [horizontal movement(4 characters)] [vertical movement(4 characters)] [number of steps(3 characters)] [timestep(3 characters)] \n

e.g From the below line ( Mouse No Click Empty(Due to no click) 100 horizontal 100 vertical 5 steps 500ms delay)
M 0 _ 0100 0100 005 050 \n

Keyboard input instructions:
Keyboard: [M/K index(1 character)] [Write/Press/Release(1 character)] [alphanumeric indicator(1 character)] [key(9 characters)] [number of steps(3 characters)] [timestep(3 characters)] \n
e.g From the below line ( Keyboard Write alphanumeric 'A' 5 times 1 ms delay)

K W 1 00000000A 005 001 \n