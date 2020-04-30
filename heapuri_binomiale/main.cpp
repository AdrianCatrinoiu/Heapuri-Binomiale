#include <iostream>
#include <list>
#include <fstream>

using namespace std;
ifstream fin("date.in");
ofstream fout("date.out");
//Structura de nod din heap
struct Nod{
    int data,grad;
    Nod* parinte;
    Nod* copil;
    Nod* frate;
};
//Initializam structura unui nod nou
Nod* NodNou(int cheie)
{
    Nod *nou=new Nod;
    nou->data=cheie;
    nou->grad=0;
    nou->parinte=NULL;
    nou->copil=NULL;
    nou->frate=NULL;
    return nou;
}
//Facem merge intre 2 arbori binomiali
Nod* mergeHeap(Nod *n1,Nod* n2)
{
    if(n1->data > n2->data)
        swap(n1,n2);        //Ne asiguram ca nodul 1 este mai mic
    //Facem arborele cu valori mai mari copilul celui cu valori mai mici
    n2->parinte=n1;
    n2->frate=n1->copil;
    n1->copil=n2;
    n1->grad++;
    return n1;
}
//Functie care face merge intre 2 heap-uri
list<Nod*> reuniuneHeapuri(list<Nod*> h1,list<Nod*> h2)
{
        list<Nod*> nou; // nou va deveni heap-ul rezultat in merge-ul lui l1 si l2
        list<Nod*>::iterator rad1=h1.begin();
        list<Nod*>::iterator rad2=h2.begin();
        while(rad1!=h1.end()&&rad2!=h2.end())
        {
            //daca gradul al primului heap este mai mic decat al celui de al doilea atunci adaugam primul heap
            if((*rad1)->grad<=(*rad2)->grad)
            {
                nou.push_back(*rad1);
                rad1++;
            }
            else
            {
                nou.push_back(*rad2);
                rad2++;
            }
        }
         //verificam daca raman elemente in primul heap
            while(rad1!=h1.end())
            {
                nou.push_back(*rad1);
                rad1++;
            }
            //verificam daca raman elemente in al doilea heap
            while(rad2!=h2.end())
            {
                nou.push_back(*rad2);
                rad2++;
            }
        return nou;//returnam merge-ul dintre cele 2 heap-uri
}
//Ordonam heap-ul ca sa ne asiguram ca este in ordinea crescatoare a gradurilor
//si ca nu exista doi arbori binomiali cu acelasi grad
list<Nod*> ordonare(list<Nod*> heap)
{
    if(heap.size()<=1)
        return heap;
    list<Nod*> heap_nou;
    list<Nod*>::iterator rad1,rad2,rad3;
    rad1=rad2=rad3=heap.begin();//luam toate radacinile de la inceput

    if(heap.size()==2)
        {
            rad2=rad1;
            rad2++;
            rad3=heap.end();
        }
        else
        {
            rad2++;
            rad3=rad2;
            rad3++;
        }
        while(rad1!=heap.end())
        {   //daca mai avem doar un element pana terminam ordonarea trecem direct la el
            if(rad2==heap.end())
                rad1++;
                //daca nu se poate face reuniunea arborilor binomiali care incep din rad1 si rad2
                //trecem mai departe in heap
            else if((*rad1)->grad<(*rad2)->grad)
            {
                rad1++;
                rad2++;
                if(rad3!=heap.end())
                    rad3++;
            }
            //daca trei arbori consecutivi au acelasi grad avansam cu toate rad
            else if(rad3!=heap.end()&&(*rad1)->grad==(*rad2)->grad&&(*rad1)->grad==(*rad3)->grad)
            {
                rad1++;
                rad2++;
                rad3++;
            }
            //daca gradul a doi arbori este acelasi atunci facem merge-ul lor in prima radacina
            //si stergem arborele de pe radacina 2 formand astfel un arbore de grad dublu
            else if((*rad1)->grad==(*rad2)->grad)
            {
                *rad1=mergeHeap(*rad1,*rad2);
                rad2=heap.erase(rad2);
                if(rad3!=heap.end())
                    rad3++;
            }
        }
        return heap;
}
//Inseram un arbore intr-un heap
list<Nod*> insertArbore(list<Nod*> heap, Nod* arbore)
{
    list<Nod*> heap_temp;
    heap_temp.push_back(arbore);
    heap_temp=reuniuneHeapuri(heap,heap_temp);
    return ordonare(heap_temp);//ordonam heap-ul rezultat in urma inserarii unui arbore
}
//Inseram un nod nou in heap
list<Nod*> insert_nod(list<Nod*> cap, int cheie)
{
    Nod *nou=NodNou(cheie);
    return insertArbore(cap,nou);
}

