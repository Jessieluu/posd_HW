#ifndef ITERATOR_H
#define ITERATOR_H

#include "struct.h"
#include "list.h"
#include <stack>
#include <queue>
#include <iostream>

using namespace std;

// template <class Type>
class Iterator 
{
public:
  virtual void first() = 0;
  virtual void next() = 0;
  virtual Term * currentItem() const = 0;
  virtual bool isDone() const = 0;
};

class NullIterator : public Iterator
{
public:
  NullIterator(Term * n){}
  void first(){}
  void next(){}
  Term * currentItem() const{
      return nullptr;
  }
  bool isDone() const{
    return true;
  }

};

class StructIterator : public Iterator
{
public:
  friend class Struct;
  void first() {
    _index = 0;
  }

  Term * currentItem() const {
    return _s->args(_index);
  }

  bool isDone() const {
    return _index >= _s->arity();
  }

  void next() {
    _index++;
  }

private:
  StructIterator(Struct *s): _index(0), _s(s) {}
  int _index;
  Struct* _s;
};

class ListIterator : public Iterator
{
friend class List;  
public:

  void first() {
    _index = 0;
  }

  Term * currentItem() const {
    return _list->args(_index);
  }

  bool isDone() const {
    return _index >= _list->arity();
  }

  void next() {
    _index++;
  }

private:
  ListIterator(List *list): _index(0), _list(list) {}
  int _index;
  List* _list;
};

template <class Type>
class DFSIterator : public Iterator{
public:
  DFSIterator(Type term):_term(term){}

  void first(){ //initial
    while(!_dfsStack.empty())
      _dfsStack.pop();
    
    for(int i = _term->arity()-1; i>=0; --i){ 
      Struct * s = dynamic_cast<Struct *>(_term->args(i));
      if(s){
        for(int num = s->arity()-1; num>=0; --num){                  
          Struct * ss = dynamic_cast<Struct *>(s->args(num));
          if(ss){
            for(int n = ss->arity()-1; n>=0; --n)  {        
              _dfsStack.push(ss->args(n));
            }
          }
          _dfsStack.push(s->args(num));
        }          
      }

      List * l = dynamic_cast<List *>(_term->args(i));
      if(l){
        for(int m = l->arity()-1; m>=0; --m)
          _dfsStack.push(l->args(m));
      }

      _dfsStack.push(_term->args(i));
    } 
    // if(!isDone())   
    //   next();
  }

  Type currentItem() const {
    return _dfsStack.top();
  }

  bool isDone() const {
    return _dfsStack.empty();
  }

  void next(){
    if(!_dfsStack.empty()){
      _dfsStack.pop();  
    }
         
  }

private:
  Type _term;
  stack<Type> _dfsStack; 
};


template <class Type>
class BFSIterator : public Iterator{
public:
  BFSIterator(Type term):_term(term){}

  void first(){ //initial
    while(!_bfsQueue.empty())
      _bfsQueue.pop();

    for(int i=0; i<=_term->arity()-1; ++i){
      _bfsQueue.push(_term->args(i));      
    }

    for(int j=0; j<=_term->arity()-1; ++j){
      Struct * s = dynamic_cast<Struct *>(_term->args(j));
      // std::cout<<_term->args(j)->symbol();  
      if(s){
        for(int n=0; n<=s->arity()-1; ++n){
          // std::cout<<s->args(n)->symbol();          
          Struct * ss = dynamic_cast<Struct *>(s->args(n)); 
          _bfsQueue.push(s->args(n));              
          if(ss){
            for(int m=0; m<=ss->arity()-1; ++m)
              _bfsQueue.push(ss->args(m));
          }      
        }                
      }
      List * l = dynamic_cast<List *>(_term->args(j));
      if(l){
        for(int num=0; num<=l->arity()-1; ++num){
          // std::cout<<s->args(n)->symbol();          
          List * ll = dynamic_cast<List *>(l->args(num)); 
          _bfsQueue.push(l->args(num));              
          if(ll){
            for(int numm=0; numm<=ll->arity()-1; ++numm)
              _bfsQueue.push(ll->args(numm));
          }      
        }         
      }
    }
    // if(!isDone())   
    //   next();
  }

  Type currentItem() const {
    return _bfsQueue.front();
  }

  bool isDone() const {
    return _bfsQueue.empty();
  }

  void next(){
    if(!_bfsQueue.empty()){
      _bfsQueue.pop();   
    }
         
  }


private:
  Type _term;
  queue<Type> _bfsQueue; 

};

#endif