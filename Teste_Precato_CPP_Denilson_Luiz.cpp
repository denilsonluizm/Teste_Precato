#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
using namespace std; 

struct PAGAMENTO {
    string id_remessa;
    string id_credor;
    string id_ente_devedor;
    float valor_inicial;
    float valor_final;
    time_t data;
    string status_remessa;
    string motivo;
};

struct CREDOR {
    string id_credor;
    string nome_credor;
    string cpf_credor;
    string status_cadastro;
};

struct ENTE_DEVEDOR {
    string id_ente_devedor;
    string nome_ente_devedor;
    string cnpj_ente_devedor;
};

void API_SolicitaPagamento(string pagamento, string credor, string devedor, PAGAMENTO &p);
string Onde(fstream &dados, string cabecaA, string valorA, string cabecaB);
bool ExisteID(fstream &dados, string valor);
void CadastraCredor(string credor);
void CadastraEnteDevedor(string devedor);
void ExcluiCadastro(string arquivo, string valor, int coluna);

int main(){

    int escolha=10;
    string id_excluida;
    bool saida=true;
    PAGAMENTO p;

    while(saida){

        cout << "\n---------- MENU ----------" << endl;
        cout << "1 - Solicitar Pagamento" << endl;
        cout << "2 - Cadastrar Credor" << endl;
        cout << "3 - Cadastrar Ente Devedor" << endl;
        cout << "4 - Excluir Solicitacao de Pagamento" << endl;
        cout << "5 - Excluir Credor" << endl;
        cout << "6 - Excluir Ente Devedor" << endl;
        cout << "0 - Sair" << endl;
        cin >> escolha;

        switch (escolha)
        {
        case 0:
            cout << "Saida do Programa!" << endl;
            saida = false;
            break;
        case 1:
            cout << "ID Remessa: ";
            cin >> p.id_remessa;
            cout << "ID Credor: ";
            cin >> p.id_credor;
            cout << "ID Ente Devedor: ";
            cin >> p.id_ente_devedor;
            cout << "Valor Inicial: ";
            cin >> p.valor_inicial;
            cout << "Valor Final: ";
            cin >> p.valor_final;

            API_SolicitaPagamento("PAGAMENTO.csv", "CREDOR.csv", "ENTE_DEVEDOR.csv", p);
            break;
        case 2:
            CadastraCredor("CREDOR.csv");
            break;
        case 3:
            CadastraEnteDevedor("ENTE_DEVEDOR.csv");
            break;
        case 4:
            cout << "ID da Remessa a exluir: ";
            cin >> id_excluida;
            ExcluiCadastro("PAGAMENTO.csv", id_excluida, 8);
            break;
        case 5:
            cout << "ID do Credor a exluir: ";
            cin >> id_excluida;
            ExcluiCadastro("CREDOR.csv", id_excluida, 4);
            break;
        case 6:
            cout << "ID do Ente Devedor a exluir: ";
            cin >> id_excluida;
            ExcluiCadastro("ENTE_DEVEDOR.csv", id_excluida, 3);
            break;

        }  // fim switch

    }  // fim while

return 0;
}

//------------FUNCOES------------//

// retorna valor de uma linha dado um valor referencia, ex: dado um ID retorna cpf referente
string Onde(fstream &dados, string cabecaA, string valorA, string cabecaB){

    int cont=0;
    int a, b, c, d;
    string nome;
    string resultado;

    while(!dados.eof()){  // loop leitura do arquivo
        getline(dados, nome, ',');

        cont++;  // conta elemento por elemento entre virgulas

        if(nome==cabecaA) {
            a = cont;  // registra posicao da coluna valor referencia
            if(a==1) valorA = "\n" + valorA;
            }
        if(nome==cabecaB) b = cont;  // registra posicao da coluna valor buscado
        if(nome==valorA) {
            c = b-a;
            d = cont;
        }
        if(cont==(d+c)) resultado = nome;  // registra valor buscado
    }
    return resultado;
}

// retorna true caso ID informado exista na tabela
bool ExisteID(fstream &dados, string valor){

    int cont=0;
    string nome;
    valor = "\n" + valor;

    while(!dados.eof()){  // loop leitura do arquivo
        getline(dados, nome, ',');

        if(nome==valor) {  // se id encontrada cont incrementa
            cont++;
            }

    }
    if(cont > 0) return true;
    else return false;
}

