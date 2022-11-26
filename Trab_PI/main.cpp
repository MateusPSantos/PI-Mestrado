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

int n; //itens

int *p, *w,*sol, c;




double stoi(string num){
    stringstream ss(num);
    double retorno=0;
    ss >> retorno;
    return retorno;
}
string itos(int i){
	stringstream s; 
	s << i; 
	return s.str(); 
}

void leitura(char *nome);
void salva_result(char *nome);



int main(int argc, char *argv[]){

	leitura(argv[1]);
	 
   

  try{
      
  
      // Create an environment
      GRBEnv env = GRBEnv(true);
      env.set("LogFile", "mip.log");
      env.start();

      //Create an empty model
      GRBModel model = GRBModel(env);
      
      //Create variables
    
      GRBVar x[n];

      for(int i=0; i<n; i++){
        x[i] = model.addVar(0.0,GRB_INFINITY,0.0, GRB_INTEGER,"x("+itos(i+1)+")");
   
      }


      
      /*objective function*/
      GRBLinExpr FO=0;
      GRBLinExpr expr=0;
      for(int i=0; i<n; i++){
        FO += p[i]*x[i];
	    }
      model.setObjective(FO, GRB_MAXIMIZE);

      /*constraints*/      
    for (int i=0; i<n; i++){
	      expr+=w[i]*x[i];
    }
    model.addConstr(expr<=c);
      
    
    
    model.set(GRB_DoubleParam_TimeLimit, 3600);
    

    model.optimize();
    sol=new int[n];
    for(int i=0;i<n;i++){

        cout << x[i].get(GRB_StringAttr_VarName) << "="
             << x[i].get(GRB_DoubleAttr_X)<<" ";
        sol[i]=x[i].get(GRB_DoubleAttr_X);
        	
    }
    cout<<endl;
    /*for(int i=0;i<n;i++){

        cout << xp[i].get(GRB_StringAttr_VarName) << "="
             << xp[i].get(GRB_DoubleAttr_X)<<" ";
    }
    cout<<endl;

    for(int i=0;i<n;i++){

        cout << yp[i].get(GRB_StringAttr_VarName) << "="
             << yp[i].get(GRB_DoubleAttr_X)<<" ";
    }
    cout<<endl;

    for(int i=0;i<n;i++){

        cout << sr[i].get(GRB_StringAttr_VarName) << "="
             << sr[i].get(GRB_DoubleAttr_X)<<" ";
    }
    cout<<endl;
    */

    cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;



    model.write("model.lp");
 
      
      //cplex.out() << "Solution status: " << cplex.getStatus() << endl;
      
      /*get solution*/
  
      
    }
  catch (GRBException e){
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
    }
  catch(...){
      
    cout << "Exception during optimization" << endl;
    }
  
    salva_result(argv[1]);
    delete[] p;
    delete[] w;
    delete[] sol;


	


	return 0;


}


void leitura(char *nome){
    string ler;

    ifstream arquivo(nome);
    
    arquivo>>ler;

    n=stoi(ler); // tamanho do problema

    p= new int[n]; // lucro dos produtos

    for(int i=0;i<n;i++){
      arquivo >> ler;
      p[i]=stoi(ler);
    }

    arquivo >> ler;

    c=stoi(ler); // capacidade da mochila

    w=new int[n];

    for(int i=0;i<n;i++){
      arquivo >> ler;
      w[i]=stoi(ler);
    }

    arquivo.close();

}


void salva_result(char *nome){
  float soma=0;

  ofstream result;
    result.open("resultados_mochila_gurobi.txt", std::ofstream::out | std::ofstream::app);
    ofstream result1;
    result1.open("resultados_mochila_vetor_gurobi.txt", std::ofstream::out | std::ofstream::app);
    string linha;


    result << nome;
    result << "  ";
    result << "\n";
    result1 << nome;
    result1 << "  ";
    result1 << "\n";

    for(int i=0;i<n;i++){
    result1 << sol[i];
    result1 <<"  ";

  }
  result << "\n";
  result1 << "\n";
    
    for(int i=0;i<n;i++){
      if(sol[i]!=0){
        soma=soma+sol[i]*p[i];
      }
    }

    //result << "lucro total: ";
    result << soma;
    result << "  ";

    soma=0;
    for(int i=0;i<n;i++){
      if(sol[i]!=0){
        soma=soma+sol[i]*w[i];
      }
    }
    //result << "peso total: ";
    result << soma;
    result << "  ";
    result << "\n";
    //result << "x_h: ";
 
    //result << "\n";
    

    result.close();
}
