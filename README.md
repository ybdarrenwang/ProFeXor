ProFeXor
========

ProFeXor ("Pro"sodic "Fe"ature e"X"tract"or") is an application that extracts prosodic (pitch, duration, energy) features from speech audio files, given corresponding raw pitch contours and time labels.

Along with ProFeXor, this repo contains yet another stand-alone application "Pitcher" for pitch contour processing. Note you don't have to run Pitcher before ProFeXor, because the function of Pitcher is also embedded in ProFeXor.

See the documentation or play in test folder for more information!

Platforms
---------
Linux, OS X

Requirements
------------
- g++ 4.2 or higher
- pdflatex (for make doc)
- Doxygen (for make doxygen)

Installation
------------

1. Run "make dir" to create folders
2. Run "make profexor" to compile ProFeXor into executable
3. Run "make pitcher" to complie Pitcher into executable
4. Run "make test" to copy the executables into test folder
5. Run "make" or "make all" to do all the above
6. Run "make doc" to compile PDF manual (NOTE: pdflatex needed)
7. Run "make doxygen" to create HTML pages explaining code structure (NOTE: Doxygen needed)
