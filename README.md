# spectral_mesh

updated image 5-7-2020:
-phase mod and ring mod switches added to every oscillator for far more dynamic waveshaping
-greyscale mode now selectable, with alpha blending as well!
-much higher resolution meshes available so long as yr willing to trade off frame rates for resolutions!
-just looks way niftier i think


installing from the above folder to the waaave pool distro or raspbian openframeworks in general is totally doable, but pretty much only if you know what you are doing! i can't really walk yalls thru that stuffs but if yr interested in that kind of stuff you should look more into working with openframeworks and makefiles on raspbian!  for anyone who does not want to mess around with that sort of stuffs all u gots to do is get an sd card, get yr self a waaaave pool set up (either from my etsy shop or build yr own) and then get a spare sd card, download the appropriate compressed image,
and extract the .img, write the image to the sd card using a program like balena etcher, plug it in and have fun!


NTSC usb input
https://drive.google.com/open?id=1e_IAbo57TYd1vskFFTdoCAUUB4c5NESm

NTSC capture edition
https://drive.google.com/open?id=1rsyEoubYUkpVQguLnruECxdrr4xV1L5e

PAL usb input
https://drive.google.com/open?id=15Etbi2IrIaQd-4GRyeaNkX3lS3WOrtwe

PAL capture edition
https://drive.google.com/open?id=1_5KR6XMYir-fA6B5sRvLekNMWOZHc60g



known issues rn
1. frame drops when you switch mesh styles, only lasts until another midi cc is activated tho.  so until i fix this when you switch mesh styles (cc 43, 44, 42, 41) keep one hand on the luma key knob and just twist it a lil bit while pressing the buttons and you shouldn't notice much of anything.  
2. difficult to key around the thin lines in older video mixers.  i added a swtich on midi cc 45, when it is turned on the mesh resolution knob (midi cc 23) will also control line thickness, its pretty discrete but it seems to work pretty well with a wide range of video mixers (tested with roland, sony, videonics, and panasonics so far)

full manual is over here
https://andreijaycreativecoding.com/Spectral-Mesh-Manual

awesome pictoral guide to the control schemes right here thanks to to Alex Tuthill-Preus and Benjamin Roberts!
![Image description](https://github.com/ex-zee-ex/spectral_mesh/blob/master/sm_nanokontrol_guide.jpg)


check out the video_waaaves fb group for discussion and updates on this and other soft/hard ware things i make!
https://www.facebook.com/groups/440566853501750



