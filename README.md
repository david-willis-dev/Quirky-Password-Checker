# Quirky Password Checker
## Problem:
### Is your password not like other passwords? Find out how secure/unique/quirky your password is in comparison to a dataset of 100,000+ most used passwords based off of the Jaro-Algorithm for determining string similarity.
## Features: 
### A list will be returned containing the 10 most similar results to your password, as well as their Jaro score, and where your password ranks among the most commonly used passwords. There will also be a list that returns the computational runtime of each data structure, as well as a general comparison word (ie: “quirky” or “unoriginal”) decided upon by the uniqueness score (based upon various calculations using the jaro value as well as usage of special characters, numbers, capital letters).
## Data:
### We are using a data list of the most common passwords, sorted by frequency featured in the link below: https://lucidar.me/en/security/list-of-100000-most-common-passwords/
## Strategy:
### The data set of the most common passwords will be parsed from a text file and sequentially compared to the user's password using the Jaro-Winkler algorithm. Two data structures are utilized to store the full dataset of passwords (a fully function Red-Black Tree as well as a Max Heap structure), and the Jaro value of those passwords when computed with the user input password. At the end of the output (the list of 10 most similar results and the resulting "quirky score"), there is a time output of the red-black tree (how many clicks it took to compare), and a time output for the max heap of the same data and the user input. This allows the user to see the difference in data structure speed for the same job, all while still getting an idea of how "quirky" their password is.

