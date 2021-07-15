
// Copyright (c) 2018 SinoVoice Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// vim: set ts=2 sw=2 fenc=utf-8 bomb ff=unix :

/**
 * @file hci/pp.h
 *
 * 提供基于 C/C++ 预处理器的 meta-programming 工具
 *
 * @link https://en.wikipedia.org/wiki/X_Macro
 * @link https://en.wikipedia.org/wiki/Metaprogramming
 * @link https://github.com/pfultz2/Cloak/wiki
 * @link http://p99.gforge.inria.fr/
 *
 * 公开接口:
 *
 *   - PP_CAT(...)
 *
 *   - PP_COUNT(...)
 *   - PP_COUNT_1(...)
 *   - PP_COUNT_2(...)
 *
 *   - PP_IF(c)
 *   - PP_NOT(c)
 *   - PP_NOT_EMPTY(...)
 *   - PP_IS_EMPTY(...)
 *
 *   - PP_IF_EMPTY(...)
 *   - PP_IF_NOT_EMPTY(...)
 *
 *   - PP_EACH(N, ...)
 *   - PP_EACH_SN(S, N, ...)
 *   - PP_EACH_NE(N, E, ...)
 *   - PP_EACH_SNE(S, N, E, ...)
 *
 *   - PP_IEACH(N, ...)
 *   - PP_IEACH_SN(S, N, ...)
 *   - PP_IEACH_NE(N, E, ...)
 *   - PP_IEACH_SNE(S, N, E, ...)
 *
 *   - PP_TA_ARGS_SN(...)
 *   - PP_TA_ARGS_NN(...)
 *
 *   - PP_TP_ARGS_SN(...)
 *   - PP_TP_ARGS_NN(...)
 *
 *   - PP_TC_ARGS_SN(...)
 *   - PP_TC_ARGS_NN(...)
 *
 *   - PP_FP_ARGS_SN(...)
 *   - PP_FP_ARGS_NN(...)
 *
 *   - PP_FC_ARGS_SN(...)
 *   - PP_FC_ARGS_NN(...)
 *
 *   - PP_VARIADIC_TEMPLS(n, XX)
 */
#ifndef HCI_PP_H_
#define HCI_PP_H_

// clang-format off

#define PP_EMPTY(...)

#define PP_EXPAND(f, a) f a

#define PP_EVAL0(...) PP_EVAL1(PP_EVAL1(PP_EVAL1(__VA_ARGS__)))
#define PP_EVAL1(...) PP_EVAL2(PP_EVAL2(PP_EVAL2(__VA_ARGS__)))
#define PP_EVAL2(...) PP_EVAL3(PP_EVAL3(PP_EVAL3(__VA_ARGS__)))
#define PP_EVAL3(...) PP_EVAL4(PP_EVAL4(PP_EVAL4(__VA_ARGS__)))
#define PP_EVAL4(...) PP_EVAL5(PP_EVAL5(PP_EVAL5(__VA_ARGS__)))
#define PP_EVAL5(...) __VA_ARGS__

#include <hci/pp-args.h>

/**
 * @def PP_CAT(...)
 *
 * 将任意个参数连接在一起。PP_CAT(a, b, c) 会被替换为 abc
 */
#define PP_CAT(...) PP_CAT_EXP(PP_CAT_,(__VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,))
#define PP_CAT_EXP(a, b) a b
#define PP_CAT_(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,...) \
  PP_CAT_26(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z)
#define PP_CAT_26(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z) \
  a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t##u##v##w##x##y##z

/**
 * @def PP_COUNT_1(...)
 *
 * 至少有一个参数时，展开为参数数量减去一，否则展开为 0
 */
#define PP_COUNT_1(...) PP_COUNT_1_EXP(PP_A29, (__VA_ARGS__,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))
#define PP_COUNT_1_EXP(a, b) a b

/**
 * @def PP_COUNT_2(...)
 *
 * 至少有两个参数时，展开为参数数量减去二，否则展开为 0
 */
