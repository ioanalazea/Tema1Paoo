#include <iostream>


//Item 4   - using the member initialization list here
class Directory{ 
public:
    Directory(const std::string& name);
    std::string toString();

private :
    std::string theName;
};


Directory::Directory(const std::string& name)
:theName(name)
 {}
std::string Directory::toString(){
    return "Nume: "+ theName;
}
 
//Pentru ca folosim Directory din toString-ul de la file, am creat un obiect static care sa fie initializat
Directory& dir(){
    static Directory dir("Folder1");                    //definesc local static object
    return dir;
}


class File{
public:
    File(const std::string& name, const std::string& extension, const int fileSize);
    std::string toString();
    
private :
    std::string theName;
    std::string theExtension;
    int theFileSize;
};


File::File(const std::string& name, const std::string& extension, const int fileSize)
    :theName(name),
    theExtension(extension),
    theFileSize(fileSize)
    {}            

std::string File::toString(){
    return "Nume: "+ theName + theExtension + " Dimensiune fisier: " + std::to_string(theFileSize) + " KB" + "  Folder: " +  dir().toString();
}



//Item 5 - default constructor
class RandomEmptyClass{
};



//Item 6
/* Base class */
class Uncopyable
{
    protected:
        Uncopyable() {}                                 //putem folosi constructor si destructor
        ~Uncopyable() {}

    private:
        Uncopyable(const Uncopyable&);                  //nu putem copia
        Uncopyable& operator=(const Uncopyable&);
};

class UncopyableFile : private Uncopyable{
public :
    UncopyableFile(const std::string& name);   
private:
    std::string theName;
};
UncopyableFile::UncopyableFile(const std::string& name)
:theName(name)
 {}


int main() {
    //Item 4 - Make sure the objects are initialized
        //Initializez manual variabila
    int variabilaInit = 2000;   
    std::cout<<variabilaInit;
        //Sa vedem ce se intampla daca nu initializam
    int variabilaNeinit;       
    std::cout<<variabilaNeinit;
    std::cout<<'\n';

        //Apelez constructorul care foloseste member initialization list
    File fisier("fisiertest",".txt", 3);
    std::cout<<(fisier.toString());
    std::cout<<'\n';



    //Item 5 - Know what functions C++ silently writes and calls

        //Functiile (def constr, copy constr, copy assign, destr) sunt generate daca e nevoie de ele
        //In acest caz compilator le creeaza pentru ca are nevoie de ele
        //Daca vreuna exista deja in clasa, nu mai era nevoie sa fie creata
        //Testam pt o clasa goala
    RandomEmptyClass e1;        //default constructor
    RandomEmptyClass e2(e1);    //copy constructor
    e2 = e1;                    //copy assignment operator
    e1.~RandomEmptyClass();     //destructor



        //Testam pentru o clasa la care am constructor, dar nu am copy constr, copy assign si destructor
        //Se observa ca functiile mentionate au fost create de compilator, motiv pentru care le si putem folosi
    File f1(fisier);
    std::cout<<(f1.toString());
    std::cout<<'\n';
    f1 = fisier;
    std::cout<<(f1.toString());
    f1.~File();


    //Item 6 - Explicitly disallow the use of compiler generated function you do not want.
        //Prin faptul ca am facut ca UncopyableFile sa mosteneasca pe Uncopyable, copy constructorul si copy assignement operator nu pot fi accesate 
        //pentru ca sunt private in clasa parinte
    UncopyableFile u1("nusepoatecopia.txt");
        //UncopyableFile u2(u1);  trebuie puse in comentarii pentru ca altfel primim eroare
        //UncopyableFile u2 = u1;

    return 0;
}