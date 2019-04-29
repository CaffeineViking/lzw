LZW and Arithmetic Coding Compression Library for C++14
=======================================================

In this 3 for 1 repository you get a bunch of data compression goodies. First we have a well known dictionary coding method called LZW as a portable C++14 library. Then we have another library of a popular entropy coding algorithm called Adaptive Arithmetic Coding that's also for C++14. While they aren't very optimal or fast (my implementation that is), they have a very small code footprint, and are nicely documented. So if you want to find out how these two compression algorithms work the two libraries I have bundled can be a good starting point. I also bundle a little script that allows you to measure the statistics of text-based sources, which you can use to compare the compression rate of different algorithms (and to find the best rate).

* **liblzw:** is the library for the dictionary-based codec I was talking about, head over to that folder for how to build/use it.
* **entropy.py** the script I was talking about, which you can find in the `entropy` folder together with some testing dataset.
* **libaac:** is the library for the entropy-based codec I mentioned before, head over there to learn how to build and use it.