#define PP_COUNT_2_EXP(a, b) a b
#define PP_COUNT_2(...) PP_COUNT_2_EXP(PP_A29, (__VA_ARGS__,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10, 9,8,7,6,5,4,3,2,1,0,0))

/**
 * @def PP_IS_PAR(...)
 *
 * 判断第一个参数是否为 ()，圆括号内可以有内容。是的话展开为 1，否则展开为 0
 */
#define PP_IS_PAR(...) PP_IS_PAR_EXP(PP_IS_PAR_, (__VA_ARGS__,))
#define PP_IS_PAR_EXP(a, b) a b
#define PP_IS_PAR_(x, ...) PP_A01(PP_IS_PAR_PROBE_ x,0)
#define PP_IS_PAR_PROBE_(...) ~,1,

#define PP_PARENTHSIS_(...) ()

/**
 * @def PP_IS_EMPTY(...)
 *
 * 判断第一个参数是否为空，是的话展开为 1，否则展开为 0
 */
#define PP_IS_EMPTY(...) PP_IS_EMPTY_EXP(PP_IS_EMPTY_, (__VA_ARGS__,))
#define PP_IS_EMPTY_EXP(a,b) a b
#define PP_IS_EMPTY_(x, ...) PP_CAT(PP_IS_EMPTY_, PP_IS_PAR(x), PP_IS_PAR(PP_PARENTHSIS_ x ()))
#define PP_IS_EMPTY_01 1
#define PP_IS_EMPTY_00 0
#define PP_IS_EMPTY_11 0
#define PP_IS_EMPTY_10 0

/**
 * @def PP_NOT_EMPTY(...)
 *
 * 判断第一个参数是否非空，是的话展开为 1，否则展开为 0
 */
#define PP_NOT_EMPTY(...) PP_NOT_EMPTY_EXP(PP_NOT_EMPTY_, (__VA_ARGS__,))
#define PP_NOT_EMPTY_EXP(a, b) a b
#define PP_NOT_EMPTY_(x, ...) PP_CAT(PP_NOT_EMPTY_, PP_IS_PAR(x), PP_IS_PAR(PP_PARENTHSIS_ x ()))
#define PP_NOT_EMPTY_01 0
#define PP_NOT_EMPTY_00 1
#define PP_NOT_EMPTY_11 1
#define PP_NOT_EMPTY_10 1

/**
 * @def PP_COUNT(...)
 *
 * 展开为参数数量，当仅有一个空参数时，等同于没有参数
 */
#define PP_COUNT(...) PP_COUNT_(PP_COUNT_EXP(PP_COUNT_1, (__VA_ARGS__)), (__VA_ARGS__))
#define PP_COUNT_EXP(a, b) a b
#define PP_COUNT_(n, a) PP_CAT (PP_COUNT__, PP_IS_PAR(PP_CAT(PP_COUNT_, n))) a
#define PP_COUNT_0 ()
#define PP_COUNT__1(...) PP_NOT_EMPTY_(__VA_ARGS__,)
#define PP_COUNT__0(...) PP_A29(__VA_ARGS__,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1)

/**
 * @def PP_IS_0(...)
 *
 * 判断第一个参数是否是 0，是的话展开为 1，否则展开为 0
 */
#define PP_IS_0(...) PP_IS_0_EXP(PP_IS_0_, (__VA_ARGS__,))
#define PP_IS_0_EXP(a, b) a b
#define PP_IS_0_(x, ...) PP_A01(PP_CAT(PP_IS_0__, x), 0)
#define PP_IS_0__0 ~,1,

/**
 * @def PP_NOT_0(...)
 *
 * 判断第一个参数是否非 0，是的话展开为 1，否则展开为 0
 */
#define PP_NOT_0(...) PP_NOT_0_EXP(PP_NOT_0_, (__VA_ARGS__,))
#define PP_NOT_0_EXP(a, b) a b
#define PP_NOT_0_(x, ...) PP_A01(PP_CAT(PP_NOT_0__, x), 1)
#define PP_NOT_0__0 ~,0,

/**
 * @def PP_IS_1(...)
 *
 * 判断第一个参数是否为 1，是的话展开为 1，否则展开为 0
 */
