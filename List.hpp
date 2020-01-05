//
//  List.hpp
//  LikeHaskell
//
//  Created by 贾皓翔 on 2020/1/5.
//  Copyright © 2020 贾皓翔. All rights reserved.
//

#ifndef List_hpp
#define List_hpp

#include <stdio.h>
#include <type_traits>
namespace List{


template<typename Type,Type...element>
struct List{
    using type=Type;
};

template<typename List>
struct Head;

template<typename T>
struct Head<List<T>>;

template<typename T,typename List<T>::type H,typename List<T>::type ...Args>
struct Head<List<T,H,Args...>>{
    static constexpr typename List<T>::type value=H;
};

namespace Imple{
template<typename T,T a,T b>
struct Greater{
    static constexpr T value=a>b?a:b;
};
template<typename T,T a,T b>
struct Litter{
    static constexpr T value=a<b?a:b;
};


template<template<typename T,T,T> class Trait,typename List>
struct FindOneBetter;

template<template<typename T,T,T> class Trait,typename T>
struct FindOneBetter<Trait,List<T>>;

template<template<typename T,T,T> class Trait,typename T,typename List<T>::type H>
struct FindOneBetter<Trait,List<T,H>>{
    static constexpr typename List<T>::type value =H;
};

template <template<typename T,T,T> class Trait,typename T,typename List<T>::type H,typename List<T>::type...Args>
struct FindOneBetter<Trait,List<T,H,Args...>>{
    static constexpr typename List<T>::type nextMax=FindOneBetter<Trait, List<T, Args...>>::value;
    static constexpr typename List<T>::type value=Trait<T,nextMax,H>::value;
};

}

template<typename List>
using Maximum=Imple::FindOneBetter<Imple::Greater,List>;
template<typename List>
using Minimum=Imple::FindOneBetter<Imple::Litter, List>;

template<typename List>
struct Tail;
template<typename T>
struct Tail<List<T>>;
template<typename T,typename List<T>::type H>
struct Tail<List<T,H>>{
    using type=List<T>;
};
template<typename T,typename List<T>::type H,typename List<T>::type...Args>
struct Tail<List<T,H,Args...>>{
    using type=List<T,Args...>;
};

template<typename List>
struct Null:public std::false_type{
};

template<typename T>
struct Null<List<T>>:public std::true_type{
};

template<typename List>
struct Length;

template<typename T>
struct Length<List<T>>{
    static constexpr size_t value=0;
};

template<typename T,typename List<T>::type H>
struct Length<List<T,H>>{
    static constexpr size_t value=1;
};

template<typename T,typename List<T>::type H,typename List<T>::type...Args>
struct Length<List<T,H,Args...>>{
    static constexpr size_t value=1+Length<List<T,Args...>>::value;
};

namespace Imple{
template <typename T,T a,T b>
struct AddValue{
    static constexpr T value=a+b;
};
template <typename T,T a,T b>
struct MulValue{
    static constexpr T value=a*b;
};
template<template<typename T,T,T>class Op,typename List>
struct OpListValue;

template<template<typename T,T,T>class Op,typename T>
struct OpListValue<Op, List<T>>{
    static constexpr typename List<T>::type value=0;
};

template<template<typename T,T,T>class Op,typename T,typename List<T>::type H>
struct OpListValue<Op, List<T,H>>{
    static constexpr typename List<T>::type value=H;
};

template<template<typename T,T,T>class Op,typename T,typename List<T>::type H,typename List<T>::type...Args>
struct OpListValue<Op, List<T,H,Args...>>{
    static constexpr typename List<T>::type value=Op<T,OpListValue<Op, List<T,Args...>>::value,H>::value;
};


}

template<typename List>
using Sum=Imple::OpListValue<Imple::AddValue, List>;
template<typename List>
using Product=Imple::OpListValue<Imple::MulValue, List>;


template<typename List,typename List::type Value>
struct PushFront;

template<typename T,T...Args,typename List<T>::type Value>
struct PushFront<List<T,Args...>,Value>{
    using type=List<T,Value,Args...>;
};

template<typename List,typename List::type Value>
struct PushBack;

template<typename T,T...Args,typename List<T>::type Value>
struct PushBack<List<T,Args...>,Value>{
    using type=List<T,Args...,Value>;
};

template<typename List1,typename List2>
struct ListCnt;
template<typename T,T...Args1,T...Args2>
struct ListCnt<List<T,Args1...>, List<T,Args2...>>{
    using type=List<T,Args1...,Args2...>;
};

template<typename List,typename List::type Value>
struct Elem;

template<typename T,typename List<T>::type Value>
struct Elem<List<T>,Value>{
    static constexpr bool value=false;
};

template<typename T,typename List<T>::type H,typename List<T>::type ...Args,typename List<T>::type Value>
struct Elem<List<T,H,Args...>,Value>{
    static constexpr bool value=(H==Value)||Elem<List<T,Args...>,Value>::value;
};




}

#endif /* List_hpp */
