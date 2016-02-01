# Huffman-Encoding

This project written in C++ is for the CSE 250 summer course of 2015. 
The purpose is to perform text encryption using Huffman compression. Cryptography is the art of achieving security by encoding or transforming messages to make them non-readable. 
The input file is tested for each character frequency before encoding. These frequencies numbers are then used to build a binary tree whereas each character of the text is represented by the tree's leaf-node. Each left-going edge is represented as 0 and each right-going edge is represented as 1. In the end, we get a binary representation of the character after tracing the nodes of the tree. Using a few bits per character, the original file size is reduced a significant amount in the new encrypted output file. 

Read more about it at:
http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1126/handouts/220%20Huffman%20Encoding.pdf
