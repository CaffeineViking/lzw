#!/usr/bin/env python
# -------- entropy.py

# Script distributed under "The MIT License".
# Copyright © 2017 Erik S. Vasconcelos Jansson
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the “Software”), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

import os
import sys
import mmap
import math
import argparse
import collections

class EntropyScript:
    SYMBOL_LENGTH = 1
    USAGE = "(-e | -p) [-w | -k INT]* SOURCE [SOURCE]*"
    DESCRIPTION = """Simple tool for estimating the #k
           entropy or the probabilities of a source"""

    def __enter__(self): return self
    def __exit__(self, etype,
                value, etrace): pass

    def __init__(self):
        parser = argparse.ArgumentParser(description  =  self.DESCRIPTION,
                                         usage = "%(prog)s " + self.USAGE)
        argument = parser.add_argument # Shortcut for adding new argument.
        operation = parser.add_mutually_exclusive_group(required  =  True)
        operation = operation.add_argument # Mutually exclusive operation.

        operation("-e", "--estimates", dest = "estimates", action = "store_true",
                  help = """gives the empirical entropy for the sources in bits""")
        operation("-p", "--probabilities", dest = "probabilities", action = "store_true",
                  help = """retrieves the empirical probabilities of the source""")

        argument("-w", "--write-results", dest = "write", action = "store_true",
                                help = """instead of printing to stdout, create file for each""")
        argument("-k", "--order", dest = "order", action = "store", default = 0, metavar = "INT",
                                help = """builds Markov models of the order k for estimations""")
        argument("sources", metavar = "SOURCE", nargs = "+", # Assume user needs to give 1 more.
                                help = """list of sources to estimate entropy and probability""")

        self.arguments = parser.parse_args()
        if (self.arguments.estimates and self.arguments.probabilities) or\
            not (self.arguments.estimates or self.arguments.probabilities):
            parser.print_help()
            sys.exit(-1)

    def execute(self, location = sys.argv[0]):
        k = int(self.arguments.order)
        print("Markov model k =",  k)
        for source in self.arguments.sources:
            with open(source, "r+b") as fd:
                print("Source: " + source)
                mm = mmap.mmap(fd.fileno(), 0)
                size = str(mm.size()) + " bytes"
                print("Source length: " + size)
                probs = self.probabilities(mm, k)
                print("Source states:", len(probs))
                mm.close() # Release the MM handle.

            if self.arguments.probabilities:
                self.print_probabilities(probs)
            elif self.arguments.estimates:
                entropy = self.estimates(probs)
                self.print_estimates(entropy)
                print(entropy)
            else: return 1 # Not reachable?
            print("") # Separate next file.
        return 0

    def probabilities(self, source, k):
        transitions = {}
        maximum_length = source.size()
        state = source.read(self.SYMBOL_LENGTH)
        history = collections.deque(maxlen = k+1)
        history.append(state) # Starting state.

        while state: # Reads symbols until EOF.
            for order in range(0, len(history)):
                transition = [history[i] for i in
                      range(order, len(history))]
                transition = b"".join(transition)
                if transition not in transitions:
                    transitions[transition] = 0
                transitions[transition] += 1
            state = source.read(self.SYMBOL_LENGTH)
            history.append(state) # Adds to window.

        probabilities = {}
        for state, frequency in transitions.items():
            total = maximum_length - len(state) -  1
            probabilities[state] = frequency / total
        return probabilities # Joint probabilities.

    def estimates(self, probabilities):
        order = int(self.arguments.order)
        entropy = 0.0 # Measured in bits and Sh,  for k.
        previous_entropy = 0.0 # Joint entropy of k - 1.
        for state, probability in probabilities.items():
            information = -math.log(probability, 2)
            if (len(state) - 1) == order: # For order k.
                entropy   +=   probability * information
            if (len(state) == order): # Previous entropy k-1.
                previous_entropy += probability * information
        # Follow the chain rule H(X|Y,Z) = H(X,Y,Z) - H(Y,Z).
        return entropy - previous_entropy # A Conditional Sh.

    def print_probabilities(self, probabilities): pass
    def print_estimates(self, entropies): pass

INITIAL_ERROR_STATUS = -1
if __name__ == "__main__":
    status = INITIAL_ERROR_STATUS
    with EntropyScript() as tool:
        status = tool.execute()
    sys.exit(status)