#define PP_IS_1(...) PP_IS_1_EXP(PP_IS_1_, (__VA_ARGS__,))
#define PP_IS_1_EXP(a, b) a b
#define PP_IS_1_(x, ...) PP_A01(PP_CAT(PP_IS_1__, x), 0)
#define PP_IS_1__1 ~,1,

/**
 * @def PP_NOT_1(...)
 *
 * 判断第一个参数是否非 1，是的话展开为 1，否则展开为 0
 */
#define PP_NOT_1(...) PP_NOT_1_EXP(PP_NOT_1_, (__VA_ARGS__,))
#define PP_NOT_1_EXP(a, b) a b
#define PP_NOT_1_(x, ...) PP_A01(PP_CAT(PP_NOT_1__, x), 1)
#define PP_NOT_1__1 ~,0,

/**
 * @def PP_NOT(x)
 *
 * 对第一个参数进行逻辑值取反操作，为 0 或空时展开为 1, 否则展开为 0
 * 第一个参数必须是一个空的或者以字母、数字或下划线开始的 token。
 */
#define PP_NOT(...) PP_NOT_EXP(PP_NOT_, (__VA_ARGS__,))
#define PP_NOT_EXP(a, b) a b
#define PP_NOT_(x, ...) PP_CAT(PP_NOT__, PP_IS_EMPTY_(x,), PP_IS_0_(x,))
#define PP_NOT__00 0
#define PP_NOT__01 1
#define PP_NOT__10 1
#define PP_NOT__11 1

/**
 * @def PP_COMPL(b)
 *
 * 对 b 进行求补运算，b 只能是 0 或 1。
 *
 * b 为 0 时结果为 1
 * b 为 1 时结果为 0
 */
#define PP_COMPL(b) PP_CAT(PP_COMPL_, b)
#define PP_COMPL_0 1
#define PP_COMPL_1 0

/**
 * @def PP_BOOL(...)
 *
 * 展开为第一个参数的布尔值。零值或空值展开为 0，否则展开为 1
 * 第一个参数必须是一个空的或者以字母、数字或下划线开始的一个 token。
 */
#define PP_BOOL(...) PP_COMPL(PP_NOT(__VA_ARGS__,))

/**
 * @def PP_IF(c)(T, F)
 *
 * 当 PP_BOOL(c) 为 1 时展开为 T, 否则展开为 F。F 可省略，默认为 PP_EMPTY。
 */
#define PP_IF(c) PP_CAT(PP_IF_, PP_BOOL(c))
#define PP_IF_1(T, ...) T
#define PP_IF_0_EXP(a, b) a b
#define PP_IF_0(...) PP_IF_0_EXP(PP_IF_0_, (__VA_ARGS__,PP_EMPTY))
#define PP_IF_0_(T, F, ...) F

#define PP_STRINGIFY(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_STRINGIFY_, PP_STRINGIFY0)(__VA_ARGS__)
#define PP_STRINGIFY0() ""
#define PP_STRINGIFY_(...) #__VA_ARGS__

/**
 * @def PP_NULL(T, F, ...)
 *
 * 当 ... 无参数或第一个参数为空参数时，展开为 T, 否则展开为 F
 */
#define PP_NULL(T, F, ...) PP_NULL_EXP(PP_NULL_, (T, F, __VA_ARGS__))
#define PP_NULL_EXP(a, b) a b
#define PP_NULL_(T, F, ...) PP_CAT(PP_NULL__, PP_IS_EMPTY_(__VA_ARGS__,))(T, F)
#define PP_NULL__1(T, F) T
#define PP_NULL__0(T, F) F

/**
 * @def PP_NEXT(N, ...)
 *
 * 当 ... 仅有一个参数时展开为 PP_EMPTY, 大于一个参数时展开为 N。无参数属于
 * 未定义行为。
 */
#define PP_NEXT(N, ...)    PP_NEXT_EXP(PP_A29, (__VA_ARGS__,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,PP_EMPTY))
#define PP_NEXT_EXP(a, b) a b PP_NEXT_EMPTY
#define PP_NEXT_EMPTY

