//
//  SList.cpp
//  Scheme++
//
//  Created by Josh Sun on 2017-05-10.
//  Copyright © 2017 Josh Sun. All rights reserved.
//

#include "SList.hpp"

SList::SList(){};
SList::SList(sType type): type(type) {};
SList::SList(std::string s) : value(s), type (SYMBOL) {}
SList::SList(double s) : value(std::to_string(s)), type(NUMBER) {}
SList::SList(SLists s) : list(s), type(LIST) {}
SList::SList(proc s) : p(s) , type(PROC) {}
SList::SList(proc_env s) : pe(s), type(PROC_ENV) {}

bool double_is_int(std::string dVal) {
    double trouble = atof(dVal.c_str());
    return fabs(trouble) == floor(fabs(trouble));
}

std::string double_to_int(std::string dVal) {
    return std::to_string((int)atof(dVal.c_str()));
}

void SList::setType (sType t) {
    type = t;
}

void SList::setEnv(Environment* env){
  env = env;
}

void SList::push(SList s) {
    list.push_back(s);
}

void SList::pushList(SLists s) {
    for (auto vi = s.begin(); vi != s.end(); vi++)
        list.push_back(*vi);
}

SList::sType SList::getType() const {
    return type;
}

Environment* SList::getEnv() const{
  return env;
}

std::string SList::getTypeString() const {
    if (type == SYMBOL) return "SYMBOL";
    else if (type == NUMBER) return "NUMBER";
    else if (type == LIST) return "LIST";
    else if (type == PROC) return "PROC";
    else if (type == PROC) return "PROC_ENV";
    else return "LAMBDA";
}

SLists SList::getList () const {
    return list;
}

SList::proc SList::getProc() const {
    return p;
}

SList::proc_env SList::getProcEnv() const {
    return pe;
}

std::string SList::val() const {return value;}

std::string SList::listToString() {
    if (type == SYMBOL) return value;
    if (type == NUMBER) return double_is_int(value) ? double_to_int(value) : value;
    std::string strVal = "";
    for (auto vi = list.begin(); vi != list.end(); vi++) {
        strVal += vi->listToString();
        if (vi!=list.end()-1)
            strVal += " ";
    }
    return strVal;
}


//for debugging
std::string SList::getPrintString() const {
    std::string s = "";
    if (type == NUMBER) {
        s+=(double_is_int(value) ? double_to_int(value) : value);
    } else if (type == SYMBOL) {
        if (value == "#t")
            return "true";
        else if (value == "#f")
            return "false";
        else
            return value;
    } else if (type == PROC) {
        return "PROC";
    } else if (type == LAMBDA) {
        return "LAMBDA";
    } else {
        s+="(";
        for (auto vi = list.cbegin(); vi != list.cend(); vi++) {
            s+=vi->getPrintString();
            if (vi!=list.cend()-1) s+= " ";
        }
        if (s.back() == ' ') s.pop_back();
        s+=") ";
    }
    return s;
}
