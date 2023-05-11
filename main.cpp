#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

struct Registro
{
    string data;
    vector<string> acao;
    vector<int> quantidades;
};

struct Ocorrencia
{
    string nome;
    vector <string> datas;
    vector <int> quantidades;
};

void interface();
string formatarData(string data);
void leArquivo(string filename, map<string, vector<Ocorrencia>> &quesitos, map<string, vector<Registro>> &registros);
void gerarRegistroCrianca(map<string, vector<Registro>> &registros, string nome);
void listaPorQuesito(map<string, vector<Ocorrencia>> &quesitos, string nomeQuesito);
int buscaOcorrenciaPorAluno(vector<Ocorrencia> &ocorrencias, string nome);

int main() {
    vector<Ocorrencia> ocorrencias;

    int escolha;
    string filename, nomeQuesito, nomeCrianca;
    fstream pFile;

    // nomeQuesito, ocorrencia
    map<string, vector<Ocorrencia>> quesitos;

    // nomeCrianca, registro
    map<string, vector<Registro>> registros;

    do
    {
        interface();
        cin >> escolha;

        switch (escolha)
        {
            case 1:
                system("cls");
                cout << "<< 1 - Ler novo arquivo CSV >> " << endl;
                cout << "Entre com o nome do arquivo: ";
                cin >> filename;

                leArquivo(filename, quesitos, registros);

                cout << endl << ">> Pressione alguma tecla para voltar. ";
                getch();
                break;
            case 2:
                system("cls");
                cout << "<< 2 - Gerar relatorio por crianca >> " << endl;
                cout << "Digite o nome da crianca: ";
                cin.ignore(); // Limpa o buffer do cin

                // Pega uma string com espaço
                getline(cin, nomeCrianca);
                gerarRegistroCrianca(registros, nomeCrianca);

                cout << endl << ">> Pressione alguma tecla para voltar. ";
                getch();
                break;
            case 3:
                system("cls");

                cout << "<< 3 - Gerar relatorio por quesito >> " << endl;
                cout << "Digite o nome do quesito: ";
                cin.ignore(); // Limpa o buffer do cin

                // Pega uma string com espaço
                getline(cin, nomeQuesito);
                listaPorQuesito(quesitos, nomeQuesito);

                cout << endl << ">> Pressione alguma tecla para voltar. ";
                getch();
                break;
            case 4:
                cout << "\nFinalizando...";
                break;
            default:
                cout << "\nValor invalido!" << endl;
                getch();
                break;
        }
    }
    while(escolha != 4);

    return 0;
}

void interface()
{
    system("cls");
    cout << "1 - Ler novo arquivo CSV" << endl;
    cout << "2 - Gerar relatorio por crianca" << endl;
    cout << "3 - Gerar relatorio por quesito" << endl;
    cout << "4 - Encerrar o programa" << endl;
    cout << "Digite o numero associado ao menu: " << endl;
    cout << "> ";
}

