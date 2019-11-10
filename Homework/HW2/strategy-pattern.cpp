#include <iostream>


// Base class
class Strategy {
public:
    // pure virtual function providing interface framework.
    virtual void store() { std::cout<< "not defined" <<std::endl; }
   
};



// Derived classes
class Relational: public Strategy {
public:
   
    void store() {
        std::cout << "We are using table store!" << std::endl;
    }
};

class NOSQL: public Strategy {
public:
   
    void store() {
        std::cout << "We are using document store!" << std::endl;
    }
};

class Graph: public Strategy {
public:
    
    void store() {
        std::cout<< "We are using node store!" <<std::endl;
    }
};


//Set g->store() syntax:
// 1 for Relational, 2 for NOSQL and 3 for Graph
void setStoreStrategy(Strategy *g, int c){
    Relational R;
    NOSQL  N;
    Graph G;
    
    if(c==1)
    {
        g = &R;
        g->store();
    }
    else if(c==2)
    {
        g = &N;
        g->store();
    }
    else if(c==3)
    {
        g = &G;
        g->store();
    }
    
}

int main(void) {
    
    int answer;
    Strategy *Stg;
    Relational Re;
    NOSQL  No;
    Graph Gr;
    
    std::cout << "Select some software "<<std::endl;
    
    std::cout << "Relational(1) NOSQL(2) Graph(3): "<<std::endl;
    
    std::cin >> answer;
    
    if(answer==1)
        {
            Stg = &Re;
            Stg->store();
        }
    else if(answer==2)
        {
            Stg = &No;
            Stg->store();
        }
    else if(answer==3)
    {
        Stg = &Gr;
        Stg->store();
    }
    else{std::cout<< "You didn't enter a correct option!"<<std::endl;}
    
    
 

    setStoreStrategy(Stg,1);
    

   

    
    return 0;
}
