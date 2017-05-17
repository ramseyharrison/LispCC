//
//  main.cpp
//  Scheme_Interpreter
//
//  Created by Josh Sun on 2017-05-08.
//  Copyright © 2017 Josh Sun. All rights reserved.
//

#include "FormattedIO.hpp"
#include "SList.hpp"
#include "Parser.hpp"
#include "NativeProcedures.hpp"
#include "Environment.hpp"

#include <unordered_map>


using namespace std;


SLists getArgs (SList l) {
    SLists unproc_args = l.getList();
    SLists args;
    for (int i = 1; i < unproc_args.size(); i++) {
        args.push_back(unproc_args[i]);
    }
    return args;
}

SList evaluate (SList s, Environment* env) {
    if (s.getType() == SList::SYMBOL) {             //variable reference
        return (*(env->find(s.val())))[s.val()];
    } else if (s.getType() == SList::NUMBER) {      //constant literal
        return s;
    } else if (s.getList()[0].val() == "define") {
        env->env[s.getList()[1].val()] = evaluate(s.getList()[2],env);
        return SList();
    } else if (s.getList()[0].val() == "lambda") {
        s.setType(SList::LAMBDA);
        return s;
    } else if (s.getList()[0].val() == "quote") {
        return s.getList()[1];
    } else if (s.getList()[0].val() == "set!") {
        (*(env->find(s.getList()[1].val())))[s.getList()[1].val()] = evaluate(s.getList()[2],env);
        return SList();
    } else if (s.getList()[0].val() == "if") {
        return evaluate(s.getList()[1],env).val()=="#t" ? evaluate(s.getList()[2],env) : (evaluate(s.getList()[3],env).val() == "else" ? evaluate(s.getList()[4],env) : SList());
    } else if (s.getList()[0].val() == "begin") {
        for (int i = 1; i < s.getList().size()-1; i++) evaluate(s.getList()[i], env);
        return evaluate(s.getList()[s.getList().size()-1],env);
    } else {            //procedure call
        SList p = evaluate(s.getList()[0],env);
        SLists args = getArgs(s);
        for (int i = 0; i < args.size(); i++)
            args[i] = evaluate (args[i],env);
        if (p.getType() == SList::LAMBDA) {
            return evaluate(p.getList()[2], new Environment(p.getList()[1].getList(),args,env));
        } else if (p.getType()==SList::PROC) {
            return p.getProc()(args);
        } else {
            return evaluate(p,env);
        }
    }
}


void env_setup (Environment* std_env) {
    std_env->env.insert({"#f",SList("#f")});
    std_env->env.insert({"#f",SList("#t")});
    std_env->env.insert({"else",SList("else")});
    
    std_env->env.insert({"+",SList(&add)});
    std_env->env.insert({"-",SList(&subtract)});
    std_env->env.insert({"*",SList(&multiply)});
    std_env->env.insert({"/",SList(&divide)});
    std_env->env.insert({"mod",SList(&mod)});
    std_env->env.insert({"sqrt",SList(&sqrt)});
    std_env->env.insert({"sin",SList(&sin)});
    std_env->env.insert({"cos",SList(&cos)});
    std_env->env.insert({"tan",SList(&tan)});
    std_env->env.insert({"asin",SList(&arcsin)});
    std_env->env.insert({"acos",SList(&arccos)});
    std_env->env.insert({"atan",SList(&arctan)});
    std_env->env.insert({"abs",SList(&abs)});
    std_env->env.insert({">",SList(&greater_than)});
    std_env->env.insert({"<",SList(&less_than)});
    std_env->env.insert({"=",SList(&equal_num)});
}

//repl
int main(int argc, const char * argv[]) {
    Environment* std_env = new Environment;
    env_setup (std_env);
    cout << FormattedIO::readLicense() << endl;;
    while (true) {
        SList temp;
        try {
            temp = evaluate(Parser::parse(FormattedIO::readLine()), std_env);
        } catch (const char* msg) {
            cerr << msg << endl;
            continue;
        } catch (...) {
            cerr << "ERROR: Uncaught Exception" << endl;
            continue;
        }
        if (temp.val().length() != 0)
            cout << temp.getPrintString() << endl;
    }
    return 0;
}
