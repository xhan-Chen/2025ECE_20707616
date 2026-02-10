//begin =adder.h============
// The following two lines prevent adder.h from being included
// more than once in any source (.cpp) file . If this were to happen
 // it would cause problems in the compile process but it is difficult to
 // prevent manually in large projects. These#ifndef , #define and#endif
 // commands for an ”include guard” and are types of compiler directive.
 // The include guard contains an ID for this file ”MATHSLIBADDERH”, this is
 // arbitrary but must be unique within the project.
#ifndef MATHSLIB_ADDER_H  // 1. 改为 ifndef
#define MATHSLIB_ADDER_H

// 2. 简化导出/导入逻辑
#if defined(WIN32) || defined(_WIN32)
#ifdef mathsEXPORTS  // 3. 确保和命令行 /D 后的名称一致
#define MATHSLIB_API __declspec(dllexport)
#else
#define MATHSLIB_API __declspec(dllimport)
#endif
#else
#define MATHSLIB_API // 非 Windows 平台设为空
#endif

// 4. 函数声明
#ifdef __cplusplus
extern "C" {  // 建议加上 extern "C"，防止 C++ 编译时函数名被混淆（Name Mangling）
#endif

    MATHSLIB_API int add(int a, int b);

#ifdef __cplusplus
}
#endif

#endif
 //end=adder.h========== =