//Scoatem elementul minim dintr-un heap prin scoaterea radacinii a unui arbore transmis ca parametru
list<Nod*> removeMinim(Nod* arbore)
{
    list<Nod*> heap;
    Nod *nou=arbore->copil;//initializam un nod nou de la copilul radacinii arborelui
    Nod *nou2;//nou2 va retine valoarea lui nou
    while(nou)//facem un heap din arbore
    {
        nou2=nou;
        nou=nou->frate;//nou ia valoarea fratelui sau
        nou2->frate=NULL;
        heap.push_front(nou2);
    }
    return heap;
}
//returnam pointer catre nodul cu valoare minima din heap
Nod* getMinim(list<Nod*> heap)
{
    list<Nod*>::iterator rad=heap.begin();
    Nod *mini=*rad;//incepem de la radacina
    while(rad!=heap.end())
    {
        if((*rad)->data<mini->data)
            mini=*rad;
        rad++;
    }
    return mini;
}
//Vom scoate minimul din heap
list<Nod*> extractMin(list<Nod*> heap)
{
    list<Nod*> new_heap,nou;
    Nod *mini;

    // mini reprezinta pointer catre elementul minim din heap
    mini = getMinim(heap);
    list<Nod*>::iterator rad;
    rad = heap.begin();
    while (rad != heap.end())
    {
        if (*rad != mini)
        {
            //Adaugam intr-un heap nou toti arborii inafara de arborele care il contine pe mini
            new_heap.push_back(*rad);
        }
        rad++;
    }
    nou = removeMinim(mini);//nou va deveni un heap format din arborele care continea elementul cu valoare
                            //minima, dupa ce aceasta valoare a fost scoasa
    new_heap = reuniuneHeapuri(new_heap,nou);//reunim arborii care nu il contineau pe mini cu noul heap
                                            //din care acesta a fost scos
    new_heap = ordonare(new_heap);//aranjam noul heap format
    return new_heap;
}
//Cautam un nod dupa o valoare data
Nod* cauta(Nod* n,int val)
{
    if(n==NULL)
        return NULL;
    if(n->data==val)
    {
        return n;
    }

    Nod* m=cauta(n->copil,val);
    if(m!=NULL)
        return m;
    return cauta(n->frate,val);
}
//Scadem valoarea nodului cautat la INT_MIN ca sa il putem apoi face minimul heap-ului si sterge cu extractMin
void scadereValoareNod(list<Nod*> heap,int val_veche,int val_noua)
{
    list<Nod*>::iterator rad=heap.begin();
    Nod* n=NULL;
    while(rad!=heap.end()&&n==NULL)
        {
            n=cauta(*rad,val_veche);
            rad++;
        }
    if(n==NULL)
        return ;
    n->data=val_noua;
    Nod* parinte=n->parinte;
    while(parinte!=NULL&&n->data<parinte->data)
    {
        swap(n->data,parinte->data);
        n=parinte;
        parinte=parinte->parinte;
    }
}
//Afisam un arbore dat
void afisare_arbore(Nod *h)
{
    while (h)
    {
        fout << h->data << " ";
        afisare_arbore(h->copil);
        h = h->frate;
    }
}
//Afisam un heap
void afisare_heap(list<Nod*> heap)
{
    list<Nod*> ::iterator rad;
    rad = heap.begin();
    while (rad != heap.end())
    {
        afisare_arbore(*rad);
        rad++;
    }
    fout<<'\n';
}
//Stergem un nod dat dupa ce i-am schimbat valoarea in INT_MIN ca acesta sa devina noul minim
//ca sa ii putem da apoi apel functiei extractMin
void delete_nod(list<Nod*> &heap,int val)
{
    scadereValoareNod(heap,val,INT_MIN);
    heap=extractMin(heap);

}
//Functie de build dintr-un vector de noduri citite
void build_h(list<Nod*> &heap,int v[],int n)
{
    int i;
    for(i=0;i<n;i++)
        heap=insert_nod(heap,v[i]);
}
int main()
{
    int n;
    int operatie,x;
    int i;
    int v[100002];
    list<Nod*> heap,heap1,heap2,heap3;
    Nod *temp;

    fin>>n;
    //heap 1
    fout<<"Heap 1:\n";
    for(i=0;i<n;i++)
        fin>>v[i];
    build_h(heap1,v,n);
    afisare_heap(heap1);
    fout<<'\n';


    //heap 2
    fout<<"Heap 2:\n";
    fin>>n;
    for(i=0;i<n;i++)
        fin>>v[i];
    build_h(heap2,v,n);
    afisare_heap(heap2);
    fout<<'\n';

    //merge intre heap1 si heap2
    fout<<"Merge intre heap1 si heap2:\n";
    heap3=reuniuneHeapuri(heap1,heap2);
    heap3=ordonare(heap3);
    afisare_heap(heap3);
    fout<<'\n';


    fout<<"Operatii pentru heap:\n";
    fin>>n;
    for(i=0;i<n;i++)
    {
        fin>>operatie;
        if(operatie==1)
        {
            fin>>x;
            fout<<"Adaugam nodul: "<<x<<" \n";
            heap=insert_nod(heap,x);
            afisare_heap(heap);
            fout<<'\n';
        }
        if(operatie==2)
        {
            fin>>x;
            fout<<"Stergem nodul: "<<x<<" \n";
            delete_nod(heap,x);
            afisare_heap(heap);
            fout<<'\n';
        }
        if(operatie==3)
        {
            temp=getMinim(heap);
            fout<<"Minim: "<<temp->data<<'\n';
            fout<<'\n';
        }
        if(operatie==4)
        {
            fout<<"Stergem Minimul: \n";
            heap=extractMin(heap);
            afisare_heap(heap);
            fout<<'\n';
        }
    }
    return 0;
}
