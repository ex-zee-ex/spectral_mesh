# spectral_mesh

updated image: now supports keyboard controls and greater ranges for x and y lfos

installing from the above folder to the waaave pool distro or raspbian openframeworks in general is totally doable, but pretty much only if you know what you are doing! i can't really walk yalls thru that stuffs but if yr interested in that kind of stuff you should look more into working with openframeworks and makefiles on raspbian!  for anyone who does not want to mess around with that sort of stuffs all u gots to do is get an sd card, get yr self a waaaave pool set up (either from my etsy shop or build yr own) and then get a spare sd card, download this image

https://drive.google.com/open?id=17F22vRG6Px7G_FY8o7H4LyN7yDTSa4Ae

and extract the .img, write the image to the sd card, plug it in and have fun!


known issues rn
1. frame drops when you switch mesh styles, only lasts until another midi cc is activated tho.  so until i fix this when you switch mesh styles (cc 43, 44, 42, 41) keep one hand on the luma key knob and just twist it a lil bit while pressing the buttons and you shouldn't notice much of anything.  
2. difficult to key around the thin lines in older video mixers.  i added a swtich on midi cc 45, when it is turned on the mesh resolution knob (midi cc 23) will also control line thickness, its pretty discrete but it seems to work pretty well with a wide range of video mixers (tested with roland, sony, videonics, and panasonics so far)
3. i didn't post a manual yet!  yo i'll get this up before the first ones ship most definitely, in the meanwhile 
ccs 16 lumakey
ccs 17 bright-x displace
ccs 18 bright-y displace
19 zlfo period
20 xlfo period
21 ylfo period
22 global zoom
23 mesh resolution (all the way counter clockwise is maximum resolution, its not real time tho you have to hit one of the mesh style buttons to recalculate the resolution)/line thickness
120/121 x y center positions, s button on -> x y global positions, m button on x y rotations
122 zlfo frequency, buttons change slider ranges
123 zlfo amplitude, buttons change waveshape
124 xlfo frequency, buttons change slider ranges
125 xlfo amplitude, buttons change waveshape
126 ylfo frequency, buttons change slider ranges
127 ylfo amplitude, buttons change waveshape


46-different xy lfo interaction styles
61-brightness to xy displace inversion
59-invert rgb global
62-switch lumakey black to lumakey white


