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
float *p; //lucro dos produtos
float *w; //peso dos produtos
float c; // capacidade da mochila
int n;
float *x;
float xk=0;
/*
*
* Declara as funções
*
*/

void leitura(char *nome);
void imprim_leitu();
void impri_resulta();
void Dantzig();
void salva_result(char *nome);

/*
*
* função principal
*
*/


int main(int argc, char *argv[]){

    leitura(argv[1]);

    x=new float[n];
    for(int i=0;i<n;i++){
    	x[i]=0;
    }
    //imprim_leitu();
    Dantzig();
    //impri_resulta();
    salva_result(argv[1]);


    

	return 0;

}


/*
*
* Funcões
*
*/

int numero(string num){
    stringstream ss(num);
    int retorno=0;
    ss >> retorno;
    return retorno;
}

void leitura(char *nome){
    string ler;

    ifstream arquivo(nome);
    
    arquivo>>ler;

    n=numero(ler); // tamanho do problema

    p= new float[n]; // lucro dos produtos

    for(int i=0;i<n;i++){
    	arquivo >> ler;
    	p[i]=numero(ler);
    }

    arquivo >> ler;

    c=numero(ler); // capacidade da mochila

    w=new float[n];

    for(int i=0;i<n;i++){
    	arquivo >> ler;
    	w[i]=numero(ler);
    }

    arquivo.close();

}

void imprim_leitu(){
	cout<<"*************************"<<endl;
	cout<<"DADOS DA INSTANCIA"<<endl;
	cout<<"*************************"<<endl;
	cout<<"tamanho do problema: "<<n<<endl;
    cout<<endl;

    cout<<"lucros"<<endl;

    for(int i=0;i<n;i++){
    	cout<<p[i]<<" ";
    }
    cout<<endl;

    cout<<"capacidade do problema: "<<c<<endl;
    cout<<endl;
    
    cout<<"pesos"<<endl;

    for(int i=0;i<n;i++){
    	cout<<w[i]<<" ";
    }
    cout<<endl;
    cout<<endl;
    cout<<endl;

}

void impri_resulta(){
	cout<<"*************************"<<endl;
	cout<<"RESULTADO"<<endl;
	cout<<"*************************"<<endl;
	cout<<"Valores de x"<<endl;


	int cont=0;

	for(int i=0;i<n;i++){
		cout<<"x["<<i+1<<"]="<<x[i]<<"  ";
		if(cont==10){
			cont=0;
			cout<<endl;
		}

	}
	cout<<endl;
    
    float soma=0;
    for(int i=0;i<n;i++){
    	if(x[i]!=0){
    		soma=soma+x[i]*p[i];
    	}
    }


	cout<<"Lucro total: "<<soma<<endl;
	soma=0;
    for(int i=0;i<n;i++){
    	if(x[i]!=0){
    		soma=soma+x[i]*w[i];
    	}
    }
    cout<<"Peso total: "<<soma<<endl;
	cout<<"x_h: "<<xk<<endl;
	


}
void salva_result(char *nome){
	float soma=0;

	ofstream result;
    result.open("resultados_mochila.txt", std::ofstream::out | std::ofstream::app);
    ofstream result1;
    result1.open("resultados_mochila_vetor.txt", std::ofstream::out | std::ofstream::app);
    string linha;

    result1 << "begin{itemize}";

    result1 << "\n";
    result << nome;
    result << "  ";
    result1 << "item ";
    result1 << nome;
    result1 << "  ";
    result1 << "\n";

    for(int i=0;i<n;i++){
		result1 << x[i];
		result1 <<"  ";

	}
	result << "\n";
	result1 << "\n";
	result1 << "\n";
	result1 <<" end{itemize}";
    
    for(int i=0;i<n;i++){
    	if(x[i]!=0){
    		soma=soma+x[i]*p[i];
    	}
    }

    //result << "lucro total: ";
    result << soma;
    result << "  ";

    soma=0;
    for(int i=0;i<n;i++){
    	if(x[i]!=0){
    		soma=soma+x[i]*w[i];
    	}
    }
    //result << "peso total: ";
    result << soma;
    result << "  ";
    //result << "x_h: ";
    result << xk;
    result << "\n";    
    //result << "\n";
    

    result.close();
}

int *indices;
float *divisoes;


void bolha(){
    for(int i = 0; i < n; i++ ){
        for(int j = i + 1; j < n; j++ ){
        	if ( divisoes[j] > divisoes[i] ){
                float aux = divisoes[j];
                divisoes[j] = divisoes[i];
                divisoes[i] = aux;

                int aux1 = indices[j];
                indices[j]=indices[i];
                indices[i]= aux1;

            }
        }
    } 
}

void Dantzig(){
	indices=new int[n];
    divisoes=new float[n];
	for(int i=0;i<n;i++){
       indices[i]=i;
       divisoes[i]=p[i]/w[i];
       
	}

    bolha();



    float soma=0;

    for(int i=0;i<n;i++){

    	if(soma+w[indices[i]]> c){
    		xk=(c-soma)/w[indices[i]];
            x[indices[i]]=xk;
    		break;
    	}
    	else{
    		x[indices[i]]=1;
    		soma=soma+w[indices[i]];

    	}

    }

    /*for(int i=0;i<n;i++){
    	if(x[indices[i]]==0){
    		if(soma+w[indices[i]] <= c){
    			x[indices[i]]=1;
    			soma=soma+w[indices[i]];
    		}
    	}
    }*/


}