// registra a solicitacao de pagamento
void API_SolicitaPagamento(string pagamento, string credor, string devedor, PAGAMENTO &p)
{

    fstream dados_pagamento;
    fstream dados_credor;
    fstream dados_devedor;

    // abrindo arquivos e ponteiro de escrita/leitura colocado no inicio do arquivo
    dados_pagamento.open(pagamento, ios::in | ios::out | ios::app);
    dados_pagamento.seekp(ios::beg);
    dados_devedor.open(devedor, ios::in | ios::out | ios::app);
    dados_devedor.seekp(ios::beg);

    string status_cadastro;
    bool existe_devedor;
    bool existe_remessa;
    bool existe_credor;

    existe_remessa = ExisteID(dados_pagamento, p.id_remessa);

    dados_pagamento.close();  // fecha arquivo
    dados_pagamento.open(pagamento, ios::in | ios::out | ios::app);  // reabre arquivo para escrita
    dados_pagamento.seekp(ios::beg);

    // if's aninhados verificando condicoes do problema
    if(existe_remessa==true){  // verifica Regra 5
        p.status_remessa = "INVALIDA";
        p.motivo = "ID_REMESSA_EXISTENTE";
    }
    else{ // else1
        dados_pagamento.close();
        dados_pagamento.open(pagamento, ios::in | ios::out | ios::app);
        dados_pagamento.seekp(ios::beg);

        dados_credor.open(credor, ios::in | ios::out | ios::app);
        dados_credor.seekp(ios::beg);
        existe_credor = ExisteID(dados_credor, p.id_credor);

        if(existe_credor==false){
            p.status_remessa = "INVALIDA";
            p.motivo = "CREDOR_INEXISTENTE";
        }
        else{  // else2
            dados_credor.close();
            dados_credor.open(credor, ios::in | ios::out | ios::app);
            dados_credor.seekp(ios::beg);

            status_cadastro = Onde(dados_credor, "id_credor", p.id_credor, "status_cadastro");

            if(status_cadastro=="R"){  // verifica Regra 1
                p.status_remessa = "INVALIDA";
                p.motivo = "CADASTRO_REPROVADO"; 
            }
            else{  // else3

                existe_devedor = ExisteID(dados_devedor, p.id_ente_devedor);

                if(existe_devedor==false){  // veriica Regra 2
                    p.status_remessa = "INVALIDA";
                    p.motivo = "ENTE_DEVEDOR_INEXISTENTE";
                }
                else{  // else4

                    if(p.valor_inicial < 0 || p.valor_final < 0 || p.valor_final > p.valor_inicial){  // verifica Regra 3 e 4
                        p.status_remessa = "INVALIDA";
                        p.motivo = "VALORES_INCONSISTENTES";
                    }
                    else{  // else5
                        p.status_remessa = "VALIDA";
                        p.motivo = "";
                    }  // fim else5

                }  // fim else4

            }  // fim else3     

        }  // fim else2      

    }  // fim else 1

    // registra data em timestamp
    p.data = time(nullptr);
    // grava solicitacao da remessa no arquivo de dados
    dados_pagamento << p.id_remessa << "," << p.id_credor << "," << p.id_ente_devedor << "," << p.valor_inicial << "," << p.valor_final << "," << p.data << "," << p.status_remessa << "," << p.motivo << "," << "\n";
    
    // fechando os arquivos
    dados_pagamento.close();
    dados_credor.close();
    dados_devedor.close();
}

