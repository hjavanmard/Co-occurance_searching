# Co-occurance_searching

**Description**

My program will read files containing English sentences, one sentence per line. The files will be specified on the command line. Once the files have been read, the program will generate a "query>" prompt. A query will consist of a sequence of words separated by white space and terminated by a carriage return all words on a single line. The program will then generate a co-occurrence list consisting of filename, line number, and sentence from the input lines. Co-occurrence is when all the query words appear in the sentence. The program will continue to prompt for queries until the user enters "!exit". At this time, the program should stop accepting queries and report basic statistics regarding the number of sentences in the results.

**My program is able to process very large sentence files and many queries efficiently. It is also respond quickly to queries with empty result sets.**

** The code is successfully tested will files containing up to one million sentences, as well as other edge cases.**

**File Format**

The input files contain lists of English language sentences. Here is a sample:

The house is red.

The baker said "The cakes are ready".

Don't touch the switch!
