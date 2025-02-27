//
//  Environment.hpp
//  Scheme++
//
//  Created by Josh Sun on 2017-05-15.
//  Copyright © 2017 Josh Sun. All rights reserved.
//

#ifndef Environment_hpp
#define Environment_hpp
#include <unordered_map>
#include "SList.hpp"

typedef std::string symbol;
typedef std::unordered_map<symbol, SList> envType;

class Environment {
public:
    Environment ();
    Environment (SLists params, SLists args, Environment* outerEnv);
    void insert(symbol,SList);
    envType* find (symbol);
    envType env;
    
private:
  Environment* outerEnv;
};
#endif /* Environment_hpp */
