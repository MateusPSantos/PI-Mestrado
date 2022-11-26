#include <stdio.h>
#include <stdlib.h>
#include <ilcplex/ilocplex.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#define maxt 6


using namespace std;

int p[maxt],f[maxt],h[maxt],d[maxt],n,C[maxt];

int numero(string num){
    stringstream ss(num);
    int retorno=0;
    ss >> retorno;
    return retorno;
}


// Funcao de Leitura das instâncias
void leitura(char *nome){
    string ler;

    ifstream arquivo(nome);
    arquivo>>ler;

    n=numero(ler);

    for(int t=0;t<n;t++){
        arquivo >> ler;
        p[t]=numero(ler);
    }

    for(int t=0;t<n;t++){
        arquivo >> ler;
        f[t]=numero(ler);
    }
    for(int t=0;t<n;t++){
        arquivo >> ler;
        h[t]=numero(ler);
    }
    for(int t=0;t<n;t++){
        arquivo >> ler;
        d[t]=numero(ler);
    }
    for(int t=0;t<n;t++){
        arquivo >> ler;
        C[t]=numero(ler);
    }
    

    arquivo.close();

}

int main(int argc, char *argv[]){
    string varName;
    double objval_;
    int elapsed_;
    time_t start, end;
    double best_bound;
    double gap;

    leitura(argv[1]);

    try{
        IloEnv env;
        IloModel model(env);
	    IloIntVarArray x(env,n);
        IloIntVarArray s(env,n);
	    IloBoolVarArray y(env,n);


        s[0]=IloIntVar(env,0,0);
        s[n]=IloIntVar(env,0,0);
        

	    IloExpr expr(env);
	    IloExpr FO(env);


        for(int t=0;t<n;t++){
            
             FO+=p[t]*x[t];
            
        }
        for(int t=0;t<n;t++){
            
             FO+=f[t]*y[t];
            
        }
        for(int t=0;t<n;t++){
            
             FO+=h[t]*s[t];
            
        }


	    model.add(IloMinimize(env,FO));

	   // retrições
        model.add(x[0]==d[0]+s[0]);

	    for(int t=1;t<n;t++){
            model.add(s[t-1]+x[t]=d[t]+s[t]);
	    }

        for(int t=0;t<n;t++){
            model.add(x[t]<=C[t]);
        }
	    



	    IloCplex cplex(model);
	    cplex.setParam(IloCplex::TiLim,3600);
	    start=time(NULL);
	    elapsed_=0;
	    cplex.solve();
	    end=time(NULL);
	    elapsed_=difftime(end,start);
	    objval_=cplex.getObjValue();
	    best_bound = cplex.getBestObjValue();
        gap = cplex.getMIPRelativeGap();
        int cont=0;

	    cout<<endl;
	    ofstream result;
        result.open("resultados_CPLEX.txt", std::ofstream::out | std::ofstream::app);
        string linha;
        result << argv[1];
        result << "\n";
        result <<"otimo: ";
        result << objval_;
        result <<"\n";
        result <<"valores X";
        result <<"\n";
        for(int t=0;t<n;t++){
             
            result << cplex.getValue(x[t]);
            result <<"  ";

        }
        result << "\n";
        result <<"valores S";
        result <<"\n";
        for(int t=0;t<n;t++){
             
            result << cplex.getValue(s[t]);
            result <<"  ";

        }

        result << "\n";
        result.close();

    }catch(const IloException& e) {
    	std::cerr << "\n\nCPLEX Raised an exception:\n";
        std::cerr << e << "\n";
    }



	return 0;

}
