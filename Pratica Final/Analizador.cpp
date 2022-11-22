#include <iostream>
#include <fstream>
#include <string>
#include <locale.h>
#include <stdlib.h>
#include "Node.h"
using namespace std;

bool developerMode;
string inputHolder;

bool searchWord(string wordToSearchFor, string fileName){
    
    ifstream fileInput;
    string lineHolder;
    fileInput.open(fileName);

    if (!fileInput.is_open())
    {
        cout << "*******Error abriendo archivo*******";
        return false;
    }
    else{
        while (fileInput >> lineHolder){
            if (lineHolder == wordToSearchFor)
            {               
                if(developerMode){cout << endl << "~Se encontro la palabra buscada: " << wordToSearchFor << "~" << endl;}
                fileInput.close();
                return true;
            }        
        }
    }
    
    fileInput.close();
    return false;
}

int buscarPos(Node oracionTree, string palabra){
    //ASI LUCE EL ARBOL DE POR SI...
    //               ORACION
    //              /       \
    //          SUJETO      PREDICADO
    //          /   \       /    |    \
    //      NUCLEO  DET  VERBO  ATR / COMPLEMENTO
    //         1     2     3     4         5
    
    // UBICAREMOS SI LA PALABRA EXISTE PRIMERO QUE TODO EN NUESTRO ARCHIVO DE PALABRAS
    if (!searchWord(palabra, "Files/0_palabras_todas.txt"))
    {
        return -1;
    }
    // EMPEZAREMOS POR EL/LOS NUCLEO/S
    if (developerMode){cout << endl << "Analizando: " << palabra << endl;}
    for (int i = 0; i < oracionTree.child1->child1->children.size(); i++){
        if (palabra == oracionTree.child1->child1->children.at(i))
        {
            if(developerMode){cout << endl << palabra << " se encontro en el Nucleo (1)" << endl;}
            if(developerMode){cout << "--------------------------------------------" << endl;}
            return 1;
        }  
    }
    // DETERMINANTE...
    //for (int i = 0; i < oracionTree.child1->child2->children.size(); i++){
    //    if (palabra == oracionTree.child1->child2->children.at(i))
    //    {
    //        if(developerMode){cout << endl << palabra << " se encontro en el Determinante (2)" << endl;}
    //        if(developerMode){cout << "--------------------------------------------" << endl;}
    //        return 2;
    //    }  
    //}
    // VERBO...
    for (int i = 0; i < oracionTree.child2->child1->children.size(); i++){
        if (palabra == oracionTree.child2->child1->children.at(i))
        {
            if(developerMode){cout << endl << palabra << " se encontro en el Verbo (3)" << endl;}
            if(developerMode){cout << "--------------------------------------------" << endl;}
            return 3;
        }  
    }
    // ATRIBUTO
    for (int i = 0; i < oracionTree.child2->child2->children.size(); i++){
        if (palabra == oracionTree.child2->child2->children.at(i))
        {
            if(developerMode){cout << endl << palabra << " se encontro en el Atributo (4)" << endl;}
            if(developerMode){cout << "--------------------------------------------" << endl;}
            return 4;
        }  
    }
    // COMPLEMENTO
    for (int i = 0; i < oracionTree.child2->child3->children.size(); i++){
        if (palabra == oracionTree.child2->child3->children.at(i))
        {
            if(developerMode){cout << endl << palabra << " se encontro en el Complemento (5)" << endl;}
            if(developerMode){cout << "--------------------------------------------" << endl;}
            return 5;
        }  
    }

    // DETERMINANTE...
    for (int i = 0; i < oracionTree.child1->child2->children.size(); i++){
        if (palabra == oracionTree.child1->child2->children.at(i))
        {
            if(developerMode){cout << endl << palabra << " se encontro en el Determinante (2)" << endl;}
            if(developerMode){cout << "--------------------------------------------" << endl;}
            return 2;
        }  
    }     

    if(developerMode){cout << endl << "La palabra no esta en ninguno de las posiciones anteriores (Somehow)" << endl;}
    if(developerMode){cout << "--------------------------------------------" << endl;}
    return 0;

}

