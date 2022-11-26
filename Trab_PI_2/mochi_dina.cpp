#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

/*
*
* Declara as variáveis globais
*
*/
int *p; //lucro dos produtos
int *w; //peso dos produtos
int c; // capacidade da mochila
int n;
int **f,**b;
int *x;
/*
*
* Declara as funções
*
*/

void leitura(char *nome);

int fr(int r,int k);

void calcula_f();

void calcula_x(int r, int k);

void salva_result(char *nome);

/*
*
* função principal
*
*/

int main(int argc, char *argv[]){
	
    leitura(argv[1]);
    

    x=new int[n+1];
    calcula_f();
    
    calcula_x(n,c);
    /*cout<<"*******************"<<endl;
    cout<<"matriz"<<endl;
    cout<<"*******************"<<endl;
    for(int k=0; k <= c;k++){
       for(int r=0; r <=n ; r++){
       	 cout<<f[k][r]<<" ";
       }
       cout<<endl;
    }
    cout<<endl;
    */
    cout<<"*******************"<<endl;
    cout<<"vetor x"<<endl;
    cout<<"*******************"<<endl;
    for(int i=1; i <=n; i++){
    	cout<<"x["<<i<<"]="<<x[i]<<" ";
    }
    cout<<endl;
    cout<<"*******************"<<endl;
    cout<<"custo e peso"<<endl;
    cout<<"*******************"<<endl;
    int custo=0;
    int peso=0;

    for(int i=1;i<=n;i++){
    	if(x[i]==1){
    		custo+=p[i];
    		peso+=w[i];
    	}
    }
    cout<<"obj = "<<custo<<endl;
    cout<<"peso = "<<peso<<endl;

    salva_result(argv[1]);
    
    for(int i=0; i<=c; i++){
    	delete[] f[i];
    	delete[] b[i];
    }
    delete[] b;
    delete[] f;
    delete[] w;
    delete[] p;
	delete[] x;
	return 0;


}

int stoi(string num){
    stringstream ss(num);
    int retorno=0;
    ss >> retorno;
    return retorno;
}

void leitura(char *nome){
    string ler;

    ifstream arquivo(nome);
    
    arquivo>>ler;

    n=stoi(ler); // tamanho do problema

    p= new int[n+1]; // lucro dos produtos
    p[0]=0;
    for(int i=1;i<=n;i++){
    	arquivo >> ler;
    	p[i]=stoi(ler);
    }

    arquivo >> ler;

    c=stoi(ler); // capacidade da mochila

    w=new int[n+1];
    w[0]=0;
    for(int i=1;i<=n;i++){
    	arquivo >> ler;
    	w[i]=stoi(ler);
    }

    arquivo.close();

}

int max(int a, int b){
	if(a>=b){
		return a;
	}
	else{
		return b;
	}
}




int fr(int r, int k){
	if(w[r] > k){
		
		return f[k][r-1];
	}
	else{
		
		return max(f[k][r-1],p[r]+f[k-w[r]][r-1]);
	}


}


void calcula_f(){
    f = new int*[c+1];
    b = new int*[c+1];
    for(int i=0; i<=c; i++){
    	f[i]=new int[n+1];
    	b[i]=new int[n+1];
    }

    for(int k=0; k <= c;k++){
       	 f[k][0]=0;
       	 b[k][0]=0;
    }
    for(int r=0; r <=n ; r++){
    	f[0][r]=0;
    	b[0][r]=0;
    }
    

    for(int r=1; r <= n;r++){
       for(int k=1; k <=c ; k++){
       	 f[k][r]=fr(r,k);
       	 if(f[k][r]!=f[k][r-1]){
       	 	b[k][r]=1;

       	 }
       	 else{
       	 	b[k][r]=0;
       	 }
       }
    }


}

void calcula_x(int r,int k){
	
    if(r==0){
    	x[0]=0;
    }
    if(b[k][r]==1){
    	x[r]=1;
    	k=k-w[r];
    	r=r-1;

    	calcula_x(r,k);
    }
    else{
    	if(b[k][r]==0){
    		x[r]=0;
    	    calcula_x(r-1,k);
    	}
    }


	
}

void salva_result(char *nome){
    int soma=0;

    ofstream result;
    result.open("resultados_mochila_dina.txt", std::ofstream::out | std::ofstream::app);
    ofstream result1;
    result1.open("resultados_mochila_dina_vetor.txt", std::ofstream::out | std::ofstream::app);
    string linha;

    result1 << "begin{itemize}";

    result1 << "\n";
    result << nome;
    result << " \n";
    result1 << "item ";
    result1 << nome;
    result1 << "  ";
    result1 << "\n";

    for(int i=1;i<=n;i++){
		result1 << x[i];
		result1 <<"  ";

	}

	
	result1 << "\n";
	result1 <<" end{itemize}";
    
    for(int i=1;i<=n;i++){
    	if(x[i]!=0){
    		soma=soma+x[i]*p[i];
    	}
    }

    //result << "lucro total: ";
    result << soma;
    result << "  ";

    soma=0;
    for(int i=1;i<=n;i++){
    	if(x[i]!=0){
    		soma=soma+x[i]*w[i];
    	}
    }
    //result << "peso total: ";
    result << soma;
    result << "  ";
    //result << "x_h: ";
   
    result << "\n";
    

    result.close();
}
