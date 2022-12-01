#include <iostream>
using namespace std;

class Directory{ 
public:
    Directory(const std::string& name);
    std::string toString();

private :
    std::string theName;
};

//Item 4   - using the member initialization list here

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
    File(string name, string extension, int fileSize);
    string toString();
    // Getter
    string getName() {
      return this->theName;
    }
    string getExtension() {
      return this->theExtension;
    }
    int getFileSize() {
      return this->theFileSize;
    }

    File& operator=(File& other)           //ITEM 10: Have assignment operators return a reference to *this.
    {
        std::cout << "copy assignment of File\n";
         if (this != &other) // not a self-assignment  ITEM 11: To handle assignment to self
        {
        this->theName= other.theName;
        this->theExtension = other.theExtension;
        this->theFileSize = other.theFileSize;
        } 
        else{
            cout<<"Assignment to self\n";
        }
        return *this;           //ITEM 10: Have assignment operators return a reference to *this.
    }

private :
    string theName;
    string theExtension;
    int theFileSize;
};

// constructor with parameters
File::File(string name, string extension,int fileSize):
    theName(name),
    theExtension(extension),
    theFileSize(fileSize)
    {}            


std::string File::toString(){
    return "Nume: "+ theName + theExtension + " Dimensiune fisier: " + std::to_string(theFileSize) + " KB" + "  Folder: " +  dir().toString();
}


//ITEM 12 here illustrated: Copy all parts of an object
class SpecialFile : public File{
    public:
    SpecialFile(string name, string extension, int fileSize, int priority);
    SpecialFile& operator=(SpecialFile& rhs);
    std::string toString();

    private:
    int thePriority;
};

SpecialFile::SpecialFile(string name, string extension, int fileSize, int priority)
:File(name, extension, fileSize),       // invoke base class copy ctor
thePriority(priority)
{}

SpecialFile&SpecialFile::operator=(SpecialFile& rhs){
    if (this == &rhs) return *this;     // identity test: if a self-assignment,
                                        // do nothing

    File::operator=(rhs);               // assign base class parts
    thePriority = rhs.thePriority;

    return *this;
}

std::string SpecialFile::toString(){
    return File::toString() +", Priority: " + std::to_string(thePriority);
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
    std::cout<<'\n';
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
    //File f;  <= eroare pentru ca File are deja constructor
    File f1(fisier);
    std::cout<<(f1.toString());
    std::cout<<'\n';
    f1 = fisier;
    std::cout<<(f1.toString());
    f1.~File();
    std::cout<<'\n';

    //Item 6 - Explicitly disallow the use of compiler generated function you do not want.
        //Prin faptul ca am facut ca UncopyableFile sa mosteneasca pe Uncopyable, copy constructorul si copy assignement operator nu pot fi accesate 
        //pentru ca sunt private in clasa parinte
    UncopyableFile u1("nusepoatecopia.txt");
        //UncopyableFile u2(u1);  trebuie puse in comentarii pentru ca altfel primim eroare
        //UncopyableFile u2 = u1;



    //ITEM 10: Have assignment operators return a reference to *this
    std::cout<<"ITEM 10";
    std::cout<<'\n';

    File f10("testItem10_1",".txt", 3);
    File f11("testItem10_2",".txt", 4);
    File f12("testItem10_3",".txt", 5);
    
    f10=f11=f12;
    std::cout<<(f10.toString())<<'\n';

    //ITEM 11: Handle assignment to self in operator=
    std::cout<<"ITEM 11"<<'\n';
    File item11("item11",".txt", 5);
    item11=item11;
    std::cout<<(item11.toString())<<'\n';

    //ITEM 12 Copy all parts of an object
    SpecialFile special("VERYspecial",".txt",1000,1);
    std::cout<<special.toString()<<'\n';

    return 0;
}