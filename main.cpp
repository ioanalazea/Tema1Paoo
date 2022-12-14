#include <iostream>
#include <memory>
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
    File(void);                            //default constructor
    File(string name, string extension, int fileSize);
    string toString();
    File* createFile();                   //Item 13: Use objects to manage resources.

    //Setter
    void setName(string name){
    this->theName = name;
    }
     void setExtension(string extension){
    this->theExtension = extension;
    }
     void setFileSize(int fileSize){
    this->theFileSize = fileSize;
    }
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
        return *this;                      //ITEM 10: Have assignment operators return a reference to *this.
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

//default constructor              
File::File():
    theName(),
    theExtension(),
    theFileSize()
    {}  

std::string File::toString(){
    return "Nume: "+ theName + theExtension + " Dimensiune fisier: " + std::to_string(theFileSize) + " KB" + "  Folder: " +  dir().toString();
}

File* createFile(){                   //Item 13: Use objects to manage resources.
    return (new File);
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


//ITEM 13
//Function to illustrate the premature return statement and how the delete statement is not reached.
int noRAII(){
    File *f = createFile();
    f->setFileSize(100);
    if (f->getFileSize() != 0) return -1;
    cout<<"got to delete\n";
    delete f;
    return 0;
}




//ITEM 14:Think carefully about copying behavior in resource-managing classes.
class FileWithPassword{
    private:
        string theName;
        bool confidential = false;
    public:

    FileWithPassword(string name){
        this->theName = name;
    }

    FileWithPassword(const FileWithPassword &f){
        this->theName = f.theName;
        this->confidential = f.confidential;
    }

    ~FileWithPassword(){
        std::cout<<"File with password deleted."<<std::endl;
    }

    string getName(){
        return this->theName;
    }

    bool getConfidential(){
        return this->confidential;
    }

    void setConfidential(bool confidential){
        this->confidential = confidential;
    }

    void isFileConfidential(){
        if(this->confidential == false) 
            std::cout<<"The file "<< this->theName <<" is not confidential and doesn't have a password."<<"\n";
        else
            std::cout<<"The file "<<this->theName<<"is confidential and can't be accessed."<<"\n";
    }

};

void lock(FileWithPassword &f){         //lock and unlock functions for managing resources
    f.setConfidential(true);
}

void unlock(FileWithPassword &f){
    f.setConfidential(false);
}

class OwnerFileWithPassword{            //owner will lock or unlock the file
    private:
        FileWithPassword &file;
    public:
    OwnerFileWithPassword(FileWithPassword &f):
    file(f){
        lock(file);
    }
    ~OwnerFileWithPassword(){
        unlock(file);
    }
};


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
    std::cout<<"ITEM 10"<<'\n';

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




     //Item 13: Use objects to manage resources.
    std::cout<<'\n'<<"ITEM 13"<<'\n';
   
    auto_ptr<File> f2(createFile());
    f2->setFileSize(240);
    auto_ptr<File> f3(f2);
    //cout<<"\n"<<f2->getFileSize()<<"\n";       //Error Segmentation fault (core dumped) because f2 is now null
    std::cout<<"\n"<<f3->getFileSize()<<"\n"; 

    shared_ptr<File> f4(createFile());
    f4->setFileSize(128);
    shared_ptr<File> f5;
    f5 = f4;
    std::cout<<"\n"<<f4->getFileSize()<<"\n";       //both f4 and f5 now point to the object
    std::cout<<"\n"<<f5->getFileSize()<<"\n";        //shared_ptr and auto_ptr release resources in their destructors -> prevent resource leaks
                                                        

    //noRAII();
    //the delete statement isn't reached


    //ITEM 14: Think carefully about copying behavior in resource-managing classes.
    std::cout<<'\n'<<"ITEM 14"<<'\n';
    FileWithPassword fileWithPass("myCONFIDENTIALnotes.txt");
    fileWithPass.isFileConfidential();                  //File unlocked doesn't have an owner yet.
    std::cout<<"\n";
    OwnerFileWithPassword owner(fileWithPass);          //Now the file will be "locked" since we created an owner.
    fileWithPass.isFileConfidential();
    std::cout<<"\n";

    return 0;
}