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
#define maxn 6
#define maxm 5

using namespace std;

int c[maxn][maxm],f[maxm],n,m;

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
    
    arquivo >> ler;
    n=numero(ler);
    
    arquivo >> ler;
    m=numero(ler);
    
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            arquivo >> ler;
            c[i][j]=numero(ler);
        }
    }

    for(int i=0;i<m;i++){
        arquivo >>ler;
        f[i]=numero(ler);
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
	    IloArray<IloBoolVarArray> x(env,n);
	    IloBoolVarArray y(env,n);
	  


	    for(int i=0 ; i < n ; i++){
		    x[i] = IloBoolVarArray(env,m);

	    }

	    IloExpr expr(env);
	    IloExpr FO(env);

	    for(int i=0;i<n;i++){
		    for(int j=0;j<m;j++){
			    FO+=c[i][j]*x[i][j];
		    }
	    }
	    for(int i=0;i<m;i++){
	        
			FO+=f[i]*y[i];
		    
	    }

	    model.add(IloMinimize(env,FO));

	   // retrições


	    for(int i=0;i<n;i++){
		    for(int j=0;j<m;j++){
		     	expr+=x[i][j];
		    }
		    model.add(expr==1);
		    expr.clear();
	    }

        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                 model.add(x[i][j]<=y[j]);
            }
           
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
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                if(cplex.getValue(x[i][j])>0.9999){
                    result << "1";
                    result <<"  ";
                }
                else{
                    result << "0";
                    result <<"  ";
                }
                
            }
            result << "\n";

        }
                result << "\n";
        result <<"valores X";
        result <<"\n";
        for(int i=0;i<m;i++){
            if(cplex.getValue(y[i])>0.9999){
                result << "1";
                result <<"  ";
            }
            else{
                    result << "0";
                    result <<"  ";
            }
                
        }

        result << "\n";
        result.close();

    }catch(const IloException& e) {
    	std::cerr << "\n\nCPLEX Raised an exception:\n";
        std::cerr << e << "\n";
    }



	return 0;

}
