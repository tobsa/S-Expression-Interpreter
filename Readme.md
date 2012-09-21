S-Expression Interpreter
========================

This is an interpreter that can read mathematical expressions and evaluate them. Implemented by using
s-expressions.

Operations supported: Addition, subtraction, multiplication, division. In addition parantheses and 
variables are supported.

Usage example:

==> fx = 5                      // Create the variable fx and set it to 5
5
==> fx + fx                     // Add them together
10
==> (g = fx) + 5 + (h = 3)      // Create two new variables (g and h) and add them together
13
    
    