/**
 * @def PP_CURR(N, E, ...)
 *
 * 当 ... 仅有一个参数时展开为 PP_EMPTY, 有两个参数时展开为 E, 大于两个参数
 * 时展开为 N。无参数属于未定义行为。
 */
#define PP_CURR(N, E, ...) PP_CURR_EXP(PP_A29, (__VA_ARGS__,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,E))
#define PP_CURR_EXP(a, b) a b

/**
 * @def PP_EACH(N, ...)
 *
 * 将 ... 中的参数 x 依次展开为 N(x), 相邻结果之间用空格连接
 *
 * 在 ... 中可以有 0~28 个参数
 */
#define PP_EACH(...) PP_EVAL0(PP_NULL(PP_EACH::EMPTY_N, PP_EACH_, __VA_ARGS__,)(__VA_ARGS__,))

#define PP_EACH_(N, ...)         PP_NEXT(PP_EACH1, __VA_ARGS__)(N, __VA_ARGS__)
#define PP_EACH1(N, X, ...) N(X) PP_NEXT(PP_EACH2, __VA_ARGS__)(N, __VA_ARGS__)
#define PP_EACH2(N, X, ...) N(X) PP_NEXT(PP_EACH1, __VA_ARGS__)(N, __VA_ARGS__)

/**
 * @def PP_KEACH(N, K, ...)
 *
 * 将 ... 中的参烽 x 依次展开为 N(K, x), 相邻结果之间用空格连接
 *
 * 在 ... 中可以有 0~28 个参数
 */
#define PP_KEACH(N, ...) PP_EVAL0(PP_NULL(PP_KEACH::EMPTY_K, PP_KEACH_, __VA_ARGS__,)(N, __VA_ARGS__,))

#define PP_KEACH_(N, K, ...)            PP_NEXT(PP_KEACH1, __VA_ARGS__)(N, K, __VA_ARGS__)
#define PP_KEACH1(N, K, X, ...) N(K, X) PP_NEXT(PP_KEACH2, __VA_ARGS__)(N, K, __VA_ARGS__)
#define PP_KEACH2(N, K, X, ...) N(K, X) PP_NEXT(PP_KEACH1, __VA_ARGS__)(N, K, __VA_ARGS__)

/**
 * @def PP_EACH_SN(S, N, ...)
 *
 * 与 PP_EACH 功能类似，将 ... 中的参数 x 依次展开，首个参数展开为 S(x),
 * 其他参数展开为 N(x)。相邻结果之间用空格连接
 *
 * 在 ... 中可以有 0~28 个参数
 */
#define PP_EACH_SN(S, ...) PP_EVAL0(PP_NULL(PP_EACH_SN::EMPTY_N, PP_EACH_SN_, __VA_ARGS__,)(S,__VA_ARGS__,))

#define PP_EACH_SN_(S, N, ...)         PP_NEXT(PP_EACH_SN1, __VA_ARGS__)(S, N, __VA_ARGS__)
#define PP_EACH_SN1(S, N, X, ...) S(X) PP_NEXT(PP_EACH1,    __VA_ARGS__)(   N, __VA_ARGS__)

/**
 * @def PP_EACH_NE(N, E, ...)
 *
 * 与 PP_EACH 功能类似, 将 ... 中的参数 x 依次展开, 非末尾参数展开为 N(x)，
 * 末尾参数展开为 E(x)。相邻结果之间用空格连接
 *
 * 在 ... 中可以有 0~28 个参数
 */
#define PP_EACH_NE(N, ...) PP_EVAL0(PP_NULL(PP_EACH_NE::EMPTY_E, PP_EACH_NE_, __VA_ARGS__,)(N, __VA_ARGS__,))

