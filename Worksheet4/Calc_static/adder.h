//begin =adder.h============
// The following two lines prevent adder.h from being included
// more than once in any source (.cpp) file . If this were to happen
 // it would cause problems in the compile process but it is difficult to
 // prevent manually in large projects. These#ifndef , #define and#endif
 // commands for an ¡±include guard¡± and are types of compiler directive.
 // The include guard contains an ID for this file ¡±MATHSLIBADDERH¡±, this is
 // arbitrary but must be unique within the project.

#ifndef MATHSLIBADDERH
 #define MATHSLIBADDERH

 // prototype for our function
 int add(int a, int b);

#endif
 //end=adder.h========== =