// cadastra um credor na tabela
void CadastraCredor(string credor)
{
    fstream dados_credor;

    // abrindo arquivos e ponteiro de escrita/leitura colocado no inicio do arquivo
    dados_credor.open(credor, ios::in | ios::out | ios::app);
    dados_credor.seekp(ios::beg);

    bool existe_credor;
    CREDOR c;

    dados_credor.seekp(ios::beg);

    cout << "ID do Credor: ";
    cin >> c.id_credor;

    existe_credor = ExisteID(dados_credor, c.id_credor);

    // loop para impedir ID repetida
    while(existe_credor==true){
        dados_credor.close();
        dados_credor.open(credor, ios::in | ios::out | ios::app);
        dados_credor.seekp(ios::beg);
        cout << "ID de credor jah existe!" << endl;
        cout << "ID do Credor: ";
        cin >> c.id_credor;
        existe_credor = ExisteID(dados_credor, c.id_credor);
    }

    dados_credor.close();
    dados_credor.open(credor, ios::in | ios::out | ios::app);

    cout << "Nome do Credor: ";
    cin >> c.nome_credor;
    cout << "CPF do Credor (apenas numeros): ";
    cin >> c.cpf_credor;
    cout << "Status do cadastro - ";
    cout << "Digite 'A' para aprovado OU 'R' para reprovado: ";
    cin >> c.status_cadastro;
    while (c.status_cadastro!="A" && c.status_cadastro!="R"){
        cout << "Entrada Invalida!" << endl;
        cout << "Digite 'A' para aprovado OU 'R' para reprovado: ";
        cin >> c.status_cadastro;
    }
    
    // grava dados no arquivo
    dados_credor << c.id_credor << "," << c.nome_credor << "," << c.cpf_credor << "," << c.status_cadastro << "," << "\n";

    dados_credor.close();
}

// cadastra ente devedor na tabela
void CadastraEnteDevedor(string devedor)
{
    fstream dados_devedor;

    // abrindo arquivos e ponteiro de escrita/leitura colocado no inicio do arquivo
    dados_devedor.open(devedor, ios::in | ios::out | ios::app);
    dados_devedor.seekp(ios::beg);

    ENTE_DEVEDOR d;
    bool existe_devedor;

    cout << "ID Ente Devedor: ";
    cin >> d.id_ente_devedor;

    existe_devedor = ExisteID(dados_devedor, d.id_ente_devedor);

    // loop para impedir ID repetida
    while(existe_devedor==true){
        dados_devedor.close();
        dados_devedor.open(devedor, ios::in | ios::out | ios::app);
        dados_devedor.seekp(ios::beg);
        cout << "ID de devedor jah existe!" << endl;
        cout << "ID Ente Devedor: ";
        cin >> d.id_ente_devedor;
        existe_devedor = ExisteID(dados_devedor, d.id_ente_devedor);
    }

    dados_devedor.close();
    dados_devedor.open(devedor, ios::in | ios::out | ios::app);

    cout << "Nome Ente Devedor: ";
    cin >> d.nome_ente_devedor;
    cout << "CNPJ (apenas numeros): ";
    cin >> d.cnpj_ente_devedor;

    // gravando dados na tabela
    dados_devedor << d.id_ente_devedor << "," << d.nome_ente_devedor << "," << d.cnpj_ente_devedor << "," << "\n";

    dados_devedor.close();
}

// exclui uma linha da tabela
void ExcluiCadastro(string arquivo, string valor, int coluna){

    fstream dados;
    bool existe_ID;

    // abre arquivo
    dados.open(arquivo, ios::in | ios::out | ios::app);
    dados.seekp(ios::beg);

    existe_ID = ExisteID(dados, valor);

    if(existe_ID==false){  // encerra caso dado nao exista
        cout << "Dado Inexistente!" << endl;
        dados.close();
    }
    else{
        dados.close();
        dados.open(arquivo, ios::in | ios::out | ios::app);
        dados.seekp(ios::beg);

        int cont=0, cont1=0, linha_delete, linha=0;
        string nome;
        valor = "\n" + valor;

        // conta as linhas
        while(!dados.eof()){
            linha++;
            getline(dados, nome, '\n');
        }
        dados.close();

        dados.open(arquivo, ios::in | ios::out | ios::app);
        dados.seekp(ios::beg);

        string M[linha];
        int i=0;    

        // copia o arquivo .csv para um vetor M[]
        while(!dados.eof()){
            getline(dados, nome, '\n');
            M[i] = nome;
            i++;
        }
        M[linha] = "\n";
        dados.close();

        dados.open(arquivo, ios::in | ios::out | ios::app);
        dados.seekp(ios::beg);

        // localiza a linha a ser deletada
        while(!dados.eof()){
            cont1++;
            getline(dados, nome, ',');
            if(nome==valor) {
                cont = cont1;
                }
        }
        linha_delete = ((cont-1)/coluna);
        dados.close();

        dados.open(arquivo, ios::out);
        dados.seekp(ios::beg);    

        // reescreve o arquivo sem a linha a ser deletada
        for(int j=0; j<(linha-1); j++){
            if(j != linha_delete) dados << M[j] << "\n";
        }
        dados.close();
    }
}