#define PP_EACH_NE_(N, E, ...)                                  PP_NEXT(PP_EACH_NE1, __VA_ARGS__)(N, E, __VA_ARGS__)
#define PP_EACH_NE1(N, E, X, ...) PP_CURR(N, E, __VA_ARGS__)(X) PP_NEXT(PP_EACH_NE2, __VA_ARGS__)(N, E, __VA_ARGS__)
#define PP_EACH_NE2(N, E, X, ...) PP_CURR(N, E, __VA_ARGS__)(X) PP_NEXT(PP_EACH_NE1, __VA_ARGS__)(N, E, __VA_ARGS__)

/**
 * @def PP_EACH_SNE(S, N, E, ...)
 *
 * 与 PP_EACH 功能类似, 将 ... 中的参数 x 依次展开, 首个参数展开为 S(x),
 * 非首尾参数展开为 N(x)，末尾参数展开为 E(x)。相邻结果之间用空格连接
 *
 * 在 ... 中可以有 0~28 个参数
 */
#define PP_EACH_SNE(S, N, ...) PP_EVAL0(PP_NULL(PP_EACH_SNE::EMPTY_E, PP_EACH_SNE_, __VA_ARGS__,)(S, N, __VA_ARGS__,))

#define PP_EACH_SNE_(S, N, E, ...)         PP_NEXT(PP_EACH_SNE1, __VA_ARGS__)(S, N, E, __VA_ARGS__)
#define PP_EACH_SNE1(S, N, E, X, ...) S(X) PP_NEXT(PP_EACH_NE1,  __VA_ARGS__)(   N, E, __VA_ARGS__)

/**
 * @def PP_IEACH(N, ...)
 *
 * 将 ... 中的每个参数 x 依次展开为 N(i,x), i 为基数为 1 的参数索引。相邻
 * 结果之间用空格连接
 *
 * 在 ... 中可以有 0~28 个参数
 */
#define PP_IEACH(...) PP_EVAL0(PP_NULL(PP_IEACH::EMPTY_N, PP_IEACH_, __VA_ARGS__,)(__VA_ARGS__,))
#define PP_INEXT(...) (__VA_ARGS__,)
#define PP_IEACH_(N, ...)                         PP_NEXT(PP_IEACH1, __VA_ARGS__)((~,),       N, __VA_ARGS__)
#define PP_IEACH1(I, N, X, ...) N(PP_COUNT_1 I,X) PP_NEXT(PP_IEACH2, __VA_ARGS__)(PP_INEXT I, N, __VA_ARGS__)
#define PP_IEACH2(I, N, X, ...) N(PP_COUNT_1 I,X) PP_NEXT(PP_IEACH1, __VA_ARGS__)(PP_INEXT I, N, __VA_ARGS__)

/**
 * @def PP_IEACH_SN(S, N, ...)
 *
 * 与 PP_IEACH 功能类似，将 ... 中的每个参数 x 依次展开，首个参数展开为
 * S(i,x)，其他参数展开为 N(i,x), i 为基数为 1 的参数索引。相邻结果之间
 * 用空格连接
 *
 * 在 ... 中可以有 0~28 个参数
 */
#define PP_IEACH_SN(S, ...) PP_EVAL0(PP_NULL(PP_IEACH_SN::EMPTY_N, PP_IEACH_SN_, __VA_ARGS__,)(S, __VA_ARGS__,))
#define PP_IEACH_SN_(S, N, ...)                         PP_NEXT(PP_IEACH_SN1, __VA_ARGS__)((~,), S,    N, ##__VA_ARGS__)
#define PP_IEACH_SN1(I, S, N, X, ...) S(PP_COUNT_1 I,X) PP_NEXT(PP_IEACH1,    __VA_ARGS__)(PP_INEXT I, N, ##__VA_ARGS__)


/**
 * @def PP_IEACH_NE(N, E, ...)
 *
 * 与 PP_IEACH 功能类似，将 ... 中的每个参数 x 依次展开，非末尾参数展开为
 * N(i,x)，末尾参数展开为 E(i,x), i 为基数为 1 的参数索引。相邻结果之间用
 * 空格连接
 *
 * 在 ... 中可以有 0~28 个参数
 */
