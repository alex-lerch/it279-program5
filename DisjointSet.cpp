// DisjointSet implementation using union by size and path compression
// By Mary Elaine Califf and Alex Lerch

/*--------------------------------------------------------------------------------*
 *   included files                                                               *
 *--------------------------------------------------------------------------------*/
#include "DisjointSet.h"
#include <iostream>


/*--------------------------------------------------------------------------------*
 *   function definitions                                                         *
 *--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
 *                                                                                *
 *   name: DisjointSet(int)                                                       *
 *                                                                                *
 *   description: constructor                                                     *
 *                                                                                *
 *--------------------------------------------------------------------------------*/
DisjointSet::DisjointSet(int numObjects) : theArray(numObjects, -1)
{
    numValues = numObjects;
}



/*--------------------------------------------------------------------------------*
 *                                                                                *
 *   name: find(int)                                                              *
 *                                                                                *
 *   description: recursive method to find the item -- does path compression on   *
 *                the way out of the recursion                                    *
 *                                                                                *
 *   returns: the root of the index passed                                        *
 *                                                                                *
 *--------------------------------------------------------------------------------*/
int DisjointSet::find(int objectIndex)
{
    // variables used
    int setName; // the name of a particular set

    // if index is a root then return that index
    if (theArray[objectIndex] < 0) {
        return objectIndex;
    }
    // the index is not a root and so we must look at index's parent to see if it is the root
    else {
        setName = find(theArray[objectIndex]);
        theArray[objectIndex] = setName;
        return setName;
    }
}



/*--------------------------------------------------------------------------------*
 *                                                                                *
 *   name: doUnion(int, int)                                                      *
 *                                                                                *
 *   description: union method joins the sets of object1 and object2              *
 *                                                                                *
 *   precondition: the objects must NOT be in the same set when union is called   *
 *                                                                                *
 *   precondition: both parameters must be in the range 0 to numValues-1          *
 *                                                                                *
 *   returns: true if values in array are now in same set and false otherwise     *
 *                                                                                *
 *--------------------------------------------------------------------------------*/
bool DisjointSet::doUnion(int objIndex1, int objIndex2)
{
    // variables used
    int root1; // the root of the set that contains objIndex1
    int root2; // the root of the set that contains objIndex2

    // get the roots of objIndex1 and objIndex2
    root1 = find(objIndex1);
    root2 = find(objIndex2);

    // if root1 and root2 are in different sets, then proceed with unioning them
    if (root1 != root2) {
        // if root1 has fewer items in the set than root2, then assign root1 to point at root2
        if (theArray[root1] > theArray[root2]) {
            // update the size
            theArray[root2] += theArray[root1];
            // assign root1 to point at root2
            theArray[root1] = root2;
        }
        // root1 has either fewer or the same number of items as root2. assign root2 to point at root1
        else {
            // update the size
            theArray[root1] += theArray[root2];
            // assign root2 to point at root1
            theArray[root2] = root1;
        }
        
        // check for complete set union
        if ( (theArray[root1] == (numValues * -1) ) || (theArray[root2] == (numValues * -1) ) ) {
            return true;
        }
    }

    // the set has not been completely unioned so return false
    return false;
}



/*--------------------------------------------------------------------------------*
 *                                                                                *
 *   name: printArrayValues(ostream)                                              *
 *                                                                                *
 *   description: provided for testing purposes                                   *
 *                                                                                *
 *--------------------------------------------------------------------------------*/
void DisjointSet::printArrayValues(std::ostream &outputStream)
{
    for (int i = 0; i < numValues; i++)
    {
        outputStream << theArray[i] << " ";
    }
    outputStream << std::endl;
}
