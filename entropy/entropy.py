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
import argparse

class EntropyScript:
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
        argument("-k", "--order", dest = "order", action = "store", default = 1, metavar = "INT",
                                help = """builds Markov chains of the order k for estimations""")
        argument("sources", metavar = "SOURCE", nargs = "+", # Assume user needs to give 1 more.
                                help = """list of sources to estimate entropy and probability""")

        self.arguments = parser.parse_args()
        if (self.arguments.estimates and self.arguments.probabilities) or\
            not (self.arguments.estimates or self.arguments.probabilities):
            parser.print_help()
            sys.exit(-1)

    def execute(self, location = sys.argv[0]):
        for source in self.arguments.sources:
            with open(source, "r+b") as fd:
                mm = mmap.mmap(fd.fileno(), 0)
                probs = self.probabilities(mm)
                mm.close()

            if self.arguments.probabilities:
                self.print_probabilities(probs)
            elif self.arguments.estimates:
                entropy = self.estimates(probs)
                self.print_estimates(entropy)
            else: return 1 # Not reachable?
        return 0

    def probabilities(self, source): pass
    def estimates(self, probabilities): pass
    def print_probabilities(self, probabilities): pass
    def print_estimates(self, entropies): pass

INITIAL_ERROR_STATUS = -1
if __name__ == "__main__":
    status = INITIAL_ERROR_STATUS
    with EntropyScript() as tool:
        status = tool.execute()
    sys.exit(status)