bool checkearOracion(Node oracionTree, string palabras[], int numPalabras){
    vector<string> palabrasVector;
    vector<int> ordenTokens;
    int verbos = 0;
    for (size_t i = 0; i < numPalabras; i++)
    {
        palabrasVector.push_back(palabras[i]);
    }

    if(developerMode){cout << endl << "Palabras vector:" << endl;}
    if(developerMode){for (int i = 0; i < numPalabras; i++)
    {
        cout << i << ". " << palabrasVector.at(i) << endl;
    }}

    for (int i = 0; i < numPalabras; i++)
    {
        ordenTokens.push_back(buscarPos(oracionTree, palabrasVector.at(i)));
    }
    // IMPRIMIMOS EL VECTOR DEL ORDEN DE LOS TOKENS
    if(developerMode){for (int i = 0; i < numPalabras; i++)
    {
        cout << i << ". " << ordenTokens.at(i) << endl;
    }}

    
    
    for (int i = 0; i < ordenTokens.size(); i++)
    {

        if (ordenTokens.at(i) == 3){verbos++;}
        // SI UNA DE LAS PALABRAS NUNCA SE CLASIFICO DE ALGUNA MANERA TAL VEZ POSIBLE
        if (ordenTokens.at(i) == -1)
        {
            if(developerMode){cout << endl << "Caso -1 en posicion " << i  << " (La palabra no fue encontrada, revisar si está en el archivo Files/0_palabras_todas.txt)";}
            return false;
        }
        // SI LA ORACION ES TIPO MAESTRO YODA
        // OSEA QUE EL SUJETO Y VERBO (<=3) ESTEN DESPUES DEL PREDICADO (>3)
        if (ordenTokens.at(i) > 3){
            // COMPROBACION PARA NO EXCEDERNOS
            int posicionTemp = i;
            while (posicionTemp+1 != ordenTokens.size())
            {
                posicionTemp++;
                if (ordenTokens.at(posicionTemp) <= 3)
                {
                    if(developerMode){cout << endl << "Caso 1 con: " << ordenTokens.at(i) << " en pos " << i << " con la pos " << posicionTemp;}
                    return false;
                }               
            }            
        }
        // SI LA ORACION TIENE 2 O MAS VERBOS
        if (verbos > 1)
        {
            return false;
        }

    }
    
    return true;

}

