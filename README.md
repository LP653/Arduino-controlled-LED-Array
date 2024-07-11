This is my project for my Arduino-controlled LED array.
It currently can only work on one array, I am working on making it scaleable to a variable number of rows and columns of arrays...arrays of arrays if you will ;)
I will upload my schematics and block diagrams soon.
The primary function of the system is to process an audio signal and display it (or its frequency spectrum) on the LED array in real-time.
Thus far, I have successfully processed audio signals at specific levels and displayed them on a panel, though I need an analog way to amplify the circuit with variable gain that depends on the volume.

The heart of the code is the displayImage function, which works like a rolling shutter, only displaying one column at a time. With a very high frequency, this effect is invisible to the eye.
