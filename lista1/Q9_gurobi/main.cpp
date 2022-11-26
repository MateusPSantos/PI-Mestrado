#include "gurobi_c++.h"
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

int **c,*f,*n,*m;

int stoi(string num){
    stringstream ss(num);
    int retorno=0;
    ss >> retorno;
    return retorno;
}
string itos(int i){
	stringstream s; 
	s << i; 
	return s.str(); 
}


// Funcao de Leitura das instâncias
void leitura(char *nome, int **c, int *f){
    string ler;
    ifstream arquivo(nome);
    
    arquivo >> ler;
    n=stoi(ler);
    
    arquivo >> ler;
    m=stoi(ler);
    c=new int*[n];
    for(int i=0;i<n;i++){
    	c[i]=new int[m];
        for(int j=0;j<m;j++){
            arquivo >> ler;
            c[i][j]=stoi(ler);
        }
    }
    f=new int[m];
    for(int i=0;i<m;i++){
        arquivo >>ler;
        f[i]=stoi(ler);
    }

    

    arquivo.close();

}





int main(int argc, char *argv[]){

	leitura(argv[1]);
	try {

		GRBEnv env = GRBEnv(true);
		env.set("LogFile","mip.log");
		env.start();

		GRBModel model = GRBModel(env);
        
        //variaveis

		GRBVar **x;
		GRBVar *y;

		x= new GRBVar*[n];
		for(int i=0; i<n; i++){
			x[i]=new GRBVar[m]; 
		}
		y=new GRBVar[n];

		for(int i=0; i<n; i++){
			for(int j=0;j<m;j++){
				x[i][j]=model.addVar(0.0,1.0,0.0,GRB_BINARY, "x["+itos(i)+"]"+"["+itos(j)+"]");
			}
		}

		for(int i=0; i<n; i++){
			y[i]=model.addVar(0.0,1.0,0.0,GRB_BINARY,"y["+itos(i)+"]");
		}
        

        //função objetivo
		GRBLinExpr FO=0;
		GRBLinExpr expr=0;
		for(int i=0;i<n;i++){
			for (int j = 0; j < m; j++){
				FO+=c[i][j]*x[i][j];
			}
		}
		for(int i=0; i < n ; i++){
			FO+=f[i]*y[i];
		}

		model.setObjective(FO, GRB_MINIMIZE);


		//Add constraint 
		for(int i=0;i<n;i++){
		    for(int j=0;j<m;j++){
		     	expr+=x[i][j];
		    }
		    model.addConstr(expr==1,"c("+itos(i)+")");
		    expr=0;
	    }
        int cont=n;
	    for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                 model.addConstr(x[i][j]<=y[j], "c("+itos(cont)+")");
                 cont++;
            }   
        }

        model.optimize();
        
        for(int i=0;i<n;i++){
        	for(int j=0;j<m;j++){
        		cout << x[i][j].get(GRB_StringAttr_VarName) << "="
                << x[i][j].get(GRB_DoubleAttr_X)<<" ";
        	}
        	cout<< endl;
        }
        for(int i=0;i<n;i++){
            cout << y[i].get(GRB_StringAttr_VarName) << "="
            << y[i].get(GRB_DoubleAttr_X)<< endl;
        }


    cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;



    model.write("model.lp");



  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }
	
	




	for(int i = 0; i < m; i++){
		delete[] c[i];
	}
    delete[] c;
    delete[] f;

    return 0;

}