int main(int argc, char const *argv[])
{
    #pragma region 0
    setlocale(LC_ALL, "spanish");
    string developerStr;
    system("CLS");
    cout << "¿Modo desarrollador? Si (Y) o No (N)" << endl;
    getline(cin, developerStr);
    if (developerStr == "y" || developerStr == "Y")
    {
        developerMode = true;
    }else{
        developerMode = false;
    }
    system("CLS");
    if(developerMode){cout << endl << "~ MODO DESARROLLADOR ACTIVADO ~" << endl << "~ CCCM ~" << endl;}

    // Algunas fuentes usadas para realizar el proyecto
    //https://sintaxis.org/analizador/
    //https://libguides.cng.edu/ld.php?content_id=56777850
    //https://libguides.cng.edu/EspanolHS/Sintaxis
    //https://infoling.org/informacion/RecursoL72.html
    //https://cmapspublic2.ihmc.us/rid=1KM073MQ0-25341R0-1FF6/LA%20ORACI%C3%93N%20SIMPLE.cmap?rid=1KM073MQ0-25341R0-1FF6&partName=htmljpeg
    //https://i0.wp.com/www.sonria.com/wp-content/uploads/2017/11/Determinantes.jpg
    #pragma endregion
    #pragma region 1
    //1. LECTURA DEL INPUT Y TRANSFORMACION A LOWERCASE PARA ANALIZAR
    if(developerMode){cout << endl << "-------------1-------------" << endl;}

    string sentenceStr;

    cout << "Ingrese una oracion: ";
    getline(cin, sentenceStr);
    string lowercaseInput = "";
    for (char& c : sentenceStr){
        lowercaseInput += tolower(c);
    }
    //1. END
    #pragma endregion
    #pragma region 2
    //2. PASAR CADA PALABRA A UNA LISTA
    if(developerMode){cout << endl << "-------------2-------------" << endl;}
    string tempString = "";
    string wordsArray[20];
    int numPalabras = 0;
    lowercaseInput += " ";
    for (char c : lowercaseInput){
        if (c == ' '){
            wordsArray[numPalabras] = tempString;
            numPalabras += 1;
            tempString.clear();
        }
        else{
            tempString += c;
        }
    }

    if(developerMode){cout << endl << "Num palabras: " << numPalabras << endl;}

    if(developerMode){for (int i = 0; i < numPalabras; i++)
    {
        cout << endl << i << ". " << wordsArray[i];
    }}
    
    //2. END

    //2.1. CREAMOS EL ARBOL QUE VA A CONTENER LA ORACION
    Node sentenceTree(sentenceStr);

    //PRUEBA CON VECTOR
    //cout << endl << "Testeto vector -------------" << endl;
    //sentenceTree.children.push_back("Cris");
    //cout << sentenceTree.children.at(0);
    //TEST PASSED
    //2.1. END
    #pragma endregion
    #pragma region 3
    if(developerMode){cout << endl << "-------------3-------------" << endl;}
    string verb = "";
    int verbPos = -1;
    int verbsCount = 0;

    //CREAR EL BOOLEANO CON EL TIPO DE PREDICADO (NOMINAL SI ES SER, ESTAR, PARECER)
    bool predicadoNominal = false;

    for (int i = 0; i < numPalabras; i++)
    {
        if (searchWord(wordsArray[i], "Files/verbos-espanol-copulativos.txt")){
            verb = wordsArray[i];
            verbPos = i;
            predicadoNominal = true;
            if(developerMode){cout << endl << "Nominal" << endl;}
            if(developerMode){cout << endl << "Verbo encontrado, PREDICADO DE TIPO NOMINAL" << endl;}
            verbsCount++;
            if (verbsCount > 1)
            {
                cout << endl << "Oracion invalida, se encontraron mas de dos verbos" << endl;
                cout << endl << "Presiona la tecla ENTER para finalizar...";  
                getline(cin, inputHolder);
                return 0;
            }
            
        }
    }
    //SI NO ENCONTRAMOS EN LOS COPULATIVOS, BUSCAMOS EN EL RESTO.
    for (int i = 0; i < numPalabras; i++)
    {
        if (searchWord(wordsArray[i], "Files/verbos-espanol-conjugaciones.txt")){
            verb = wordsArray[i];
            verbPos = i;
            if(developerMode){cout << endl << "Verbal" << endl;}
            if(developerMode){cout << endl << "Verbo encontrado, PREDICADO DE TIPO VERBAL" << endl;}
            verbsCount++;
            if (verbsCount > 1)
            {
                cout << endl << "Oracion invalida, se encontraron mas de dos verbos" << endl;
                cout << endl << "Presiona la tecla ENTER para finalizar...";  
                getline(cin, inputHolder);
                return 0;
            }
        }
    }
    if (verb == "")
    {
        cout << endl << "Oracion invalida, no se encontro algun verbo en los archivos de verbos" << endl;
        cout << endl << "Presiona la tecla ENTER para finalizar...";
        getline(cin, inputHolder);
        return 0;
    }
    #pragma endregion
    #pragma region 4
    //4. UBICAR EL SUJETO BASANDONOS EN LA UBICACIÓN DEL VERBO
    //CREAMOS LA LISTA HOLDER DE LAS PALABRAS SUJETO
    if(developerMode){cout << endl << "-------------4-------------" << endl;}
    string sujetoHolder[10];
    int contSujeto = 0;
    int ultimaPosSujeto = -1;

    for (int i = 0; i < verbPos; i++){
        if (searchWord(wordsArray[i], "Files/sustantivos propios-espanol.txt")){
            if (i>ultimaPosSujeto){
                ultimaPosSujeto = i;
            }        
        }
    }
    //SI SIGUE VACIO (NO ENCONTRAMOS EN LOS PROPIOS, BUSCAMOS EN LOS COMUNES O IMPROPIOS)
    for (int i = 0; i < verbPos; i++){
        if (searchWord(wordsArray[i], "Files/sustantivos comunes-espanol.txt")){
            if (i>ultimaPosSujeto){
                ultimaPosSujeto = i;
            }
        }
    }

    if(developerMode){cout << endl << "Ultima pos sujeto: " << ultimaPosSujeto << endl;}
    

    for (int i = 0; i < ultimaPosSujeto+1; i++)
    {
        sujetoHolder[i] = wordsArray[i];
        contSujeto++;        
    }

    if(developerMode){cout << endl << "Num palabras sujeto: " << contSujeto << endl;}

    if(developerMode){for (int i = 0; i < contSujeto; i++)
    {
        cout << endl << i << ". " << sujetoHolder[i];
    }}

    //4. END
    #pragma endregion
    #pragma region 5
    //5. UBICAR EL PREDICADO BASANDONOS EN LA UBICACIÓN DEL SUJETO
    //CREAMOS LA LISTA HOLDER DE LAS PALABRAS PREDICADO
    if(developerMode){cout << endl << "-------------5-------------" << endl;}
    string predicadoHolder[10];
    int contPredicado = 0;
    int tempCont = 0;
    
    for (int i = ultimaPosSujeto+1; i < numPalabras; i++)
    {
        predicadoHolder[tempCont] = wordsArray[i];
        contPredicado++;
        tempCont++;
    }

    if(developerMode){cout << endl << "Num palabras predicado: " << contPredicado << endl;}

    if(developerMode){for (int i = 0; i < contPredicado; i++)
    {
        cout << endl << i << ". " << predicadoHolder[i];
    }}

    //5. END
    #pragma endregion
    #pragma region 6
    //6. PRIMEROS COMIENZOS DE RELLENO DEL ARBOL CON EL SUJETO Y EL VERBO
    //UNIREMOS LAS PALABRAS DEL SUJETO Y SERÁ EL HIJO DEL ARBOL
    if(developerMode){cout << endl << "-------------6-------------" << endl;}
    string sujetoSentence = "";
    for (int i = 0; i < contSujeto; i++)
    {
        sujetoSentence += sujetoHolder[i];
        sujetoSentence += " ";
    }
    //QUITAMOS EL ESPACIO EXTRA DEL FINAL
    if (verbsCount != 0){
        sujetoSentence.pop_back();
    }
    
    //REPETIMOS CON EL PREDICADO
    string predicadoSentence = "";
    for (int i = 0; i < contPredicado; i++)
    {
        predicadoSentence += predicadoHolder[i];
        predicadoSentence += " ";     
    }

    predicadoSentence.pop_back();
    //6.1 CREAMOS UNA RAMA PARA SUJETO Y UNA PARA PREDICADO
    if (contSujeto == 0)
    {
        sujetoSentence = "#";
    }

    Node Sujeto(sujetoSentence);
    sentenceTree.child1 = &Sujeto;
    Node Predicado(predicadoSentence);
    sentenceTree.child2 = &Predicado;

    //6. END
    #pragma endregion
    #pragma region 7
    //7. ANALIZAREMOS EL SUJETO AHORA
    // ¿cual sujeto ~ ?
    if(developerMode){cout << endl << "-------------7-------------" << endl;}

    //EMPEZAREMOS CON EL NUCLEO
    Node Nucleo("Nucleo");
    Nucleo.tokenType = 1;
    Sujeto.child1 = &Nucleo;
    //LO PONEMOS DE HIJO DEL SUJETO
    //
    int nucleoCont = 0;
    int lastNucleoPos = -1;

    //COMPASION ACA, QUE MIS ARCHIVOS DE TEXTO TIENEN LOS MAS COMUNES, MAS NO TODOS...
    //BUSCAMOS EN LOS SUSTANTIVOS PROPIOS
    for (int i = 0; i < contSujeto; i++){
        if (searchWord(sujetoHolder[i], "Files/sustantivos propios-espanol.txt")){
            Nucleo.children.push_back(sujetoHolder[i]);
            nucleoCont++;
            lastNucleoPos = i;
        }
    }
    //SI SIGUE VACIO (NO ENCONTRAMOS EN LOS PROPIOS, BUSCAMOS EN LOS COMUNES O IMPROPIOS)
    for (int i = 0; i < contSujeto; i++){
        if (searchWord(sujetoHolder[i], "Files/sustantivos comunes-espanol.txt")){
            Nucleo.children.push_back(sujetoHolder[i]);
            nucleoCont++;
            lastNucleoPos = i;
        }
    }
    //IMPRIMIR VECTORES CONTENEDORES DE LOS NUCLEOS
    if(developerMode){cout << endl << "Vectores del nucleo:";}
    if(developerMode){for (int i = 0; i < nucleoCont; i++)
    {
        cout << endl << "Vector " << i << ". " << Nucleo.children.at(i);
    }}
    
    
    //SI NO EXISTE NI EN LOS PROPIOS NI EN LOS IMPROPIOS:
    if (Nucleo.children.empty()){
        if(developerMode){cout << endl << "No existe ni un nucleo" << endl;}
    }else{
        if(developerMode){cout << endl << "Nucleo/s encontrado/s" << endl;}
    }

    //Iba a usar modificadores, pero era mejor usar determinantes y ya
    #pragma region MODIFICADORES
    //SEGUIREMOS CON EL MOD DIRECTO
    //Node MDirecto("MDirecto");
    //MDirecto.tokenType = 2;
    //Sujeto.child2 = &MDirecto;
    //int modDCont = 0;
//
    //for (int i = 0; i < contSujeto; i++)
    //{
    //    if (searchWord(sujetoHolder[i], "Files/adjetivos-espanol.txt")){
    //        MDirecto.children.push_back(sujetoHolder[i]);
    //        modDCont++;
    //    }
    //}
    //for (int i = 0; i < contSujeto; i++)
    //{
    //    if (searchWord(sujetoHolder[i], "Files/pronombres-espanol.txt")){
    //        MDirecto.children.push_back(sujetoHolder[i]);
    //        modDCont++;
    //    }
    //}
//
    ////IMPRIMIR VECTORES CONTENEDORES DE LOS NUCLEOS
    //if(developerMode){cout << "Vectores del MD:";}
    //for (int i = 0; i < modDCont; i++)
    //{
    //    if(developerMode){cout << endl << "Vector " << i << ". " << MDirecto.children.at(i);}
    //}
//
    //if (MDirecto.children.empty())
    //{
    //    if(developerMode){cout << "No existe ni un Modificador Directo" << endl;}
    //}else{
    //    if(developerMode){cout << endl << "Modificador/es Directo/s encontrado/s" << endl;}
    //}
//
    ////FIN CON EL MOD INDIRECTO
    //Node MIndirecto("MIndirecto");
    //MIndirecto.tokenType = 3;
    //Sujeto.child3 = &MIndirecto;
    //int modIDCont = 0;
    //int modIDPos = 999;
//
    //for (int i = lastNucleoPos-1; i < contSujeto; i++)
    //{
    //    if (searchWord(sujetoHolder[i], "Files/mi-espanol.txt")){
    //        MIndirecto.children.push_back(sujetoHolder[i]);
    //        modIDCont++;
    //        modIDPos = i;
    //        break;
    //    }
    //}
    //if(developerMode){cout << endl << "MODIDPOS: " << modIDPos << endl;}
    //if(developerMode){for (int i = modIDPos+1; i < contSujeto; i++)
    //{
    //    MIndirecto.children.push_back(sujetoHolder[i]);
    //    modIDCont++;
    //}}
    //
//
    ////IMPRIMIR VECTORES CONTENEDORES DE LOS NUCLEOS
    //if(developerMode){cout << "Vectores del MID:";}
    //if(developerMode){for (int i = 0; i < modIDCont; i++)
    //{
    //    cout << endl << "Vector " << i << ". " << MIndirecto.children.at(i);
    //}}
//
    //if (MIndirecto.children.empty())
    //{
    //    if(developerMode){cout << "No existe ni un Modificador Indirecto" << endl;}
    //}else{
    //    if(developerMode){cout << endl << "Modificador/es Indirecto/s encontrado/s" << endl;}
    //}
    #pragma endregion

    #pragma region DETERMINANTES
    Node Determinantes("Determinantes");
    int determinantesCont = 0;
    Determinantes.tokenType = 2;
    Sujeto.child2 = &Determinantes;

    for (int i = 0; i < numPalabras; i++)
    {
        if (searchWord(wordsArray[i], "Files/determinantes-espanol.txt")){
            Determinantes.children.push_back(wordsArray[i]);
            determinantesCont++;
        }
    }

    //IMPRIMIR VECTORES CONTENEDORES DE LOS DETERMINANTES
    if(developerMode){cout << "Vectores de los Determinantes:";}
    for (int i = 0; i < determinantesCont; i++)
    {
        if(developerMode){cout << endl << "Vector " << i << ". " << Determinantes.children.at(i);}
    }

    #pragma endregion

    //7. END
    #pragma endregion
    #pragma region 8
    //8. ANALIZAREMOS EL PREDICADO AHORA
    if(developerMode){cout << endl << "-------------8-------------" << endl;}
    // CREAMOS EL HIJO VERBO
    Node Verbo("Verbo");
    Verbo.tokenType = 3;
    Verbo.children.push_back(verb);
    Predicado.child1 = &Verbo;
    //CREAMOS EL HIJO ATRIBUTO
    Node Atributo("Atributo");
    Atributo.tokenType = 4;
    Predicado.child2 = &Atributo;
    //CREAMOS EL HIJO COMPLEMENTO
    Node Complementos("Complementos");
    Complementos.tokenType = 5;
    Predicado.child3 = &Complementos;
    // SI ES NOMINAL, BUSCAREMOS ATRIBUTOS
    if (predicadoNominal)
    {
        int atributosCount = 0;
        bool tieneAtributo = false;
        //POR AHORA SIRVEN BIEN LOS ADJETIVOS
        //EMPEZAREMOS DESDE LA POSICION DEL VERBO (0) + 1 (PARA NO CONTARLO INNECESARIAMENTE)
        for (int i = 1; i < contPredicado; i++)
        {
            if (searchWord(predicadoHolder[i], "Files/adjetivos-espanol.txt")){
                tieneAtributo = true;
                break;
            }
        }
        //Agregaremos todo lo que este adelante del verbo como atributo
        if (tieneAtributo){
            for (int i = 1; i < contPredicado; i++)
            {
                Atributo.children.push_back(predicadoHolder[i]);
                atributosCount++;
            }        
        }else{
            cout << endl << "Oracion Invalida, no existe algun atributo o la oración está mal ordenada" << endl;
            cout << endl << "Presiona la tecla ENTER para finalizar...";  
            getline(cin, inputHolder);
            return 0;
        }

        //IMPRIMIR VECTORES CONTENEDORES DE LOS NUCLEOS
        if(developerMode){cout << endl << "Vectores del atributo:";}
        if(developerMode){for (int i = 0; i < atributosCount; i++)
        {
            cout << endl << "Vector " << i << ". " << Atributo.children.at(i);
        }}

    }else{
        //8.1. EN CASO DE NO SER NOMINAL
        if(developerMode){cout << endl << "-------------8.1-------------" << endl;}
        //Node CD("CD");
        //Complementos.child1 = &CD;
        //Node CC("CC");
        //Complementos.child2 = &CC;
        //Node CI("CI");
        //Complementos.child3 = &CI;
        int complementosCount = 0;

        for (int i = 1; i < contPredicado; i++)
        {
            Complementos.children.push_back(predicadoHolder[i]);
            complementosCount++;
        }
        
        //IMPRIMIR VECTORES CONTENEDORES DE LOS COMPLEMENTOS
        if(developerMode){cout << endl << "Vectores del complemento:";}
        if(developerMode){for (int i = 0; i < complementosCount; i++)
        {
            cout << endl << "Vector " << i << ". " << Complementos.children.at(i);
        }}
        
    }
    
    #pragma endregion
    #pragma region 9
    if(developerMode){cout << endl << "-------------9-------------" << endl;}
    //VAMOS A CHECKEAR SI LA ORACION Y SU ORDEN ES CORRECTO USANDO SU ARBOL
    if (checkearOracion(sentenceTree, wordsArray, numPalabras)){
        cout << endl << "Oracion simple valida" << endl;
    }else{
        cout << endl << "Oracion invalida" << endl;
    }

    cout << endl << "Presiona la tecla ENTER para finalizar...";  
    getline(cin, inputHolder);
    #pragma endregion
    
    return 0;
}