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
    int coluna_jogada;
    int score;
    struct node* p[7];
    struct node* pai;
} Node;

int verificaVencedor(Node* arvore, int timeX);

int proxLinhaLivre(Node* arvore, int coluna);

void insereElemento(Node* &arvore, int coluna, int timeX);

bool temElemento(Node* arvore, int linha, int coluna);

void cria(Node* &arvore) {

    arvore = new Node;

    arvore->pai = nullptr;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            arvore->tabuleiro[i][j] = 0;
        }
    }

    for(int i=0;i<7;i++){
        arvore->p[i] = nullptr;
    }
}

void gera(Node* &arvore, int depth, int MaxDepth,int time){
    
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
    
    if((arvore->estado == 'V' || arvore->estado == 'D') || depth > MaxDepth){
       for(int i=0;i<7;i++){
            arvore->p[i] = nullptr;
        }
    }else{
        for(int i=0;i<7;i++){   
            if(proxLinhaLivre(arvore, i) != -1){ 
                arvore->p[i] = new Node;
                arvore->p[i]->pai = arvore;
                for(int a=0;a<6;a++)
                    for(int b=0;b<7;b++)
                        arvore->p[i]->tabuleiro[a][b] = arvore->tabuleiro[a][b];
                insereElemento(arvore->p[i],i,time);  
                arvore->coluna_jogada = i;
                gera(arvore->p[i], depth + 1, MaxDepth, time == 2 ? 1 : 2);
            }else{
                arvore->p[i] = nullptr;
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


int calculaScore(Node* arvore, int timeX){
    int score=0;
    
     // Verificar vitória na horizontal
    for (int linha = 0; linha < 6; linha++) {
        for (int coluna = 0; coluna < 4; coluna++) {
            if (arvore->tabuleiro[linha][coluna] == timeX && arvore->tabuleiro[linha][coluna + 1] == timeX && arvore->tabuleiro[linha][coluna + 2]) {
                if(arvore->tabuleiro[linha][coluna + 3]  == 0) score +=9;
            }else if(arvore->tabuleiro[linha][coluna] == timeX && arvore->tabuleiro[linha][coluna + 1] == timeX){
                if(arvore->tabuleiro[linha][coluna + 2]  == 0)score += 3;
            }else if(arvore->tabuleiro[linha][coluna] == timeX){
                if(arvore->tabuleiro[linha][coluna + 1]  == 0)score+=1;
            }
        }
    }

    // Verificar vitória na vertical
    for (int coluna = 0; coluna < 7; coluna++) {
        for (int linha = 0; linha < 3; linha++) {
            if (arvore->tabuleiro[linha][coluna] == timeX && arvore->tabuleiro[linha+1][coluna] == timeX && arvore->tabuleiro[linha+2][coluna]) {
                if(arvore->tabuleiro[linha+3][coluna]  == 0) score +=9;
            }else if(arvore->tabuleiro[linha][coluna] == timeX && arvore->tabuleiro[linha+1][coluna] == timeX){
                if(arvore->tabuleiro[linha+2][coluna]  == 0)score += 3;
            }else if(arvore->tabuleiro[linha][coluna] == timeX){
                if(arvore->tabuleiro[linha+1][coluna]  == 0)score+=1;
            }
        }
    }

    //verifica vitoria na diagonal para baixo
    for (int linha = 0; linha < 3; linha++) {
        for (int coluna = 0; coluna < 4; coluna++) {
            if (arvore->tabuleiro[linha][coluna] == timeX &&arvore->tabuleiro[linha + 1][coluna + 1] == timeX &&arvore->tabuleiro[linha + 2][coluna + 2] == timeX) {
                if(arvore->tabuleiro[linha + 3][coluna + 3] == 0) score+=9;
            }else if(arvore->tabuleiro[linha][coluna] == timeX &&arvore->tabuleiro[linha + 1][coluna + 1] == timeX){
                if(arvore->tabuleiro[linha +2][coluna + 2] == 0) score+=3;
            }else if(arvore->tabuleiro[linha][coluna] == timeX){
                if(arvore->tabuleiro[linha + 1][coluna + 1] == 0) score+=1;
            }
        }
    }

    // Verificar vitória na diagonal para cima
    for (int linha = 3; linha < 6; linha++) {
        for (int coluna = 0; coluna < 4; coluna++) {
            if (arvore->tabuleiro[linha][coluna] == timeX &&arvore->tabuleiro[linha - 1][coluna + 1] == timeX &&arvore->tabuleiro[linha - 2][coluna + 2] == timeX) {
               if(arvore->tabuleiro[linha - 3][coluna + 3] == 0) score+=9;
            }else if(arvore->tabuleiro[linha][coluna] == timeX &&arvore->tabuleiro[linha - 1][coluna + 1] == timeX){
                if(arvore->tabuleiro[linha - 2][coluna + 2] == 0) score+=3;
            }else if(arvore->tabuleiro[linha][coluna] == timeX){
                if(arvore->tabuleiro[linha - 1][coluna + 1] == 0) score+=1;
            }
        }
    }

    return score;
}

Node* apontaMaiorScore(Node* arvore, int &x, Node* &temp){
    if(arvore != NULL){
        for(int i=0;i<7;i++){
            if(x < (calculaScore(arvore->p[i], 2) - calculaScore(arvore->p[i],1) ))
                x = (calculaScore(arvore->p[i], 2) - calculaScore(arvore->p[i],1));
                temp = arvore->p[i];
        }
    }
    return temp;
}

int escolhaDaJogada2EletricBoogaloo(Node* temp){
    Node* aux;
    aux = temp;
    while(temp != nullptr){
        aux = temp;
        temp = temp->pai;
    }
    return aux->coluna_jogada;
}

int main() {
    Node* Arvore;
    cria(Arvore);
    int x=0;
    Node* temp=nullptr;;
    
    while((verificaVencedor(Arvore, 1) != 1)&&(verificaVencedor(Arvore, 2) !=2)&&(verificaVencedor(Arvore, 1) != 3)) {
        int colunaSelecionada;
        cout << "Selecione uma coluna (0 a 6): " << endl;
        cin >> colunaSelecionada;
        insereElemento(Arvore, colunaSelecionada, 1);
        printTabuleiro(Arvore);
        if(verificaVencedor(Arvore, 1) == 1 || verificaVencedor(Arvore, 1) == 3) break;
        gera(Arvore, 0, 6, 2);
        apontaMaiorScore(Arvore, x, temp);
        insereElemento(Arvore, escolhaDaJogada2EletricBoogaloo(Arvore), 2);
        x=0;
        temp = nullptr;
        for(int i=0;i<7;i++){
            if(Arvore->p[i] != nullptr){
                destroi(Arvore->p[i]);
            }
        }
        printTabuleiro(Arvore);
    }
    cout << "fim de jogo!" << endl;
    if(verificaVencedor(Arvore,1)==1){
        cout << "Você ganhou!!" << endl;
    }else if(verificaVencedor(Arvore,2)==2){
        cout << "você perdeu :<" << endl;
    }else if(verificaVencedor(Arvore,1)==3){
        cout << "empate!!" << endl;
    }
    destroi(Arvore);
    return 0;
}

