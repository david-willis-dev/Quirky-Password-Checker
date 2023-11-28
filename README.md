# Quirky Password Checker
===========================
Features:
A list will be returned containing the most similar results to your password, as well as their
Jaro-Winkler score, and where your password ranks among the most commonly used passwords.
There will also be a list that returns the computational runtime of each data structure, as well as a
general comparison word (ie: “quirky” or “unoriginal”) decided upon by the uniqueness score.
---
Data:
We are using a data list of the most common passwords, sorted by frequency featured in the link
below:
https://lucidar.me/en/security/list-of-100000-most-common-passwords/
---
Strategy:
The data set of the most common passwords will be parsed from a text file and sequentially
compared to the user's password using the Jaro-Winkler algorithm. A red-black tree (structure subject to change) 
of the 100 most similar passwords will be shown to the user as the algorithm iterates over the
dataset. At any time the user will be able to stop the program and a "quirkiness" factor will be
calculated based on how similar their password is to passwords in the dataset, and how common
the most similar passwords are. The user can repeat this task to try any number of passwords.
---
