
//
//  SList.hpp
//  Scheme++
//
//  Created by Josh Sun on 2017-05-10.
//  Copyright © 2017 Josh Sun. All rights reserved.
//

#ifndef SList_hpp
#define SList_hpp

#include <vector>
#include <string>
#include <math.h>
#include <stdlib.h>


#include <iostream>

class Environment;


class SList {
public:
    typedef SList (*proc)(const std::vector<SList>&);
    typedef SList (*proc_env)(const std::vector<SList>&, Environment* env);
  enum sType {SYMBOL, NUMBER, LIST, PROC, LAMBDA, PROC_ENV};
  
    void push(SList s);
    std::string getPrintString() const;
    size_t size() const;
    sType getType() const;
    void setType(sType t);
    void setEnv(Environment* env);
    Environment* getEnv() const;
    std::string val() const;
    std::vector<SList> getList () const;
    std::string listToString();
    proc getProc() const;
    proc_env getProcEnv() const;
    void pushList(std::vector<SList> s);
    std::string getTypeString() const;
    
    SList();
    SList(sType type);
    SList(std::string s);
    SList(double s);
    SList(std::vector<SList> s);
    SList(proc s);
    SList(proc_env s);
private:
    std::string value;
    std::vector<SList> list;
    proc p;
    proc_env pe;
    Environment* env;
    sType type;
    
    
};

typedef std::vector<SList> SLists;

#endif /* SList_hpp */