void leArquivo(string filename, map<string, vector<Ocorrencia>> &quesitos, map<string, vector<Registro>> &registros)
{
    fstream pFile;
    pFile.open("../" + filename + ".csv", fstream ::in);

    string linha, palavra;

    // nomeCrianca, ocorrencia
    map<string, vector<Ocorrencia>>::iterator itq;

    // nomeCrianca, registro
    map<string, vector<Registro>>::iterator itr;

    if(pFile.is_open())
    {
        // linha
        while(getline(pFile, linha))
        {
            Ocorrencia ocorrencia;
            Registro registro;
            string nomeAluno;

            stringstream ss(linha);

            // Nome aluno
            getline(ss, nomeAluno, ',');

            // palavra
            while (getline(ss, palavra, ','))
            {
                string strNum;
                Ocorrencia oc;
                map<string, vector<Ocorrencia>>::iterator itq;

                registro.data = formatarData(filename);
                registro.acao.push_back(palavra);

                //numero
                getline(ss, strNum, ',');
                registro.quantidades.push_back(stoi(strNum));

                itq = quesitos.find(palavra);

                if(itq != quesitos.end())
                {
                    int pos = buscaOcorrenciaPorAluno(itq->second, nomeAluno);

                    if(pos != -1)
                    { // Insere um novo dado em um aluno existente
                        itq->second[pos].datas.push_back(formatarData(filename));
                        itq->second[pos].quantidades.push_back(stoi(strNum));
                    }
                    else
                    { // Insere um novo aluno em um vetor de ocorrências existente
                        oc.nome = nomeAluno;
                        oc.datas.push_back(formatarData(filename));
                        oc.quantidades.push_back(stoi(strNum));
                        itq->second.push_back(oc);
                    }
                }
                else
                {   // Caso o quesito não exista, cria e insere dados nele
                    oc.nome = nomeAluno;
                    oc.datas.push_back(formatarData(filename));
                    oc.quantidades.push_back(stoi(strNum));
                    quesitos[palavra].push_back(oc);
                }
            } // -- end while --

            registros[nomeAluno].push_back(registro);
        } // -- end while --

        pFile.close();
        cout << "Arquivo lido com sucesso!" << endl;
    }
    else
        cout << "Arquivo nao encontrado.";
}

string formatarData(string data)
{
    // Pega o dia, mês e ano da string
    string dia = data.substr(0, 2);
    string mes = data.substr(2, 2);
    string ano = data.substr(4, 4);

    return dia + "/" + mes + "/" + ano;
}

void listaPorQuesito(map<string, vector<Ocorrencia>> &quesitos, string nomeQuesito)
{
    map<string, vector<Ocorrencia>>::iterator itq;
    itq = quesitos.find(nomeQuesito);

    if(itq != quesitos.end())
    {
        cout
                << endl
                << "Nome do Quesito: "
                << itq->first
                << "\n\n"
                << "-------------------------------------------------------------"
                << endl;

        for(int i = 0; i < itq->second.size(); i++)
        {
            cout
                    << "Nome do aluno: " << itq->second[i].nome
                    << endl
                    << "Data da ocorrencia | Vezes ocorrido "
                    << endl;

            for(int j = 0; j < itq->second[i].datas.size(); j++)
            {
                cout << " > "
                     << itq->second[i].datas[j]
                     << " | "
                     << itq->second[i].quantidades[j]
                     << endl;
            }

            cout << endl;
        }

        cout
                << "-------------------------------------------------------------"
                << endl;
    }
    else
        cout << "Quesito nao encontrado. " << endl;
}

void gerarRegistroCrianca(map<string, vector<Registro>> &registros, string nome)
{
    map<string, vector<Registro>>::iterator ito;

    ito = registros.find(nome);

    if(ito != registros.end())
    {
        cout
                << endl
                << "Nome do aluno: "
                << ito->first
                << "\n\n"
                << "-------------------------------------------------------------"
                << endl;

        for(int i = 0; i < ito->second.size(); i++)
        {
            cout
                    << "Data: "
                    << ito->second[i].data << endl
                    << "Estatisticas: " << endl;

            for(int j = 0; j < ito->second[i].acao.size(); j++)
            {
                cout
                        << ito->second[i].acao[j] << ": "
                        << ito->second[i].quantidades[j] << endl;
            }

            cout << "\n\n";
        }

        cout
                << "-------------------------------------------------------------"
                << endl;
    }
    else
        cout << "Crianca nao encontrada. " << endl;
}

// Busca sequencial - verifica se o nome do quesito existe
int buscaOcorrenciaPorAluno(vector<Ocorrencia> &ocorrencias, string nome)
{
    for(int i = 0; i < ocorrencias.size(); i++)
    {
        if(ocorrencias[i].nome == nome)
            return i;
    }
    return -1;
}
