//
//  NativeProcedures.cpp
//  Scheme++
//
//  Created by Josh Sun on 2017-05-12.
//  Copyright © 2017 Josh Sun. All rights reserved.
//

#include "NativeProcedures.hpp"

SList dispatch_procedure(SList, SLists, Environment*);
SList evaluate(SList, Environment*);

SList add (const SLists& argv)  {
    double dVal = atof(argv[0].val().c_str());
    for (auto vi = argv.begin()+1; vi != argv.end(); vi++)
        dVal += atof((*vi).val().c_str());
    return SList(dVal);
}

SList subtract (const SLists& argv)  {
    double dVal = atof(argv[0].val().c_str());
    for (auto vi = argv.begin()+1; vi != argv.end(); vi++)
        dVal -= atof((*vi).val().c_str());
    return SList(dVal);
}

SList multiply (const SLists& argv)  {
    double dVal = atof(argv[0].val().c_str());
    for (auto vi = argv.begin()+1; vi != argv.end(); vi++)
        dVal *= atof((*vi).val().c_str());
    return SList(dVal);
}

SList divide (const SLists& argv)  {
    double dVal = atof(argv[0].val().c_str());
    for (auto vi = argv.begin()+1; vi != argv.end(); vi++)
        dVal /= atof((*vi).val().c_str());
    return SList(dVal);
}

SList mod (const SLists& argv)  {
    int iVal = atoi(argv[0].val().c_str());
    for (auto vi = argv.begin()+1; vi != argv.end(); vi++)
        iVal %= atoi((*vi).val().c_str());
    return SList(iVal);
}

SList sqrt (const SLists& arg)  {
    return SList(sqrt(atof(arg[0].val().c_str())));
}

SList sin (const SLists& arg)  {
    return SList(sin(atof(arg[0].val().c_str())));
}

SList cos (const SLists& arg)  {
    return SList(cos(atof(arg[0].val().c_str())));
}

SList tan (const SLists& arg)  {
    return SList(tan(atof(arg[0].val().c_str())));
}

SList arcsin (const SLists& arg)  {
    return SList(asin(atof(arg[0].val().c_str())));
}

SList arccos (const SLists& arg)  {
    return SList(acos(atof(arg[0].val().c_str())));
}

SList arctan (const SLists& arg)  {
    return SList(atan(atof(arg[0].val().c_str())));
}

SList abs (const SLists& arg) {
    return SList(fabs(atof(arg[0].val().c_str())));
}

SList greater_than (const SLists& argv) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (atof((*(vi+1)).val().c_str()) >= atof((*vi).val().c_str()))
            return SList("#f");
    }
    return SList("#t");
}

SList less_than (const SLists& argv) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (atof((*(vi+1)).val().c_str()) <= atof((*vi).val().c_str()))
            return SList("#f");
    }
    return SList("#t");
}

SList equal_num (const SLists& argv) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (fabs(atof((*(vi+1)).val().c_str())-atof((*vi).val().c_str()))>0.00000001) {
            return SList("#f");
        }
    }
    return SList("#t");
}

SList greater_equal (const SLists& argv) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (atof((*(vi+1)).val().c_str()) > atof((*vi).val().c_str()))
            return SList("#f");
    }
    return SList("#t");
}

SList less_equal (const SLists& argv) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (atof((*(vi+1)).val().c_str()) < atof((*vi).val().c_str()))
            return SList("#f");
    }
    return SList("#t");
}

SList append (const SLists& argv) {
    SList l;
    for (auto vi = argv.begin(); vi != argv.end(); vi++) {
        l.pushList(vi->getList());
        
    }
    return SList(l);
}

SList apply (const SLists& argv, Environment* env) {
    SList p = evaluate(argv[0],env);
    SLists args = argv[1].getList();
    return dispatch_procedure(p,args,env);
}

SList map (const SLists& argv, Environment* env){    
    SList newList(SList::LIST);
    for (int i = 0; i < argv[1].getList().size(); i++) {
        SLists n;
        SList args(SList::LIST);
        n.push_back(argv[0]);
        for (int j = 1; j < argv.size(); j++) {
            args.push(argv[j].getList()[i]);
        }
        n.push_back(args);
        newList.push(apply(n,env));
    }
    return newList;
}

SList max (const SLists& argv) {
    double max = -std::numeric_limits<double>::infinity();
    for (auto vi = argv.begin(); vi != argv.end(); vi++) {
        if (atof(vi->val().c_str()) > max)
            max = atof(vi->val().c_str());
    }
    return max;
}

SList min (const SLists& argv) {
    double min = std::numeric_limits<double>::infinity();
    for (auto vi = argv.begin(); vi != argv.end(); vi++) {
        if (atof(vi->val().c_str()) < min)
            min = atof(vi->val().c_str());
    }
    return min;
}

SList eqv (const SLists& argv) {
    for (auto vi = argv.begin(); vi != argv.end()-1; vi++) {
        if (vi->getType() != (vi+1)->getType()) {
            return SList("#f");
        } else if (vi->getType() == SList::SYMBOL) {
            if (vi->val() == "#f" || vi->val() != (vi+1)->val())
                return SList("#f");
        } else if (vi->getType() == SList::NUMBER) {
            if (fabs(atof((*(vi+1)).val().c_str())-atof((*vi).val().c_str()))>0.00000001)
                return SList("#f");
        } else if (vi->getType() == SList::LAMBDA || vi->getType() == SList::PROC) {
            if (vi->getProc() != (vi+1)->getProc())
                return SList("#f");
        } else {
            return eqv(vi->getList());
        }
    }
    return SList("#t");
}

SList isNumber (const SLists& arg)  {
    return arg[0].getType()==SList::NUMBER ? SList("#t") : SList("#f");
}

SList isProcedure (const SLists& arg)  {
    return arg[0].getType()==SList::PROC ? SList("#t") : SList("#f");
}

SList isSymbol (const SLists& arg)  {
    return arg[0].getType()==SList::SYMBOL ? SList("#t") : SList("#f");
}

SList isList (const SLists& arg)  {
    return arg[0].getType()==SList::LIST ? SList("#t") : SList("#f");
}

SList isNull (const SLists& arg) {
    if (arg[0].getType()!=SList::LIST) return SList("#f");
    if (arg[0].getList().size()==0) return SList("#t");
    return SList("#f");
}

SList length (const SLists& arg)  {
    return arg[0].getList().size();
}

SList list (const SLists& argv) {
    SList newList(SList::LIST);
    for (auto vi = argv.begin(); vi != argv.end(); vi++) {
        newList.push(*vi);
    }
    return newList;
}

SList cons (const SLists& argv) {
    SList newList(SList::LIST);
    newList.push(argv[0]);
    newList.push(argv[1]);
    return newList;
}

SList car (const SLists& argv) {
    return argv[0].getList()[0];
}

SList cdr (const SLists& argv) {
    SList s(SList::LIST);
    for (int i = 1; i < argv[0].getList().size(); i++) {
        s.push(argv[0].getList()[i]);
    }
    return s;
}

