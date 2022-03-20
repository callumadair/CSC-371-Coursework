# 371pass

___
You should use this README to document things you did that you think are clever and could get you marks, or things you
couldn't get working and perhaps why. It is fully expected that a lot of students will not complete this coursework—it
is designed to be challenging. The mark scheme is also designed to be fair—marks are awarded for everything you
complete, with some given for the quality of the code you write.
___
All required functionality specified in the specification is present and working fully as far as my own and the
automated tests can show, including functionality mentioned in the coursework discussion question board such as
favouring the entries held in a new container over the existing container and being able to update multiple containers
at once (e.g. the category and item simultaneously).

All functions are commented above the provided comments (e.g. "my comment" followed by "provided comment"), except those
that are trivial and the code is self documenting, i.e. the getIdent(), setIdent(), size() and empty() member functions in the container
classes, and the provided functions are in the same order as provided, but may have some new member functions between
them, such as in the App namespace where each action has been extracted and placed in order after the run member
function to improve the readability of the code, or in the container classes where the merge function follows the
addItem and addCategory functions.

All variable names are written in snake_case and all member functions names are written in camelCase to ensure
consistency with the provided framework.