#define PP_IEACH_NE(N, ...) PP_EVAL0(PP_NULL(PP_IEACH_NE::EMPTY_E, PP_IEACH_NE_, __VA_ARGS__,)(N, __VA_ARGS__,))
#define PP_IEACH_NE_(N, E, ...)                                                  PP_NEXT(PP_IEACH_NE1, __VA_ARGS__)((~,),       N, E, __VA_ARGS__)
#define PP_IEACH_NE1(I, N, E, X, ...) PP_CURR(N, E, __VA_ARGS__)(PP_COUNT_1 I,X) PP_NEXT(PP_IEACH_NE2, __VA_ARGS__)(PP_INEXT I, N, E, __VA_ARGS__)
#define PP_IEACH_NE2(I, N, E, X, ...) PP_CURR(N, E, __VA_ARGS__)(PP_COUNT_1 I,X) PP_NEXT(PP_IEACH_NE1, __VA_ARGS__)(PP_INEXT I, N, E, __VA_ARGS__)

/**
 * @def PP_IEACH_SNE(S, N, E, ...)
 *
 * 与 PP_IEACH 功能类似，将 ... 中的每个参数 x 依次展开，首个参数展开为
 * S(i,x)，非首尾参数展开为 N(i,x), 末尾参数展开为 E(i,x), i 为基数为 1
 * 的参数索引。相邻结果之间用空格连接
 *
 * 在 ... 中可以有 0~28 个参数
 */
#define PP_IEACH_SNE(S, N, ...) PP_EVAL0(PP_NULL(PP_IEACH_SNE::EMPTY_E, PP_IEACH_SNE_, __VA_ARGS__,)(S, N, __VA_ARGS__,))
#define PP_IEACH_SNE_(S, N, E, ...)                         PP_NEXT(PP_IEACH_SNE1, __VA_ARGS__)((~,), S,    N, E, __VA_ARGS__)
#define PP_IEACH_SNE1(I, S, N, E, X, ...) S(PP_COUNT_1 I,X) PP_NEXT(PP_IEACH_NE1,  __VA_ARGS__)(PP_INEXT I, N, E, __VA_ARGS__)

#define __PP_NSB(...) namespace __VA_ARGS__ {
#define PP_NS_BEGIN(...) PP_EVAL0(PP_IF(PP_COUNT(__VA_ARGS__))(PP_EACH_)(__PP_NSB, __VA_ARGS__,))

#define __PP_NSE(...) }
#define PP_NS_END(...)   PP_EVAL0(PP_IF(PP_COUNT(__VA_ARGS__))(PP_EACH_)(__PP_NSE, __VA_ARGS__,))

// 模板中的模板参数声明
#define PP_TA_A0(i, t) typename t
#define PP_TA_AN(i, t) , typename t
#define PP_TA_ARGS_SN(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_IEACH_SN_)(PP_TA_A0, PP_TA_AN, __VA_ARGS__,)
#define PP_TA_ARGS_NN(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_IEACH_)   (          PP_TA_AN, __VA_ARGS__,)

// 模板中的函数参数声明
#define PP_TP_A0(i, t) t&& PP_CAT(a, i)
#define PP_TP_AN(i, t) , t&& PP_CAT(a, i)
#define PP_TP_ARGS_SN(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_IEACH_SN_)(PP_TP_A0, PP_TP_AN, __VA_ARGS__,)
#define PP_TP_ARGS_NN(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_IEACH_)   (          PP_TP_AN, __VA_ARGS__,)

// 模板中的函数调用参数
#define PP_TC_A0(i, t) std::forward<t>(PP_CAT(a, i))
#define PP_TC_AN(i, t) , std::forward<t>(PP_CAT(a, i))
#define PP_TC_ARGS_SN(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_IEACH_SN_)(PP_TC_A0, PP_TC_AN, __VA_ARGS__,)
#define PP_TC_ARGS_NN(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_IEACH_)   (          PP_TC_AN, __VA_ARGS__,)

// 非模板的函数参数声明
#define PP_FP_A0(i, t) t PP_CAT(a, i)
#define PP_FP_AN(i, t) , t PP_CAT(a, i)
#define PP_FP_ARGS_SN(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_IEACH_SN_)(PP_FP_A0, PP_FP_AN, __VA_ARGS__,)
#define PP_FP_ARGS_NN(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_IEACH_)   (          PP_FP_AN, __VA_ARGS__,)

