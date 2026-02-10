//begin =adder.h============
// The following two lines prevent adder.h from being included
// more than once in any source (.cpp) file . If this were to happen
 // it would cause problems in the compile process but it is difficult to
 // prevent manually in large projects. These#ifndef , #define and#endif
 // commands for an ”include guard” and are types of compiler directive.
 // The include guard contains an ID for this file ”MATHSLIBADDERH”, this is
 // arbitrary but must be unique within the project.
#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

#ifdef maths_EXPORTS
#define MATHSLIB_API __declspec(dllexport)
#else
#define MATHSLIB_API __declspec(dllimport)
#endif

// 必须有这个宏，否则不会生成 .lib 文件
MATHSLIB_API int add(int a, int b);

#endif
#ifndef MATHSLIBADDERH
 #define MATHSLIBADDERH

 // prototype for our function
 int add(int a, int b);

#endif
 //end=adder.h========== =