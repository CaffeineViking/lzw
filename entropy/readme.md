Entropy
-------

Besides having a bunch of different datasets, the star of this folder is the `entropy.py` measuring script. You can use it to find the probability of a symbol from an ASCII alphabet for any given text-based source. With this you can also find the Shannon entropy of a source. The script can also model the source as a Markov source, which means it can estimate the conditional or joint probability of a source of degree k. You should call `entropy.py -h` to find out how it works, but in a nutshell, you call it with either `-e` (for entropy) or `-p` (for probabilities) and mention with `-k [ORDER]` the Markov order you want. The tail of the input is the file to analyze. Example usage: `entropy.py -p -k 3 alice29.txt`. **Note:** the datasets themeslves aren't mine.
