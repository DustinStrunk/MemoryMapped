# reading_comparsion
Compares Windows' memory mapped file read speed versus C++ std::ifstream library.

### Setup
  - Microsoft Visual Studio IDE including compiler
  - Windows 10 (Education Edition, x64)
  - SSD with 16 GB of RAM

###Tests
In order to get a rough idea of the speed comparison between **memory mapped files** and **std::ifstream** I setup a project which would simply just read varying sizes of text files. These files were generated from a slightly modified Java project located [here](https://github.com/SL5R0/GenFile).

I generated 200 unicode files varying in size from 50KB to 10,000KB, with an increment of 50 KB, leading to 200 files. I wrote an extremely barebones class to handle the memory mapping API of Windows and then wrote a basic std::ifstream implementation. It is important to note I did no optimization to either, such as increasing the read buffer size of the ifstream object. My code runs the test 10 times and averages the times, then outputs to seperate files.

##Testing Yourself
There are a few things to note when doing the test yourself.
1. Due to the fact that the data must be in C:\data\ you often times may need administrator rights.
2. The data is expected to be unicode, though I did try to offer ANSI support as well. It should work; however, you need to change the string literals to be non-wide.

###Results
![File Reading Comparison Results](http://i.imgur.com/li0QmNG.png)

It is evident that memory mapped files are not only a lot faster, but seem to not directly depend on file size. This is an extremely important part for any software that works with very large files. 
  


  
