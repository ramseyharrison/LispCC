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

SList evaluate(SList s, Environment* env);

SLists getArgs (SList l) {
  SLists unproc_args = l.getList();
  SLists args;
  for (int i = 1; i < unproc_args.size(); i++) {
    args.push_back(unproc_args[i]);
  }
  return args;
}


SList evaluate_sequence(SList s,Environment* env,int offset){
  auto sl = s.getList().begin()+offset;
  for(sl;sl != s.getList().end()-1;++sl)
    evaluate(*sl,env);
  return evaluate(s.getList()[s.getList().size()-1],env);
}

SList dispatch_procedure(SList p, SLists args, Environment* env)
{      
  if (p.getType() == SList::LAMBDA) {
    Environment* new_env = new Environment(p.getList()[1].getList(),args,env);
    return evaluate_sequence(p,new_env,2);		
  } else if (p.getType()==SList::PROC) {
    return p.getProc()(args);
  } else if (p.getType() == SList::PROC_ENV) {
    return p.getProcEnv()(args,env);
  } else {
    return evaluate(p,env);
  }
}

SList cond_to_if(SList s){
  SLists branch_list;
  branch_list.push_back(SList("if"));
  for(size_t cond_index = 1; cond_index < s.size(); cond_index++){
    SLists begin_list;
    branch_list.push_back(s[cond_index][0]);
    begin_list.push_back(SList("begin"));
    for(size_t begin_index = 1; begin_index < s[cond_index].size(); begin_index++){
      begin_list.push_back(s[cond_index][begin_index]);
    }
    branch_list.push_back(SList(begin_list));
    if(cond_index < s.size() - 1){      
      branch_list.push_back(SList("if"));
    }
  }
  return branch_list;
}

void get_begin(SList s, int index, SLists& begin){
  for(size_t begin_index = 1; begin_index < s[index].size(); begin_index++)
    begin.push_back(s[index][begin_index]);
}
SList cond_if(SList s, int index = 1){
  if(index == s.size()){
    return SList();
  }  
  SLists branch,begin;
  branch.push_back(SList("if"));
  s[index][0].val() == "else" ? branch.push_back(SList("#t")) : branch.push_back(s[index][0]);
  begin.push_back(SList("begin"));
  get_begin(s,index,begin);
  branch.push_back(SList(begin));
  branch.push_back(cond_if(s,++index));
  return branch;
}

SList evaluate (SList s, Environment* env) {
  if (s.getType() == SList::SYMBOL) {             //variable reference
    if (s.val()[0] == '\'')
      return SList(s.val().substr(1,s.val().length()-1));
    return (*(env->find(s.val())))[s.val()];
  } else if (s.getType() == SList::NUMBER) {      //constant literal
    return s;
  } else if (s.getList().size() == 0) {
    return s;
  } else if (s.getList()[0].val() == "define") {
    return (env->env[s.getList()[1].val()] = evaluate(s.getList()[2],env));
  } else if (s.getList()[0].val() == "lambda") {
    s.setType(SList::LAMBDA);
    return s;
  } else if (s.getList()[0].val() == "quote") {
    return s.getList()[1];
  } else if (s.getList()[0].val() == "set!") {
    (*(env->find(s.getList()[1].val())))[s.getList()[1].val()] = evaluate(s.getList()[2],env);
    return SList();
  } else if (s.getList()[0].val() == "if") {
    return evaluate(s.getList()[1],env).val()!="#f" ? evaluate(s.getList()[2],env) : evaluate(s.getList()[3],env);
  } else if (s.getList()[0].val() == "cond") {
    return evaluate(cond_if(s),env);
  } else if (s.getList()[0].val() == "begin") {
    return evaluate_sequence(s,env,1);       
  } else {            //procedure call
    SList p = evaluate(s.getList()[0],env);
    SLists args = getArgs(s);
    for (int i = 0; i < args.size(); i++)
      args[i] = evaluate (args[i],env);
    return dispatch_procedure(p,args,env);    
  }
}


void env_setup (Environment* std_env) {
  std_env->env.insert({"#f",SList("#f")});
  std_env->env.insert({"#t",SList("#t")});
  std_env->env.insert({"nil",SList("nil")});
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
  std_env->env.insert({"=",SList(&equal_num)});
  std_env->env.insert({"append",SList(&append)});
  std_env->env.insert({"apply",SList(&apply)});
  std_env->env.insert({"map",SList(&map)});
  std_env->env.insert({"max",SList(&max)});
  std_env->env.insert({"min",SList(&min)});
  std_env->env.insert({"eqv",SList(&eqv)});
  std_env->env.insert({"number?",SList(&isNumber)});
  std_env->env.insert({"list?",SList(&isList)});
  std_env->env.insert({"procedure?",SList(&isProcedure)});
  std_env->env.insert({"null?",SList(&isNull)});
  std_env->env.insert({"length",SList(&length)});
  std_env->env.insert({"list",SList(&list)});
  std_env->env.insert({"cons",SList(&cons)});
  std_env->env.insert({"car",SList(&car)});
  std_env->env.insert({"cdr",SList(&cdr)});
}

//repl
int main(int argc, const char * argv[]) {
  Environment* std_env = new Environment;
  env_setup (std_env);
  cout << FormattedIO::readFile("message.txt") << endl;;
  while (true) {
    SList temp;
    try {
      std::cout << "schm/> ";
      std::string line = FormattedIO::readLine();
      if (line.length()==0) continue;
      temp = evaluate(Parser::parse(line), std_env);
    } catch (const char* msg) {
      cerr << msg << endl;
      continue;
    } catch (...) {
      cerr << "Interpreter Error: Unhandled Exception" << endl;
      continue;
    }
    cout << "=> " << temp.getPrintString() << endl;
            
  }
  return 0;
}
