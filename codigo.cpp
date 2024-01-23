#include <iostream>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;



int gerarNumeroAleatorio() {
    static bool sementeInicializada = false;
    if (!sementeInicializada) {
        srand(time(0));
        sementeInicializada = true;
    }
return rand() % 7;
}

typedef struct node {
    int tabuleiro[6][7];
    char estado;
    node* p[7];
} Node;

int verificaVencedor(Node* arvore, int timeX);

int proxLinhaLivre(Node* arvore, int coluna);

void insereElemento(Node* &arvore, int coluna, int timeX);

bool temElemento(Node* arvore, int linha, int coluna);

void cria(Node* &arvore) {

    arvore = new Node;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            arvore->tabuleiro[i][j] = 0;
        }
    }

    for(int i=0;i<7;i++){
        arvore->p[i] = nullptr;
    }
}

void gera(Node* &arvore){
    int state = verificaVencedor(arvore, 2);
    
    switch (state)
    {
    case 2:
        arvore->estado = 'V';
        break;
    case 1:
        arvore->estado = 'D';
        break;
    case 3:
        arvore->estado = 'E';
        break;
    case 0:
        arvore->estado = '?';
    default:
        break;
    }

    cout << arvore->estado;
    if(arvore->estado == 'V' || arvore->estado == 'D'){
       for(int i=0;i<7;i++){
            arvore->p[i] = nullptr;
        }
        
    }else{
        for(int i=0;i<7;i++){   
            if(proxLinhaLivre(arvore->p[i], i) != -1){ 
                cout<<"joooj";
                arvore->p[i] = new Node;
                for(int a=0;a<6;a++)
                    for(int b=0;b<7;b++)
                        arvore->p[i]->tabuleiro[a][b] = arvore->tabuleiro[a][b];
                insereElemento(arvore->p[i],i,2);  
                gera(arvore->p[i]); 
            }else{
                arvore->p[i] = nullptr;
                cout<<"joooj";
            }
        }
    }
}



void destroi(Node* &arvore) {
    for(int i=0;i<7;i++){
        if(arvore->p[i] != nullptr){
            destroi(arvore->p[i]);
        }
    }

    delete arvore;
    arvore = nullptr;
}

void printTabuleiro(Node* arvore) {
    cout << "(0) (1) (2) (3) (4) (5) (6)" << endl;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            cout << " " << arvore->tabuleiro[i][j] << " ";
        }
    cout << endl;
    }

    cout << endl;
    }

bool temElemento(Node* arvore, int linha, int coluna) {
    return (arvore->tabuleiro[linha][coluna] != 0);
}

int proxLinhaLivre(Node* arvore, int coluna) {
    int linha = 5;
    while (linha >= 0) {
        if (!temElemento(arvore, linha, coluna))
            return linha;
        else
            linha--;
    }
    return -1;
}

void insereElemento(Node* &arvore, int coluna, int timeX) {
    if (proxLinhaLivre(arvore, coluna) != -1)
        arvore->tabuleiro[proxLinhaLivre(arvore, coluna)][coluna] = timeX;
}

int verificaVencedor(Node* arvore, int timeX) {
    // Verificar vitória na horizontal
    for (int linha = 0; linha < 6; linha++) {
        for (int coluna = 0; coluna < 4; coluna++) {
            if (arvore->tabuleiro[linha][coluna] == timeX &&
            arvore->tabuleiro[linha][coluna + 1] == timeX &&
            arvore->tabuleiro[linha][coluna + 2] == timeX &&
            arvore->tabuleiro[linha][coluna + 3] == timeX) {
                return timeX;
            }
        }
    }

    // Verificar vitória na vertical
    for (int coluna = 0; coluna < 7; coluna++) {
        for (int linha = 0; linha < 3; linha++) {
            if (arvore->tabuleiro[linha][coluna] == timeX &&
            arvore->tabuleiro[linha + 1][coluna] == timeX &&
            arvore->tabuleiro[linha + 2][coluna] == timeX &&
            arvore->tabuleiro[linha + 3][coluna] == timeX) {
                return timeX;
            }
        }
    }

    //verifica vitoria na diagonal para baixo
    for (int linha = 0; linha < 3; linha++) {
        for (int coluna = 0; coluna < 4; coluna++) {
            if (arvore->tabuleiro[linha][coluna] == timeX &&
            arvore->tabuleiro[linha + 1][coluna + 1] == timeX &&
            arvore->tabuleiro[linha + 2][coluna + 2] == timeX &&
            arvore->tabuleiro[linha + 3][coluna + 3] == timeX) {
                return timeX;
            }
        }
    }

    // Verificar vitória na diagonal para cima
    for (int linha = 3; linha < 6; linha++) {
        for (int coluna = 0; coluna < 4; coluna++) {
            if (arvore->tabuleiro[linha][coluna] == timeX &&
            arvore->tabuleiro[linha - 1][coluna + 1] == timeX &&
            arvore->tabuleiro[linha - 2][coluna + 2] == timeX &&
            arvore->tabuleiro[linha - 3][coluna + 3] == timeX) {
                return timeX;
            }
        }
    }

    // Verificar empate
    int linhasCheias = 0;
    for (int j=0; j<7; j++)
        if (proxLinhaLivre(arvore, j) == -1)
            linhasCheias++;
    if(linhasCheias == 6)
        return 3;
    return 0; // Nenhum time venceu ainda
}

int escolherJogadaComputador(Node* arvore){
    //verifica se é possível vencer na próxima jogada
    for(int coluna = 0; coluna < 7; coluna++) {
        if(proxLinhaLivre(arvore, coluna) != -1) {
            // Faça uma cópia temporária do tabuleiro
            Node* temp = new Node(*arvore);
            insereElemento(temp, coluna, 2);
            if(verificaVencedor(temp, 2) == 2) {
                delete temp;
                return coluna;
            }
            delete temp;
        }
    }

    // verificar se é necessário bloquear o oponente de vencer
    for(int coluna = 0; coluna < 7; coluna++) {
        if(proxLinhaLivre(arvore, coluna) != -1) {
            // Faça uma cópia temporária do tabuleiro
            Node* temp = new Node(*arvore);
            insereElemento(temp, coluna, 1);
            if(verificaVencedor(temp, 1) == 1) {
                delete temp;
                return coluna;
            }
            delete temp;
        }
    }
    int proxjogada;
    bool x = true;
    while(x == true){
        proxjogada = gerarNumeroAleatorio();
        if(proxLinhaLivre(arvore, proxjogada) != -1){
            return proxjogada;
            x = false;
        }
    }
}

int main() {
    Node* Arvore;
    cria(Arvore);
    gera(Arvore);
    
    /*while((verificaVencedor(Arvore, 1) != 1)&&(verificaVencedor(Arvore, 2) !=2)&&(verificaVencedor(Arvore, 1) != 3)) {
        int colunaSelecionada;
        cout << "Selecione uma coluna (0 a 6): " << endl;
        cin >> colunaSelecionada;
        insereElemento(Arvore, colunaSelecionada, 1);
        printTabuleiro(Arvore);
        insereElemento(Arvore, escolherJogadaComputador(Arvore), 2);
        printTabuleiro(Arvore);
    }
    cout << "Fim de Jogo." << endl;
    destroi(Arvore);*/
    return 0;
}

