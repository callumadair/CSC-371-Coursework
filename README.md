# This is my implementation of 371pass coursework for the advanced object oriented programming module (CSC371) at Swansea University. The program functions entirely on the command line and makes use of the catch2 unit testing framework for behaviour validation, the majority of tests were written by Martin Porcheon, with additional unit testing added by myself. Additionally member function signatures were provided but very little implementation was initially present.


All required functionality specified in the specification is present and working fully as far as my own and the
automated tests can show, including functionality mentioned in the coursework discussion question board such as
favouring the entries held in a new container over the existing container and being able to update multiple containers
at once (e.g. the category and item simultaneously).

Functions are commented above the provided comments minus the TODOs (e.g. "my comment" followed by "provided comment"), except those
that are trivial and the code is self documenting, i.e. the getIdent(), setIdent(), size() and empty() member functions in the container
classes, and the provided functions are in the same order as provided, but may have some new member functions between
them, such as in the App namespace where each action has been extracted and placed in order after the run member
function to improve the readability of the code, or in the container classes where the merge function follows the
addItem and addCategory functions.

All variable names are written in snake_case and all member functions names are written in camelCase to ensure
consistency with the provided framework.