// 非模板的函数调用参数
#define PP_FC_A0(i, t) PP_CAT(a, i)
#define PP_FC_AN(i, t) , PP_CAT(a, i)
#define PP_FC_ARGS_SN(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_IEACH_SN_)(PP_FC_A0, PP_FC_AN, __VA_ARGS__,)
#define PP_FC_ARGS_NN(...) PP_IF(PP_COUNT(__VA_ARGS__))(PP_IEACH_)   (          PP_FC_AN, __VA_ARGS__,)

#define PP_WHEN_T(...) __VA_ARGS__
#define PP_WHEN_F(...)
#define PP_WHEN(x) PP_IF(x)(PP_WHEN_T, PP_WHEN_F)

#define PP_DEFER(x) x PP_EMPTY()
#define PP_OBSTRUCT(...) __VA_ARGS__ PP_DEFER(PP_EMPTY)()

#define PP_DEC(x) PP_CAT(PP_DEC_, x)
#define PP_DEC_0 0
#define PP_DEC_1 0
#define PP_DEC_2 1
#define PP_DEC_3 2
#define PP_DEC_4 3
#define PP_DEC_5 4
#define PP_DEC_6 5
#define PP_DEC_7 6
#define PP_DEC_8 7
#define PP_DEC_9 8
#define PP_DEC_10 9
#define PP_DEC_11 10
#define PP_DEC_12 11
#define PP_DEC_13 12
#define PP_DEC_14 13
#define PP_DEC_15 14
#define PP_DEC_16 15
#define PP_DEC_17 16
#define PP_DEC_18 17
#define PP_DEC_19 18

#define PP_INC(x) PP_CAT(PP_INC_, x)
#define PP_INC_0 1
#define PP_INC_1 2
#define PP_INC_2 3
#define PP_INC_3 4
#define PP_INC_4 5
#define PP_INC_5 6
#define PP_INC_6 7
#define PP_INC_7 8
#define PP_INC_8 9
#define PP_INC_9 10
#define PP_INC_10 11
#define PP_INC_11 12
#define PP_INC_12 13
#define PP_INC_13 14
#define PP_INC_14 15
#define PP_INC_15 16
#define PP_INC_16 17
#define PP_INC_17 18
#define PP_INC_18 19

#define PP_REPEAT(count, macro, ...) \
  PP_WHEN(count) \
  ( \
    PP_OBSTRUCT(PP_REPEAT_INDIRECT)() \
    ( \
      PP_DEC(count), macro, __VA_ARGS__ \
    ) \
    PP_OBSTRUCT(macro) \
    ( \
      count, __VA_ARGS__ \
    ) \
  )
#define PP_REPEAT_INDIRECT() PP_DEFER(PP_REPEAT)


/**
 * @def PP_VARIADIC_TEMPLS(n, XX)
 *
 * 展开 n + 1 个 XX 宏
 *
 * PP_VARIADIC_TEMPLS(0, XX) 展开为 XX();
 * PP_VARIADIC_TEMPLS(1, XX) 展开为 XX(); XX(T1);
 * PP_VARIADIC_TEMPLS(2, XX) 展开为 XX(); XX(T1); XX(T1,T2);
 * PP_VARIADIC_TEMPLS(3, XX) 展开为 XX(); XX(T1); XX(T1,T2); XX(T1,T2,T3);
 *
 * 以此类推, n 最大可以是 28
 *
 * 在定义 XX 宏时，你会用到 PP_XX_ARGS_SN 和 PP_XX_ARGS_NN 宏
 */
#define PP_VARIADIC_TEMPLS(n, XX) PP_EVAL0(XX()); PP_EVAL0(PP_REPEAT(n, PP_VT_, XX, ~))
#define PP_VT_(i, XX, ...) XX(PP_CAT(PP_SLICE_, i)(T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28));
// clang-format on
#endif  // HCI_PP_H_
